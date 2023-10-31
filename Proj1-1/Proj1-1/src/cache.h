#ifndef __CACHE_H__
#define __CACHE_H__
#include "para.h"
#include "statistic.h"
#include <bitset>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <ostream>
#include <vector>

class Cache {
private:
    CacheParam p;
    Statistic st;
    uint32_t lru = 0;       // LRU Global Counter
    struct Block{
        uint32_t tag = 0;   // tag
        uint32_t cb = 0;    // count block
        uint32_t lu = 0;    // Last Used
        bool d = false; // dirty
        bool v = false; // valid
        bool operator<(const Block &t) {return cb < t.cb;}
        friend ostream& operator<<(ostream& os, Block&b) {
            os << "{tag:" << b.tag << ", Count-Block:" << b.cb << ", Last-Used:" << b.lu << ", Dirty:" << b.d << ", Valid:" << b.v << "}";
            return os;
        }
        
    };

    struct Set {
        uint32_t cs = 0;    // count set
        vector<Block> bs;    // Blocks
    };

    vector<Set> c;  // cache
    
    Cache * L2;

public:
    Cache(){}
    Cache(int argc, char *argv[]);
    
    void Write(uint32_t addr);
    void Read(uint32_t addr);

    void Visit(uint32_t addr, bool isWrite);

    friend ostream& operator<<(ostream& os, Cache& c){

        os << c.p;
        
        os << "===== L1 contents =====" << endl;
        for (uint32_t i=0;i!=c.c.size();i++){
            os << "set" << setw(4) << i << ":";
            for(auto &b:c.c[i].bs)
                os << hex << setw(8) << b.tag << ' ' << ((c.p.WritePolicy() == CacheParam::WBWA && b.d) ? 'D' : ' ');
            os << endl << dec;
        }

        os << c.st;
        return os;
    }

    void Run();
    
};

#endif // !__CACHE_H__
