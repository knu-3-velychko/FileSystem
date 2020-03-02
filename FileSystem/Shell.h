#ifndef FILESYSTEM_SHELL_H
#define FILESYSTEM_SHELL_H

#include <iostream>

class Shell {
public:
    Shell();

    ~Shell();

    void read();

private:
    bool recognizeCommand(std::string *command);

    void create(std::string *command);

    void destroy(std::string *command);

    void open(std::string *command);

    void close(std::string *command);

    void read(std::string *command);

    void write(std::string *command);

    void seek(std::string *command);

    void directory(std::string *command);

    void createDisk(std::string *command);

    void save(std::string *command);

    void error(std::string message);
};


#endif //FILESYSTEM_SHELL_H
