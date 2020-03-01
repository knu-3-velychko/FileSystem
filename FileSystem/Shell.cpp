//
// Created by My Surface on 3/1/2020.
//

#include "Shell.h"

Shell::Shell(std::istream *in, std::ostream *out) {
    this->in = in;
    this->out = out;
}

Shell::~Shell() {
    delete in;
    delete out;
}

void Shell::read() {
    std::string line;
    while ((*in) >> line) {
        std::istringstream iss(line);
        std::vector<std::string> command((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());

        if (recognizeCommand(command)) {
            break;
        }
    }
}


bool Shell::recognizeCommand(std::vector<std::string> &command) {
    auto command_name = command[0];
    if (command_name == "cd") {
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
        createDisk(command);
    } else if (command_name == "sv") {
        save(command);
    } else if (command_name == "quit") {
        return false;
    } else {

    }
    return false;
}

void Shell::create(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    (*out) << "File " << command[1] << " created." << std::endl;
}

void Shell::destroy(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    (*out) << "File " << command[1] << " destroyed." << std::endl;
}

void Shell::open(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    (*out) << "File " << command[1] << " opened." << std::endl;
}

void Shell::close(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    (*out) << "File " << command[1] << " closed." << std::endl;
}

void Shell::read(std::vector<std::string> &command) {
    if (command.size() != 3) {
        error("Wrong command signature");
        return;
    }

    (*out) << command[2] << "bytes read: " << std::endl;//+<read bytes>
}

void Shell::write(std::vector<std::string> &command) {
    if (command.size() != 4) {
        error("Wrong command signature");
        return;
    }

    (*out) << command[2] << "bytes written. " << std::endl;
}

void Shell::seek(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    (*out) << "Current position is" << command[1] << std::endl;
}

void Shell::directory(std::vector<std::string> &command) {
    if (command.size() != 1) {
        error("Wrong command signature");
        return;
    }

    (*out) << "Files in directory" << std::endl;
}

void Shell::createDisk(std::vector<std::string> &command) {
    if (command.size() != 6) {
        error("Wrong command signature");
        return;
    }

    (*out) << "Disk restored." << std::endl;
}

void Shell::save(std::vector<std::string> &command) {
    if (command.size() != 2) {
        error("Wrong command signature");
        return;
    }

    (*out) << "Disk saved." << std::endl;
}

void Shell::error(const std::string &message) {
    (*out) << "Error: " << message << std::endl;
}
