#pragma once
#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <vector>

class FileHandler {
public:
    // Constructor: Open the file asynchronously
    FileHandler(const std::string& m_fileName, std::ios::openmode mode);
    // Destructor: Ensure the file is closed if not already
    ~FileHandler();

    // Asynchronous file read
    void readFileAsync(size_t size);
    // Asynchronous file write
    void writeFileAsync(const char* bytes, size_t size);
    // Wait for the file to be opened and read
    void waitForFileHandling();

    char* getBuffer() { return m_buffer; }

private:
    std::string m_fileName;
    std::unique_ptr<std::fstream> m_file;
    std::future<void> m_asyncOpenFile;
    std::future<void> m_asyncReadFile;
    std::future<void> m_asyncWriteFile;
    char* m_buffer; //null terminated

    // Open the file in read mode
    void openFile(std::ios::openmode mode);
    // Read `size` bytes from the file
    void readFile(size_t size);
    void writeFile(const char* bytes, size_t size);
};