#ifndef FINDPATH_FINDPATHLIB_H
#define FINDPATH_FINDPATHLIB_H

#include <iostream>
#include <filesystem>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>

class findPathLib {
private:
    std::string path;
    std::string wantedFile;

    // Atomic flag to indicate if wanted file is found
    std::atomic<bool> found{false};

    // Atomic flag to known how many threads is active
    std::atomic<int> activeThreads{0};

    // Mutex and cv for thread sync
    std::mutex queueMutex;
    std::condition_variable queueCV;

    // Vector to hold the dir to be processed
    std::vector<std::string> directoriesQueue;

    // Method that searches for a file in a directory
    void processDirectory(const std::string &dirPath, const std::string &wantedFile);

    // A method that checks if the directory is empty
    // and adds the directory to the directoriesQueue vector if it is not empty
    void findFile(const std::string &wantedFile);

public:
    //constructor
    findPathLib(const std::string &path, const std::string &wantedFile);

    // run method
    void run();
};

#endif //FINDPATH_FINDPATHLIB_H
