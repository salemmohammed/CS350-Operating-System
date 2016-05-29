#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

class Block {
    public:
        Block();
        Block(Block& b);
        ~Block();

        void overwrite(char character, unsigned start, unsigned size);
        void overwrite(void* characters, unsigned start, unsigned size);
        std::string getStringOfLength(unsigned lengthBytes);
        std::string getFormattedBytesOfLength(unsigned lengthBytes);
        unsigned readUnsignedAtIndex(unsigned index);

        Block& operator=(const Block& other);
        friend std::istream& operator>>(std::istream& input, Block& block);
        friend std::ostream& operator<<(std::ostream& output, const Block& block);

        static const unsigned blockSize = 1024;

    public:
        char* data;
};

#endif
