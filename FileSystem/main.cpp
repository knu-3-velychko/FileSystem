#include "Shell.h"
#include <iostream>
#include <fstream>

int main() {
    std::ifstream ifs;
    ifs.open(R"(C:\Users\My Surface\Documents\Projects\FileSystem\FileSystem\input.txt)", std::ios::in);

    ifs.close();
    return 0;
}
