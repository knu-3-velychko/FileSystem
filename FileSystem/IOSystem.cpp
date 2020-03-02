#include "IOSystem.h"

IOSystem::IOSystem() {
    ldisk = new char *[64];
    for (int i = 0; i < 64; i++) {
        ldisk[i] = new char[64];
    }
}

IOSystem::~IOSystem() {
    for (int i = 0; i < 64; i++) {
        delete ldisk[i];
    }
    delete ldisk;
}

void IOSystem::read_block(int i, char *p) {
    try {
        for (int k = 0; k < 64; k++) {
            *(p + k) = ldisk[i][k];
        }
    } catch (std::exception &e) {
        std::cout << "Error: Read block error. " << e.what() << std::endl;
    }
}

void IOSystem::write_block(int i, char *p) {
    try {
        for (int k = 0; k < 64; k++) {
            ldisk[i][k] = *(p + k);
        }
    } catch (std::exception &e) {
        std::cout << "Error: Write block error. " << e.what() << std::endl;
    }
}
