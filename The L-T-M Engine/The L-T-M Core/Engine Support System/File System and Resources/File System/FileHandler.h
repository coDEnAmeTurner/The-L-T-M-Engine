#pragma once
#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <vector>

class FileHandler {
public:
    // Constructor: Open the file asynchronously
    FileHandler(const std::string& m_fileName);
    // Destructor: Ensure the file is closed if not already
    ~FileHandler();

    // Asynchronous file read
    void readFileAsync(size_t size);
    // Wait for the file to be opened and read
    void waitForOpenAndRead();

private:
    std::string m_fileName;
    std::unique_ptr<std::ifstream> m_file;
    std::future<void> m_asyncOpenFile;
    std::future<void> m_asyncReadFile;

    // Open the file in read mode
    void openFile();
    // Read `size` bytes from the file
    void readFile(size_t size);
};