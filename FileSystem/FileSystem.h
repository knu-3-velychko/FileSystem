//
// Created by My Surface on 3/1/2020.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include "IOSystem.h"
#include <iostream>

class FileSystem {
public:
    FileSystem();

    ~FileSystem();

    void create(char *fName);

    bool destroy(char *fName);

    int open(char *fName);

    int close(int index);

    int read(int index, char *mem_area, int count);

    int write(int index, char *mem_area, int count);

    int lseek(int index, int pos);

    std::string directory();

    std::string init();

    std::string init(std::string fName);

    std::string save(std::string fName);



    //helper functions

    void show_bitmap();

    int *get_desc(int d);

    int set_desc(int d);

    void show_desc();


    int alloc_bitmap();

    int alloc_desc();

    int alloc_dir();

    int seek_block(int index, int slot);


    int search_dir(char *dName);

private:
    char *buffer;
    IOSystem *ldisk;
    unsigned int *mask;
    int *descriptor;
};


#endif //FILESYSTEM_FILESYSTEM_H
