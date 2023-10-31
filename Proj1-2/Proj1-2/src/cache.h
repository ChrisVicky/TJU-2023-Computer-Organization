#ifndef __CACHE_H__
#define __CACHE_H__
#include "para.h"
#include "statistic.h"
#include <algorithm>
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
    struct Block{
        uint32_t tag = 0;   // tag
        uint32_t lru = 0;    // Last Used
        bool d = false; // dirty
        bool v = false; // valid
        bool operator<(const Block& t)const{ return lru < t.lru; }
        friend ostream& operator<<(ostream& os, Block&b) {
            os << "{tag:" << b.tag << ", LRU:" << b.lru << ", Dirty:" << b.d << ", Valid:" << b.v << "}";
            return os;
        }

    };

    vector<vector<Block>> c;  // Cache
    vector<Block> vc;         // Victim Cache
    string level;

public:

    Cache *NextCache = nullptr;

    Cache(){}
    Cache(int argc, char *argv[]);
    Cache(int bs, int s, int as, int vs, string l="L1", int rp = CacheParam::LRU, int wp = CacheParam::LFU);
    Cache(int bs, int s, int as, int vs, Cache* n=nullptr, string l="L1", int rp = CacheParam::LRU, int wp = CacheParam::LFU);

    ~Cache(){
        delete NextCache;
    }

    Statistic* GetStatis() {return &st; }

    void Write(uint32_t addr);
    void Read(uint32_t addr);

    void Visit(uint32_t rawAddr, bool isWrite);

    friend ostream& operator<<(ostream& os, Cache& c){
        if (!c.p.Size()) return os;
        os << "===== " << c.level << " contents =====" << endl;
        for(unsigned i = 0; i != c.c.size(); i++){
            os << "set " << i << ": ";
            auto temp = c.c[i];
            stable_sort(temp.begin(), temp.end());
            // for(auto b : temp) os << hex << b.tag << (b.d ? " D  " : "    ") << (b.v ? " V  " : "    ") << b.lru << " ";
            for(auto b : temp) os << hex << b.tag << (b.d ? " D  " : "    "); 
            os << endl << dec;
        }
        if(c.vc.size()){
            os << "===== Victim Cache contents =====" << endl;
            os << "set 0: ";
            auto temp = c.vc;
            stable_sort(temp.begin(), temp.end());
            // for(auto b : temp) os << hex << b.tag << (b.d ? " D  " : "    ") << (b.v ? " V  " : "    ") << b.lru << " ";
            for(auto b : temp) os << hex << b.tag << (b.d ? " D  " : "    "); 
            os << endl << dec;
        }
        return os;
    }

    void Run();

};

#endif // !__CACHE_H__
