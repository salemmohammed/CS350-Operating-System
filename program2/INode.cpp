#include "INode.h"

INode::INode(std::string name) 
    : Block(),
    fileName{name},
    fileSize{0},
    blockAddresses(blockSize/4),
    fileSizeIdx{0} {
    	// add file name including null character
        const char* n = name.c_str();
        for (unsigned i = 0; i <= sizeof(n) && i < blockSize - maxFileBlocks && n[i] != '\0'; i++) {
            data[i] = n[i];
	        currentIdx = i + 1;
        }
        data[currentIdx++] = '\0'; // add null terminator
        fileSizeIdx = currentIdx; // location of file size (right after file name)
        currentIdx += sizeof(fileSize); // move data write head to next free byte 
        writeFileSize();
}

INode::INode(Block& b)
    : Block(b),
    fileName(data),
    fileSize{0},
    blockAddresses(blockSize/4),
    fileSizeIdx{0} {
        readFileSize();
        currentIdx = fileSizeIdx + sizeof(fileSizeIdx) + (fileSize * sizeof(unsigned));
        for (unsigned i = 0; i < fileSize; i++) {
            unsigned index = fileSizeIdx + sizeof(fileSizeIdx) + (i * sizeof(unsigned));
            unsigned address = 0;
            memcpy(&address, &(data[index]), sizeof(address));
            blockAddresses[i] = address;
        }
}

unsigned INode::addBlockAddress(unsigned address) {
    unsigned addressIdx = currentIdx;
    if (currentIdx < fileSizeIdx + sizeof(fileSizeIdx) + maxFileBlocks) {
        for (unsigned i = 0; i < sizeof(address); i++) {
            data[currentIdx + i] = ((char*)&address)[i];
        }
        currentIdx += sizeof(address);
        fileSize++;
        writeFileSize();
        blockAddresses[addressIdx/4] = address;
    }
    return addressIdx;
}

void INode::updateBlockAddressAtIndex(unsigned address, unsigned index) {
    index = fileSizeIdx + sizeof(fileSize) + (index*sizeof(unsigned));
    memcpy(&(data[index]), &address, sizeof(address));
    blockAddresses[index/4] = address;
}

void INode::writeFileSize() {
    memcpy(&(data[fileSizeIdx]), &fileSize, sizeof(fileSize));
}

void INode::readFileSize() {
	fileSizeIdx = fileName.size() + 1;
	memcpy(&fileSize, &(data[fileSizeIdx]), sizeof(fileSize));
}
