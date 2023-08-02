#include "findPathLib.h"

// Method that searches for a file in a directory
void findPathLib::processDirectory(const std::string &dirPath, const std::string &wantedFile) {
    try {
        // Info about each process of thread
        // std::cout << "Thread " << std::this_thread::get_id() << " is processing directory: " << dirPath << std::endl;

        // Go through all the files in the directory
        for (const auto &entry: std::filesystem::directory_iterator(dirPath)) {
            // If the file was found break the loop
            if (this->found) {
                break;
            }
            // If the entry is directory go to it
            if (entry.is_directory()) {
                // Create a lock. This ensures that this thread will perform the operation from the queue
                std::unique_lock<std::mutex> lock(queueMutex);

                // Add path to queue
                directoriesQueue.push_back(entry.path().string());

                // Remove the lock so that all threads can access the queue
                // and send a message to the thread that is waiting for work
                lock.unlock();
                queueCV.notify_one();

                // If this file is the file we are want, we print it to the console
            } else if (entry.is_regular_file() && entry.path().filename() == wantedFile) {
                std::cout << "Thread " << std::this_thread::get_id() << " found the file: " << entry.path()
                          << std::endl;
                this->found = true;
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        // Handle any filesystem-related exceptions if needed.
        //std::cout << e.what() << std::endl;
    }
}

// A method that checks if the directory is empty
// and adds the directory to the directoriesQueue vector if it is not empty
void findPathLib::findFile(const std::string &wantedFile) {
    while (!this->found) {
        std::string currentDir; // Variable to hold the current directory to be processed
        {
            // Unique lock which is used to provide secure access to data
            std::unique_lock<std::mutex> lock(queueMutex);
            // Check is dir empty?
            if (directoriesQueue.empty()) {
                if (activeThreads == 0) {
                    // We searched all directories -> break loop.
                    break;
                }
                // If the queue is empty but there are active threads, wait for a notification.
                // The thread will be put to sleep (blocking) until a notification is received.
                queueCV.wait(lock);
            } else {
                // Retrieve the last directory path from the queue and remove it.
                currentDir = directoriesQueue.back();
                directoriesQueue.pop_back();
                activeThreads++; // Increment the active threads counter for tracking.
            }
        }

        if (!currentDir.empty()) {
            processDirectory(currentDir, wantedFile);
        }
        // Reduce the number of active threads by 1, thus showing that our thread has finished working
        activeThreads--;
    }
}

// run method
void findPathLib::run() {
    // Set limit of threads
    const unsigned int maxThreads = std::min(std::thread::hardware_concurrency(), 8u);

    // Create thread pool
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < maxThreads; ++i) {
        threads.emplace_back(&findPathLib::findFile, this, wantedFile);
    }

    // Wait until all thread finish
    for (std::thread &t: threads) {
        t.join();
    }
}

// Constructor
findPathLib::findPathLib(const std::string &path, const std::string &wantedFile) {
    this->found = false;
    this->activeThreads = 0;
    this->directoriesQueue.clear();
    this->directoriesQueue.push_back(path);
    this->wantedFile = wantedFile;
}
