//
// Created by My Surface on 3/1/2020.
//

#ifndef FILESYSTEM_SHELL_H
#define FILESYSTEM_SHELL_H

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <iterator>

class Shell {
public:
    Shell(std::istream *in, std::ostream *out);

    ~Shell();

    void read();

private:
    std::istream *in;
    std::ostream *out;

    bool recognizeCommand(std::vector<std::string> &command);

    void create(std::vector<std::string> &command);

    void destroy(std::vector<std::string> &command);

    void open(std::vector<std::string> &command);

    void close(std::vector<std::string> &command);

    void read(std::vector<std::string> &command);

    void write(std::vector<std::string> &command);

    void seek(std::vector<std::string> &command);

    void directory(std::vector<std::string> &command);

    void createDisk(std::vector<std::string> &command);

    void save(std::vector<std::string> &command);

    void error(const std::string& message);
};


#endif //FILESYSTEM_SHELL_H
