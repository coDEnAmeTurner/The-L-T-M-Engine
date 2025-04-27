#include "FileHandler.h"

FileHandler::FileHandler(const std::string& m_fileName)
    : m_fileName(m_fileName), m_file(nullptr) 
{
    m_asyncOpenFile = std::async(std::launch::async, &FileHandler::openFile, this);
}

// Destructor: Ensure the file is closed if not already
FileHandler::~FileHandler() {
    if (m_file && m_file->is_open()) {
        m_file->close();
    }
}

// Asynchronous file read
void FileHandler::readFileAsync(size_t size) {
    if (!m_file || !m_file->is_open()) {
        std::cerr << "File is not open yet!" << std::endl;
        return;
    }

    // Launch async task to read from the file
    m_asyncReadFile = std::async(std::launch::async, &FileHandler::readFile, this, size);
}

// Wait for the file to be opened and read
void FileHandler::waitForOpenAndRead() {
    if (m_asyncOpenFile.valid()) {
        m_asyncOpenFile.get();  // Wait for the file to open
    }
    if (m_asyncReadFile.valid()) {
        m_asyncReadFile.get();  // Wait for the file read to complete
    }
}

// Open the file in read mode
void FileHandler::openFile() {
    m_file = std::make_unique<std::ifstream>(m_fileName, std::ios::in);
    if (!m_file->is_open()) {
        std::cerr << "Failed to open m_file: " << m_fileName << std::endl;
    }
    else {
        std::cout << "m_file opened successfully!" << std::endl;
    }
}

// Read `size` bytes from the file
void FileHandler::readFile(size_t size) {
    //need to allocate from heap --> pending --> need heap allocator, since the file can be large
    std::vector<char> buffer(size);
    if (m_file->read(buffer.data(), size)) {
        std::cout << "Read " << m_file->gcount() << " bytes from m_file" << std::endl;

    }
    else {
        std::cerr << "Failed to read from the m_file" << std::endl;
    }
}