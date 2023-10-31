#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "para.h"

int CacheParam::Extract(int argc, char* argv[]){
    std::stringstream strh;
    strh << "Usage: " << argv[0] << " <L1_BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L1_REPLACEMENT_POLICY> <L1_WRITE_POLICY> <TRACE_FILE>\n\t<L1_REPLACEMENT_POLICY>: 0 for LRU, 1 for LFU\n\t<L1_WRITE_POLICY>: 0 for WBWA, 1 for WTNA" << endl;
    
    std::stringstream stre;
    if(argc != 7) {
        stre << "Error! Must have all 6 Arguments!";
        throw Exception(stre.str(), strh.str());
    }
    
    bs = atoi(argv[1]);
    if ((bs < 0) || !(bs ^ (bs-1))) {
        stre << "Block Size: '" << bs << "' INVALID! Must be (2^x) && > 0";
        throw Exception(stre.str(), strh.str());
    }

    s = atoi(argv[2]);
    if (s < 0) {
        stre << "Cache Size: '"<< s << "' INVALID! Must be > 0";
        throw Exception(stre.str(), strh.str());
    }

    as = atoi(argv[3]);
    if (as < 1) {
        stre << "Cache Association: '" << as << "' INVALID! Must be >= 1";
        throw Exception(stre.str(), strh.str());
    }

    rp = atoi(argv[4]);
    if (rp!=LRU && rp!=LFU){
        stre << "Replacement Policy: '" << rp << "' INVALID! Must be 0 (LRU) or 1 (LFU)";
        throw Exception(stre.str(), strh.str());
    }
    
    wp = atoi(argv[5]);
    if (wp!=WBWA && wp!=WTNA){
        stre << "Write Policy: '" << wp << "' INVALID! Must be 0 (WBWA) or 1 (WTNA)";
        throw Exception(stre.str(), strh.str());
    }
    
    tf = std::string(argv[6]);
    struct stat buffer;
    if (stat(("../traces/"+tf).c_str(), &buffer)) {
        stre << "Trace File: '" << tf << "' INVALID! Cannot Open it";
        throw Exception(stre.str(), strh.str());
    }
    
    setupTIO();
    
    return 0;
}

