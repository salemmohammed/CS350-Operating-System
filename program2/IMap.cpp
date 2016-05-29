#include "IMap.h"

IMap::IMap()
    : Block(),
    currentIdx{0},
    freeCount{blockSize} {
    
}

IMap::IMap(Block& b)
    : Block(b),
    iNodeAddresses(blockSize/4, 0),
    currentIdx{0},
    freeCount{blockSize} {
        for (unsigned i = 0; i < blockSize; i += sizeof(unsigned)) {
            unsigned address = 0;
            memcpy(&address, &(data[i]), sizeof(address));
            iNodeAddresses[i/4] = address;
            if (address != 0) {
                freeCount--;
                currentIdx = i + sizeof(unsigned);
            }
        }
    }

unsigned IMap::addINodeWithAddress(unsigned address) {
    unsigned addressIdx = currentIdx;
    // TODO - add to free spots when at end of list
    if (currentIdx < blockSize) {
        for (unsigned i = 0; i < sizeof(address); i++) {
            data[currentIdx + i] = ((char*)&address)[i];
        }
        iNodeAddresses[addressIdx/4] = address;
        currentIdx += sizeof(address);
        freeCount--;
    }
    return addressIdx;
}

void IMap::updateINodeAddressAtIndex(unsigned address, unsigned index) {
    memcpy(&(data[index]), &address, sizeof(address));
    iNodeAddresses[index/4] = address;
}


void IMap::removeINodeAtIndex(unsigned index) {
    for (unsigned i = 0; i < sizeof(unsigned); i++) {
        data[index + i] = '\0';
    }
    iNodeAddresses[index/4] = 0;
    freeCount++;
}

bool IMap::hasFree() {
    // return freeCount > 0;
    return currentIdx < blockSize;
}

unsigned IMap::getNextINodeIndex() {
    return currentIdx;
}

unsigned IMap::getIndexForINodeAddress(unsigned address) {
	// assumes address is in this imap.
	unsigned index = 0;
	for (auto iter = iNodeAddresses.begin(); iter != iNodeAddresses.end(); iter++) {
		if (*iter == address) {
			index = std::distance(iNodeAddresses.begin(), iter);
			break;
		}
	}
	return index;
}

// bool hasFreeIndecies() {
//     return freeCount > 0;
// }

// unsigned findFreeIndex() {
//     if (currentIdx < blockSize) {
//         return currentIdx;
//     }

//     for (unsigned i = 0; i < blockSize; i++) {
//         if (data[i] == '\0') {
//             return i;
//         }
//     }
// }