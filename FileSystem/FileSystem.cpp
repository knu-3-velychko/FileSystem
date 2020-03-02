#include <fstream>
#include "FileSystem.h"

const int BLOCKS = 64;
const int BLOCK_SIZE = 64;      // 64 bytes = 16 integers
const int BITMAP_SIZE = 64;     // 64 bits = 2 integers
const int DESCRIPTOR_SIZE = 4;  // 4 integers
const int FILE_LENGTH = 1;      // 1 int
const int MASK_SIZE = 32;

const int NUMBER_OF_DESCRIPTORS = 24;   // 6 blocks
const int MAX_FILE_NAME_LENGTH = 4; // 4 chars
const int DIRECTORY_ENTRY = 2;  // 2 integers
const int DESCRIPTOR_INDEX = 1; // 1 integer

using namespace std;

FileSystem::FileSystem(std::ostream *out) {
    this->out = out;

    ldisk = new IOSystem();
    buffer = new char[BLOCK_SIZE];
    mask = new unsigned int[MASK_SIZE];
    oft = new OFT[MAX_FILE_NAME_LENGTH];
    descriptor = new int[DESCRIPTOR_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) {
        buffer[i] = '\0';
    }
    for (int i = 0; i < BITMAP_SIZE; i++) {
        ldisk->write_block(i, buffer);
    }
    for (int i = 31; i >= 0; i--) {
        mask[i] = 1 << (31 - i);
    }
    //init();
}

FileSystem::~FileSystem() {
    delete buffer;
    delete ldisk;
    delete mask;
    delete descriptor;
    delete oft;
}

bool FileSystem::create(char *fName) {
    int descriptorN;
    int bitmapN;

    if (search_dir(fName) != oft[0].getLength()) {
        (*out) << "Error: File already exists" << std::endl;
        return false;
    }
    descriptorN = alloc_desc();
    if (descriptorN == -1) {
        (*out) << "Error: Descriptor is already taken" << std::endl;
        return false;
    }
    alloc_dir();
    for (int i = 0; i < DESCRIPTOR_SIZE; i++)
        write(0, &(fName[i]), 1);
    for (int i = 0; i < DESCRIPTOR_SIZE; i++)
        write(0, &(((char *) &descriptorN)[i]), 1);

    set_desc(descriptorN);
    return true;
}

bool FileSystem::destroy(char *fName) {
    int descriptorN;
    int bitmapN;

    lseek(0, 0);
    if (search_dir(fName) == oft[0].getLength()) {
        (*out) << "Error: File already exists" << std::endl;
        return false;
    }
    read(0, buffer, 8);
    descriptorN = (int) (((int *) buffer)[1]);
    descriptor = get_desc(descriptorN);

    for (int i = 1; i < DESCRIPTOR_SIZE; i++)
        if (oft[i].getDescriptor() == descriptorN) {
            close(i);
            break;
        }

    ldisk->read_block(0, buffer);
    for (int i = 1; i < 4; i++) {
        bitmapN = descriptor[i];
        if (bitmapN != -1)
            ((unsigned int *) buffer)[bitmapN / 32] = ((unsigned int *) buffer)[bitmapN / 32] & ~mask[bitmapN % 32];
        descriptor[i] = -1;
    }
    ldisk->write_block(0, buffer);
    descriptor[0] = -1;
    set_desc(descriptorN);
    search_dir(fName);
    write(0, NULL, 8);
    (*out) << "File " + std::string(fName) + " destroyed." << std::endl;
    return true;
}

int FileSystem::open(char *fName) {
    int descriptorN;
    lseek(0, 0);
    int position = search_dir(fName);

    if (position == oft[0].getLength()) {
        (*out) << "Error: File already exists" << std::endl;
        return -1;
    }
    read(0, buffer, 8);
    descriptorN = ((int *) buffer)[1];
    descriptor = get_desc(descriptorN);

    for (int i = 1; i < 4; i++) {
        if (oft[i].getDescriptor() == ((int *) buffer)[0]) {
            (*out) << "Error: File already opened" << std::endl;
            return -1;
        }
    }

    for (int i = 1; i < 4; i++) {
        if (oft[i].getDescriptor() == -1) {
            oft[i].init(descriptorN, descriptor[0]);
            return i;
        }
    }

    (*out) << "OFT full" << std::endl;
    return -1;
}

int FileSystem::close(int index) {
    if (index > 3 || index < 0) {
        (*out) << "Index out of bound" << std::endl;
        return -1;
    }

    int descriptorN, slot, report;
    if (oft[index].getDescriptor() != -1) {
        descriptorN = oft[index].getDescriptor();
        get_desc(descriptorN);

        ((int *) descriptor)[0] = oft[index].getDescriptor();
        oft[index].getLength();
        set_desc(descriptorN);

        slot = oft[index].getSlot();
        report = oft[index].getStatus();
        if (report == 1) {
            oft[index].init();
            return index;
        } else if (report > 1 || report < -1) {
            slot = slot - 1;
            ldisk->write_block(descriptor[slot], oft[index].getBuffer());
        } else {
            ldisk->write_block(descriptor[slot], oft[index].getBuffer());
        }
        oft[index].init();
        return index;
    } else {
        (*out) << "Nothing to close" << std::endl;
        return -1;
    }
}

string FileSystem::directory() {
    lseek(0, 0);
    int descriptorN = oft[0].getDescriptor();
    descriptor = get_desc(descriptorN);
    int length = oft[0].getLength();
    if (length == 0) { return ""; }
    int numberOfFile = (length / 4) / 2;
    string result = "";
    char *name = new char[5];
    char *skip = new char[5];
    skip[4] = '\0';

    for (int i = 0; i < numberOfFile; i++) {
        name[4] = '\0';
        read(0, name, 4);
        if (name[0] != '\0') {
            result.append(string(name));
            result.append(" ");
        }
        read(0, name, 4);
    }
    return result;
}

int FileSystem::search_dir(char *dName) {
    lseek(0, 0);
    char temp[8];
    while (oft[0].getCurrentPosition() < oft[0].getLength()) {
        read(0, temp, 8);
        if ((int) *((int *) temp) == (int) *((int *) dName)) {
            lseek(0, oft[0].getCurrentPosition() - 8);
            return oft[0].getCurrentPosition();
        }
    }

    return oft[0].getCurrentPosition();
}

int FileSystem::alloc_desc() {
    for (int i = 0; i < 6; i++) {
        ldisk->read_block(i + 1, buffer);
        for (int j = 0; j < 4; j++)
            if (((int *) buffer)[j * 4] == -1)
                return ((i) * 4) + j;
    }
    return -1;
}

int FileSystem::alloc_dir() {
    char temp[4] = {'\0', '\0', '\0', '\0'};

    lseek(0, 0);
    search_dir(temp);

    if (oft[0].getCurrentPosition() == 64 * 3)
        return -1;
    return oft[0].getCurrentPosition();
}

int *FileSystem::get_desc(int d) {
    ldisk->read_block(1 + (d / 4), buffer);
    for (int i = 0; i < 4; i++)
        descriptor[i] = ((int *) buffer)[i + (d % 4) * 4];
    return descriptor;
}

int FileSystem::set_desc(int d) {
    ldisk->read_block(1 + (d / 4), buffer);
    for (int i = 0; i < 4; i++)
        ((int *) buffer)[i + (d % 4) * 4] = descriptor[i];
    ldisk->write_block(1 + (d / 4), buffer);
    return 0;
}

int FileSystem::read(int index, char *mem_area, int count) {
    int i = 0;
    if (index > 3 || index < 0) {
        (*out) << "Read: Out of bound exception." << std::endl;
        return -1;
    }
    if (count < 0) {
        (*out) << "Read: Count cannot be negative." << std::endl;
        return -1;
    }
    int report = oft[index].getStatus();
    int descriptor_n = oft[index].getDescriptor();
    descriptor = get_desc(descriptor_n);
    while ((report <= 0) && count > 0 && (oft[index].getCurrentPosition() < oft[index].getLength())) {
        if (report < 0) {
            ldisk->read_block(descriptor[(-1) * report], oft[index].getBuffer());
        }
        report = oft[index].readChar(&mem_area[i]);
        i++;
        count--;
    }
    return count;
}

int FileSystem::write(int index, char *mem_area, int count) {
    if (index > 3 || index < 0) {
        (*out) << "Write: Out of bound exception." << std::endl;
        return -1;
    }
    if (count < 0) {
        (*out) << "Write: Cannot write negative amount." << std::endl;
        return -1;
    }
    if (oft[index].getDescriptor() == -1) {
        (*out) << "Write: Cannot open the file." << std::endl;
        return -1;
    }

    int report = oft[index].getStatus();
    int descriptor_n = oft[index].getDescriptor();
    descriptor = get_desc(descriptor_n);
    int bitmap_n = descriptor[oft[index].getSlot()];

    while (report != 4 && count > 0) {
        if (report < 0) {
            if (report < 1) {
                ldisk->write_block(descriptor[(-1) * report - 1], oft[index].getBuffer());
            }
            ldisk->read_block(descriptor[(-1) * report], oft[index].getBuffer());
        } else if (report > 0) {
            if (report > 1 && report != 5) {
                ldisk->write_block(descriptor[report - 1], oft[index].getBuffer());
            }
            bitmap_n = alloc_bitmap();
            if (bitmap_n == -1) {
                return count;
            }
            ldisk->read_block(0, buffer);
            ((unsigned int *) buffer)[bitmap_n / 32] =
                    ((unsigned int *) buffer)[bitmap_n / 32] | mask[bitmap_n % 32];
            ldisk->write_block(0, buffer);
            descriptor[report] = bitmap_n;
            descriptor[0] = oft[index].getCurrentPosition();
            set_desc(descriptor_n);
            oft[index].flush();
        }
        report = oft[index].writeChar(mem_area);
        count--;
    }
    return count;
}

int FileSystem::alloc_bitmap() {
    int tmp;
    ldisk->read_block(0, buffer);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 32; j++) {
            tmp = ((unsigned int *) buffer)[i] & mask[j];
            if (tmp == 0) {
                return i * 32 + j;
            }
        }
    }
    return -1;
}

int FileSystem::lseek(int index, int pos) {
    if (index > 3 || index < 0) {
        (*out) << "Lseek: Index out of bound." << std::endl;
        return -1;
    }
    if (oft[index].getDescriptor() == -1) {
        (*out) << "Lseek: File not opened." << std::endl;
        return -1;
    }
    if (oft[index].getLength() + 1 < pos || pos < 0 || pos == 64 * 3) {
        (*out) << "Lseek: Out of index." << std::endl;
        return -1;
    }

    int descriptor_n = oft[index].getDescriptor();
    get_desc(descriptor_n);

    int slot = oft[index].getDescriptor();
    int report = oft[index].getStatus();

    if (report == 0) {
        ldisk->write_block(descriptor[slot], oft[index].getBuffer());
    } else {
        if (report != 1 && report != -1) {
            ldisk->write_block(descriptor[slot - 1], oft[index].getBuffer());
        }
    }
    oft[index].seek(pos);
    report = oft[index].getStatus();
    slot = oft[index].getSlot();
    if (report <= 0) {
        ldisk->read_block(descriptor[slot], oft[index].getBuffer());
    }
    return pos;
}

std::string FileSystem::init() {
    ((unsigned int *) buffer)[0] = mask[0];
    ((unsigned int *) buffer)[0] =
            ((unsigned int *) buffer)[0] | mask[1] | mask[2] | mask[3] | mask[4] | mask[5] | mask[6];
    ((unsigned int *) buffer)[1] = mask[1] & mask[2];
    ldisk->write_block(0, buffer);

    descriptor[0] = 0;
    for (int i = 1; i < DESCRIPTOR_SIZE; i++) {
        descriptor[i] = -1;
    }
    set_desc(0);

    for (int i = 0; i < DESCRIPTOR_SIZE; i++) {
        descriptor[i] = -1;
    }
    for (int i = 1; i < NUMBER_OF_DESCRIPTORS; i++) {
        set_desc(i);
    }
    for (int i = 0; i < DESCRIPTOR_SIZE; i++) {
        oft[i].init();
    }

    oft[0].init(0, 0);
    ldisk->read_block(7, oft[0].getBuffer());

    return "Disk initialized";
}

std::string FileSystem::init(const std::string &fName) {
    char dummy[1];
    std::ifstream ifile(fName, std::ifstream::in);
    try {
        if (ifile) {
            for (int i = 0; i < BLOCKS; i++) {
                for (int j = 0; j < BLOCK_SIZE; j++) {
                    ifile.read(buffer + j, 1);
                    if (j % 4 == 3) {
                        ifile.read(dummy, 1);
                    }
                }
                ifile.read(dummy, 1);
                ldisk->write_block(i, buffer);
            }

            ifile.close();

            for (int i = 0; i < MAX_FILE_NAME_LENGTH; i++) {
                oft[i].init();
            }

            descriptor = get_desc(0);
            oft[0].init(0, descriptor[0]);
            //ldisk->read_block(descriptor[0], oft[0].getBuffer());

            return "Disk restored";
        }
    } catch (...) {

    }

    return init();
}

bool FileSystem::save(const std::string &fName) {
    for (int i = 3; i >= 0; i--) {
        if (oft[i].getDescriptor() != -1) {
            close(i);
        }
    }

    std::ofstream ofile(fName, std::ofstream::out);
    if (ofile) {
        for (int i = 0; i < BLOCKS; i++) {
            ldisk->read_block(i, buffer);
            for (int j = 0; j < FILE_LENGTH; j++) {
                ofile << buffer[j];
                if (j % 4 == 3) {
                    ofile << " ";
                }
            }
            ofile << std::endl;
        }
        ofile.close();
        init();
        return true;
    }
    return false;
}
