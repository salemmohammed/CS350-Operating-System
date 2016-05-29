#ifndef _LFS_H_
#define _LFS_H_

#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Segment.h"
#include "IMap.h"
#include "INode.h"

class LFS {
    public:
        LFS();
        ~LFS();

        void import(std::string& lfsFileName, std::istream& data);
        std::string list();
        void remove(std::string& lfsFileName);
        std::string cat(std::string lfsFileName);
        std::string display(std::string lfsFileName, int howMany, int start);
        void overwrite(std::string lfsFileName, int howMany, int start, char c);
        void flush();
        void clean(unsigned numToClean);
        int findFile(std::string lfsFileName);

    private:
    	std::fstream checkpointFile;
        std::vector<Segment*> segments;
        std::vector<bool> isClean;
        std::vector<unsigned> iMapAddresses;
        std::map<std::string, unsigned> files;
        unsigned currentIMapIdx;
        unsigned currentSegmentIdx;
        unsigned currentBlockIdx;
        unsigned numCleanSegments;

        unsigned getBlockIndexFromAddress(unsigned address);
        unsigned getSegmentIndexFromAddress(unsigned address);
        unsigned getImapIndexFromINodeAddress(unsigned address);
        void selectNewCleanSegment(bool recursion = false);
        void cleanSegmentAtIndex(unsigned index);
        void combineSegments(unsigned fullIndex, unsigned emptyIndex);
        unsigned countDeadBlocksForSegmentAtIndex(unsigned address);
        void updateClean();
        void flushCheckpoint();
};

#endif
