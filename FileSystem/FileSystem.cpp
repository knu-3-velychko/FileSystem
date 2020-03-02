#include "FileSystem.h"

const int BLOCKS = 64;
const int BLOCK_SIZE = 64;      // 64 bytes = 16 integers
const int BITMAP_SIZE = 64;     // 64 bits = 2 integers
const int DESCRIPTOR_SIZE = 4;  // 4 integers
const int FILE_LENGTH = 1;      // 1 int

const int NUMBER_OF_DESCRIPTORS = 24;   // 6 blocks
const int MAX_FILE_NAME_LENGTH = 4; // 4 chars
const int DIRECTORY_ENTRY = 2;  // 2 integers
const int DESCRIPTOR_INDEX = 1; // 1 integer

using namespace std;

void FileSystem::create(char *fName) {
	int descriptorN;
	int bitmapN;

	if (search_dir(fName) != oft[0].getLength())
		cout << "File already exisssts" << endl;
//    	throw std::exception("File already exists");

	descriptorN = alloc_desc();
	if (descriptorN == -1)
		cout << "Descriptor is already taken" << endl;
//    	throw std::exception("Descriptor is already taken");

	alloc_dir();
	for (int i = 0; i < DESCRIPTOR_SIZE; i++)
		write(0, &(fName[i]), 1);
	for (int i = 0; i < DESCRIPTOR_SIZE; i++)
		write(0, &(((char *) &descriptorN)[i]), 1);

	descriptor[0] = 0;
	descriptor[1] = -1;
	descriptor[2] = -1;
	descriptor[3] = -1;

	set_desc(descriptorN);
}

bool FileSystem::destroy(char *fName) {
	int descriptorN;
	int bitmapN;

	lseek(0, 0);
	if (search_dir(fName) == oft[0].getLength())
		cout << "File already exisssts" << endl;
//    	throw std::exception("File already exists");

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
	// THIS MAY CAUSE BUG. THERE WAS &null in original code instead of NULL
	write(0, NULL, 8);
	return true;
}

int FileSystem::open(char *fName) {
	int descriptorN;
	lseek(0, 0);
	int position = search_dir(fName);

	if (search_dir(fName) == oft[0].getLength())
		cout << "File already exisssts" << endl;
//    	throw std::exception("File already exists");
	read(0, buffer, 8);
	descriptorN = ((int *) buffer)[1];
	descriptor = get_desc(descriptorN);

	for (int i = 1; i < 4; i++) {
		if (oft[i].getDescriptor() == ((int *) buffer)[0])
			cout << "File already opened" << endl;
//    	throw std::exception("File already opened");
	}

	for (int i = 1; i < 4; i++) {
		if (oft[i].getDescriptor() == -1) {
			oft[i].init(descriptorN, descriptor[0]);
			return i;
		}
	}

	cout << "OFT full" << endl;
//	throw exception("OFT full");
	return -1;
}

int FileSystem::close(int index) {
	if (index > 3 || index < 0)
		cout << "Index out of bound" << endl;
//	throw exception("Index out of bound");

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
		cout << "Nothing to close" << endl;
//	throw exception("Nothing to close");
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
