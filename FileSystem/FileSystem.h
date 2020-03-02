#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include "IOSystem.h"
#include "OFT.h"
#include <exception>
#include <io.h>
#include <string>
#include <sstream>
#include <iostream>

class FileSystem {
public:
    FileSystem(std::ostream *out);

    ~FileSystem();

    bool create(char *fName);

    bool destroy(char *fName);

    int open(char *fName);

    int close(int index);

    int read(int index, char *mem_area, int count);

    int write(int index, char *mem_area, int count);

    int lseek(int index, int pos);

    std::string directory();

    std::string init();

    std::string init(const std::string &fName);

    bool save(const std::string& fName);


    //helper functions
    int *get_desc(int d);

    int set_desc(int d);

    int alloc_bitmap();

    int alloc_desc();

    int alloc_dir();


    int search_dir(char *dName);

private:
    std::ostream *out;

    char *buffer;
    IOSystem *ldisk;
    unsigned int *mask;
    int *descriptor;
    OFT *oft;
};


#endif //FILESYSTEM_FILESYSTEM_H
