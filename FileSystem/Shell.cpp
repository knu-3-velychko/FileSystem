#include "Shell.h"

Shell::Shell(std::istream *in, std::ostream *out) {
    this->in = in;
    this->out = out;
    this->fileSystem = new FileSystem(out);
}

Shell::~Shell() {
    delete in;
    delete out;
}

void Shell::read() {
    std::string line;
    while (std::getline((*in), line)) {
        std::cout << line << std::endl;
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> command;
        while (std::getline(iss, token, ' ')) {
            command.push_back(token);
        }
        if (!recognizeCommand(command)) {
            break;
        }
    }
}


bool Shell::recognizeCommand(std::vector<std::string> &command) {
    auto command_name = command[0];
    if (command_name == "cr") {
        create(command);
    } else if (command_name == "de") {
        destroy(command);
    } else if (command_name == "op") {
        open(command);
    } else if (command_name == "cl") {
        close(command);
    } else if (command_name == "rd") {
        read(command);
    } else if (command_name == "wr") {
        write(command);
    } else if (command_name == "sk") {
        seek(command);
    } else if (command_name == "dr") {
        directory(command);
    } else if (command_name == "in") {
        init(command);
    } else if (command_name == "sv") {
        save(command);
    } else if (command_name == "quit") {
        return false;
    } else {
        (*out) << "Error: Unknown command" << std::endl;
    }
    return true;
}

void Shell::create(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    if (fileSystem->create(strcpy(new char[command[1].length() + 1], command[1].c_str()))) {
        (*out) << "File " << command[1] << " created." << std::endl;
    }
}

void Shell::destroy(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    if (fileSystem->destroy(strcpy(new char[command[1].length() + 1], command[1].c_str()))) {
        (*out) << "File " << command[1] << " destroyed." << std::endl;
    }
}

void Shell::open(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }
    if (fileSystem->open(strcpy(new char[command[1].length() + 1], command[1].c_str()))) {
        (*out) << "File " << command[1] << " opened." << std::endl;
    }
}

void Shell::close(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    int index;
    std::istringstream(command[1]) >> index;
    if (fileSystem->close(index))
        (*out) << "File " << command[1] << " closed." << std::endl;
}

void Shell::read(std::vector<std::string> &command) {
    if (command.size() != 4) {
        error("Wrong command signature");
        return;
    }


    int index;
    char *mem_area;
    int count;
    std::istringstream(command[1]) >> index;
    mem_area = strcpy(new char[command[2].length() + 1], command[2].c_str());
    std::istringstream(command[3]) >> count;

    int bytes = fileSystem->read(index, mem_area, count);
    if (bytes != -1)
        //FIXME:
        (*out) << command[2] << "bytes read: " << bytes << std::endl;//+<read bytes>
}

void Shell::write(std::vector<std::string> &command) {
    if (command.size() != 4) {
        error("Wrong command signature");
        return;
    }

    int index;
    char *mem_area;
    int count;
    std::istringstream(command[1]) >> index;
    mem_area = strcpy(new char[command[2].length() + 1], command[2].c_str());
    std::istringstream(command[3]) >> count;

    int bytes = fileSystem->read(index, mem_area, count);
    if (bytes != -1)
        (*out) << bytes << "bytes written. " << std::endl;
}

void Shell::seek(std::vector<std::string> &command) {
    if (command.size() != 3) {
        error("Wrong command signature");
        return;
    }

    int index;
    int pos;
    std::istringstream(command[1]) >> index;
    std::istringstream(command[2]) >> pos;

    int position = fileSystem->lseek(index, pos);
    if (position != -1)
        (*out) << "Current position is" << position << std::endl;
}

void Shell::directory(std::vector<std::string> &command) {
    if (command.size() != 1) {
        error("Wrong command signature");
        return;
    }

    (*out) << "Files in directory" << fileSystem->directory() << std::endl;
}

void Shell::init(std::vector<std::string> &command) {
    if (command.size() != 6) {
        error("Wrong command signature");
        return;
    }

    std::cout << fileSystem->init(command[5]) << std::endl;
    std::cout << "here" << std::endl;
    (*out) << fileSystem->init(command[5]) << std::endl;
}

void Shell::save(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }
    if (fileSystem->save(strcpy(new char[command[1].length() + 1], command[1].c_str()))) {
        (*out) << "Disk saved." << std::endl;
    } else {
        (*out) << "Saving process failed." << std::endl;
    }
}

void Shell::error(const std::string &message) {
    (*out) << "Error: " << message << std::endl;
}
