#include "FileHandler.h"

FileHandler::FileHandler(const std::string& m_fileName, std::ios::openmode mode)
    : m_fileName(m_fileName), m_file(nullptr), m_buffer(nullptr)
{
    m_asyncOpenFile = std::async(std::launch::async, &FileHandler::openFile, this, mode);
}

// Destructor: Ensure the file is closed if not already
FileHandler::~FileHandler() {
    if (m_file && m_file->is_open()) {
        m_file->close();
    }

    delete[] m_buffer;
    m_buffer = nullptr;
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

// Asynchronous file read
void FileHandler::writeFileAsync(const char* bytes, size_t size) {
    if (!m_file || !m_file->is_open()) {
        std::cerr << "File is not open yet!" << std::endl;
        return;
    }

    // Launch async task to read from the file
    m_asyncWriteFile = std::async(std::launch::async, &FileHandler::writeFile, this, bytes, size);
}

// Wait for the file to be opened and read
void FileHandler::waitForFileHandling() {
    if (m_asyncOpenFile.valid()) {
        m_asyncOpenFile.get();  // Wait for the file to open
    }
    if (m_asyncReadFile.valid()) {
        m_asyncReadFile.get();  // Wait for the file read to complete
    }
    if (m_asyncWriteFile.valid()) {
        m_asyncWriteFile.get();  // Wait for the file read to complete
    }
}

// Open the file in either mode
void FileHandler::openFile(std::ios::openmode mode) {
    m_file = std::make_unique<std::fstream>(m_fileName, mode);
    if (!m_file->is_open()) {
        std::cerr << "Failed to open m_file: " << m_fileName << std::endl;
    }
    else {
        std::cout << "m_file opened successfully!" << std::endl;
    }
}

// Read `size` bytes from the file
void FileHandler::readFile(size_t size) {
    m_buffer = new char[size + 1];
    if (m_file->read(m_buffer, size)) {
        std::cout << "Read " << m_file->gcount() << " bytes from m_file" << std::endl;
        *(m_buffer + size) = '\0';
    }
    else {
        std::cerr << "Failed to read from the m_file" << std::endl;
    }
}

void FileHandler::writeFile(const char* bytes, size_t size) {
    if (!m_file->is_open()) {
        throw std::runtime_error("File not open for writing.");
    }

    m_file->write(bytes, size);
    if (!m_file) {
        throw std::runtime_error("Failed to write to file.");
    }
}