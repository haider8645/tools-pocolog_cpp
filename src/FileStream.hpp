#ifndef FILESTREAM_H
#define FILESTREAM_H

#include <fstream>
#include <vector>

namespace pocolog_cpp
{
    
/**
 * Reimplementation of the std::fstream class.
 * 
 * This implementation is buffered, meanding that the class
 * will keep a read and a writebuffer, and only perform disc
 * accesses if neccessary. 
 *
 * This reimplementation performs way better in the usecase,
 * that seekp/seekg is called often. The std::fstream implementation
 * discards its buffer every time seek is called, resulting in a
 * horrible runtime performance in our usecase.
 * */
class FileStream
{
private:
    std::vector<char> readBuffer;
    int fd;
    off_t readBufferPosition;
    off_t readBufferEndPosition;
    off_t readPos;
    off_t writePos;
    off_t fileSize;
    off_t blockSize;
    
    bool posInBuffer(off_t pos) const
    {
        return pos >= readBufferPosition && pos < readBufferEndPosition;
    }
    
    bool reloadBuffer(off_t position);
    bool goodFlag;
    std::string fileName;
    
public:
    FileStream();
    
    FileStream(const char* __s,
           std::ios_base::openmode mode);
    ~FileStream();
    
    bool open(const char* fileName, std::ios_base::openmode mode);
    
    void read(char* buffer, size_t size);
    
    std::streampos tellg();
    std::streampos tellp();
    
    std::streampos seekg(std::streampos pos);
    std::streampos seekp(std::streampos pos);
    
    bool good() const;
    bool eof() const
    {
        return readPos >= fileSize;
    }
    
    const std::string getFileName() const
    {
        return fileName;
    }

    bool fail() const;
    off_t size() const;
    void close();
    
};
}
#endif // FILESTREAM_H
