#include <iostream>
#include "findPathLib.h"

int main() {
    // Send message in console
    std::cout << "Enter start directory path: " << std::endl;

    //Get path by keyboard input in console
    std::string path{};
    std::cin >> path;

    // Send message in console
    std::cout << "Enter wanted file name: " << std::endl;

    // Get wanted file name by keyboard input in console
    std::string wantedFile{};
    std::cin >> wantedFile;

    // Create an object
    findPathLib finder(path, wantedFile);
    // Call the method run
    finder.run();

    return 0;
}
