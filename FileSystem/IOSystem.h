//
// Created by My Surface on 3/1/2020.
//

#ifndef FILESYSTEM_IOSYSTEM_H
#define FILESYSTEM_IOSYSTEM_H


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
