#ifndef __PARA_H__
#define __PARA_H__
#include <cstdint>
#include <bitset>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

struct TIO {
    uint32_t tag, index, offset, rAddr;
};

/**
 * @class CacheParam
 * @brief
 *
 */
class CacheParam {
public:
    enum {
        LRU = 0,
        LFU
    };

    enum {
        WBWA = 0,
        WTNA
    };

private:
    int     bs;     // Block Size
    int     s;      // Size 
    int     as;     // Assoc
    int     rp;     // Replicement Policy
    int     wp;     // Write Policy: 1 -> WTNA, 0 -> 
    
    int     vs;     // Victim Size
    int     vas;    // Victim Asso

    // +-------------+------+---+
    // |     t       |   i  | o |
    // +-------------+------+---+

public:
    int     t;      // tag bit
    int     i;      // index
    int     o;      // offset
     
    uint32_t om;    // offset mask
    uint32_t im;    // index mask

    int     vt;     // victim tag bit
    int     vi;     // victim index
    int     vo;     // victim offset

    uint32_t vom;   // victim offset mask
    uint32_t vim;   // victim index mask


public:

    CacheParam(){}

    CacheParam(int blocksize, int size, int ass, int vis, int rep, int wrp){
        bs = blocksize;
        s = size;
        as = ass;
        vs = vis;
        rp = rep;
        wp = wrp;
        setupTIO();
    }
    

    int BlockSize() {return bs;}
    int Size() {return s;}
    int Assoc() {return as;}
    int ReplicementPolicy() {return rp;}
    int WritePolicy() {return wp;}

    /**
     * @brief Get HT
     * @return 0.25 + 2.5 * (s/ (512.0 * 1024)) + 0.025 * (bs/ 16.0) + 0.025 * as;
     */
    double GetHT() {
        return 0.25 + 2.5 * (s/ (512.0 * 1024)) + 0.025 * (bs/ 16.0) + 0.025 * as;
    }

    double GetHT2(){
        return 2.5 + 2.5 * (s / (512.0 * 1024)) + 0.025 * (bs / 16.0) + 0.025 * as;
    }

    /**
     * @brief Get Miss Penalty
     * @return 20 + 0.5 * (bs / 16.0);
     */
    double GetMP() {
        return 20 + 0.5 * (bs / 16.0);
    }

    int log2_floor(uint32_t x) {
        return x == 0 ? -1 : 31 - __builtin_clz(x); 
    }

    uint32_t GetMaxIndex() {
        return (1<<i);
    }

    uint32_t GetVictimAs(){
        return vas;
    }

    uint32_t GetAs() {
        return as;
    }

    void setupTIO(){
        // setup t, i, b
        o = log2_floor(bs);
        i = log2_floor(s/(bs*as));
        t = 32 - o - i;
        
        om = (1<<o) - 1;
        im = ((1<<i)-1) << o;

        if (!vs) return;

        vo = log2_floor(bs);
        vi = 0;
        vt = 32 - vo - vi;
        vas = vs / bs;

        vom = (1<<vo) - 1;
        // vim = ((1<<vi)-1) << vo;

    }

    TIO VictimAddrParser(uint32_t addr){
        uint32_t offset = addr & vom;
        // uint32_t index = (addr & vim) >> vo;
        uint32_t tag = addr >> vo;
        return {tag, 0, offset, addr};
    }

    TIO AddrParser(uint32_t addr){
        uint32_t offset = addr & om;
        uint32_t index = (addr & im) >> o;
        uint32_t tag = addr >> (o+i);
        return {tag, index, offset, addr};
    }

    uint32_t UnParser(uint32_t tag, TIO tio){
        return (tag << (o+i)) | (tio.index << o) | (tio.offset);
    }

    uint32_t UnParserVic(uint32_t tag, TIO tio){
        return (tag << (vo)) | (tio.offset);
    }

};


#endif // !__PARA_H__
