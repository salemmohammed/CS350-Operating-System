#include "Segment.h"

Segment::Segment(std::string fileName)
        : maxBlocks{1024},
        summaryBlockCount{8},
        currentBlockIdx{0},
        segmentFileName{fileName},
        file{segmentFileName, std::ios::binary | std::ios::in | std::ios::out},
        blocks{maxBlocks} {
    Block block;
    block.overwrite(std::numeric_limits<unsigned char>::max(), 0, 1024);
    for (unsigned i = 0; i < summaryBlockCount; i++) {
        blocks[i] = block;
    }
    if (!file) {
        file.close();
        file.open(segmentFileName,
            std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc); 
    } else {
        while (file && currentBlockIdx < maxBlocks) {
            file >> blocks[currentBlockIdx++];
        }
    }
    // find next open block according to summary blocks
    currentBlockIdx = 8;
    bool stop = false;
    for (unsigned i = 0; i < summaryBlockCount && !stop; i++) {
        for (unsigned j = 0; j < Block::blockSize; j += sizeof(unsigned) * 2) {
            unsigned blockStatus = blocks[i].readUnsignedAtIndex(j);
            unsigned iNodeStatus = blocks[i].readUnsignedAtIndex(j + sizeof(unsigned));
            if (blockStatus == std::numeric_limits<unsigned>::max()
                && iNodeStatus == std::numeric_limits<unsigned>::max()) {
                // found empty block
                stop = true;
                break;
            }
            currentBlockIdx++;
        }
    }
    file.clear();
    file.seekg(0, std::ios::beg);
}

Segment::~Segment() {
    file.close();
}

unsigned Segment::getBlockStatusForBlockAtIndex(unsigned index) {
    unsigned summaryBlockIdx = index / Block::blockSize;
    unsigned summaryBlockBlockOffset = index % Block::blockSize;
    return blocks[summaryBlockIdx].readUnsignedAtIndex(summaryBlockBlockOffset);
}

unsigned Segment::getINodeStatusForBlockAtIndex(unsigned index) {
    unsigned summaryBlockIdx = index / Block::blockSize;
    unsigned summaryBlockBlockOffset = index % Block::blockSize;
    return blocks[summaryBlockIdx].readUnsignedAtIndex(
        summaryBlockBlockOffset + sizeof(unsigned));
}

unsigned Segment::emptyBlockCount() {
    unsigned emptyCount = 0;
    for (unsigned i = 0; i < summaryBlockCount; i++) {
        for (unsigned j = 0; j < Block::blockSize; j += sizeof(unsigned) * 2) {
            unsigned blockStatus = blocks[i].readUnsignedAtIndex(j);
            unsigned iNodeStatus = blocks[i].readUnsignedAtIndex(j + sizeof(unsigned));
            if (blockStatus == std::numeric_limits<unsigned>::max()
                && iNodeStatus == std::numeric_limits<unsigned>::max()) {
                emptyCount++;
            }
        }
    }
    return emptyCount;
}

bool Segment::isEmpty() {
    return emptyBlockCount() == maxBlocks;
}


void Segment::write() {
    for (auto block: blocks) {
        file << block;
    }
    file.clear();
    file.seekg(0, std::ios::beg);
}

unsigned Segment::addBlock(Block& block, unsigned blockIndex,
        unsigned iNodeIndex, unsigned howMany) {
    if (currentBlockIdx + howMany < maxBlocks) {
        blocks[currentBlockIdx] = block;
        // update summary block
        unsigned summaryBlockAbsoluteIdx = (currentBlockIdx - summaryBlockCount) * (sizeof(unsigned) * 2);
        unsigned summaryBlockIdx = summaryBlockAbsoluteIdx / Block::blockSize;
        unsigned summaryBlockBlockOffset = summaryBlockAbsoluteIdx % Block::blockSize;
        unsigned summaryBlockINodeIdx = summaryBlockBlockOffset + sizeof(unsigned);
        blocks[summaryBlockIdx].overwrite(&blockIndex, summaryBlockBlockOffset, sizeof(unsigned));
        blocks[summaryBlockIdx].overwrite(&iNodeIndex, summaryBlockINodeIdx, sizeof(unsigned));
        return currentBlockIdx++;
    }
    return 0;
}

unsigned Segment::addBlock(Block& block, unsigned blockIndex, unsigned iNodeIndex) {
    return addBlock(block, blockIndex, iNodeIndex, 0);
}

unsigned Segment::addBlock(INode& block, unsigned index) {
    return addBlock(block, 128, index, 0);
}

unsigned Segment::addBlock(IMap& block, unsigned index) {
    return addBlock(block, index, 10 * 1024, 0);
}
