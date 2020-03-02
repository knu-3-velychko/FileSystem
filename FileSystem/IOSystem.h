#ifndef FILESYSTEM_IOSYSTEM_H
#define FILESYSTEM_IOSYSTEM_H

#include <iostream>
#include <exception>

class IOSystem {
public:
    IOSystem();

    ~IOSystem();

    void read_block(int i, char *p);

    void write_block(int i, char *p);

private:
    char **ldisk;
};


#endif //FILESYSTEM_IOSYSTEM_H
