#include "Shell.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    std::ifstream ifs;
    std::ofstream ofs;
    std::string path = R"(C:\Users\My Surface\Documents\Projects\FileSystem\FileSystem\)";

    ifs.open(path + "input.txt", std::ios::in);
    ofs.open(path + "output.txt", std::ios::out);

    auto *shell = new Shell(&ifs, &ofs);
    shell->read();

    ifs.close();
    ofs.close();

    delete shell;
    
    return 0;
}
