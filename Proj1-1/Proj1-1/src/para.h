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

class Exception: public exception{
private: 
    string message;
public:
    Exception(string msg, string help): message("\033[31m" + msg + "\n" + help + "\033[0m") {};
    const char * what() const throw() {
        return message.c_str();
    }
};

struct TIO {
    uint32_t tag, index, offset;
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
    string  tf;    // Trace File Name
    string  help;  // help

    // +-------------+------+---+
    // |     t       |   i  | o |
    // +-------------+------+---+

public:
    int     t;      // tag bit
    int     i;      // index
    int     o;      // offset

    uint32_t om;    // offset mask
    uint32_t im;    // index mask

public:

    int BlockSize() {return bs;}
    int Size() {return s;}
    int Assoc() {return as;}
    int ReplicementPolicy() {return rp;}
    int WritePolicy() {return wp;}
    string TraceFile() {return tf;}

    /**
     * @brief Extract Cache Parameter p
     *
     * @param argc
     * @param argv 
     * @return 
     *  1: Error (throw)
     *  0: Success
     */
    int Extract(int argc, char* argv[]);

    // ===== Simulator configuration =====
    // L1_BLOCKSIZE:                    16
    // L1_SIZE:                      16384
    // L1_ASSOC:                         1
    // L1_REPLACEMENT_POLICY:            0
    // L1_WRITE_POLICY:                  0
    // trace_file:           gcc_trace.txt
    // ===================================
    friend ostream& operator<<(ostream &os, const CacheParam& cp){
        os << "  ===== Simulator configuration =====\n";
        os << "  L1_BLOCKSIZE:" << setw(22) << cp.bs << endl;
        os << "  L1_SIZE:" << setw(27) << cp.s << endl;
        os << "  L1_ASSOC:" << setw(26) << cp.as << endl;
        os << "  L1_REPLACEMENT_POLICY:" << setw(13) << cp.rp << endl;
        os << "  L1_WRITE_POLICY:" << setw(19) << cp.wp << endl;
        os << "  trace_file:" << setw(24) << cp.tf << endl;
        os << "  ===================================\n\n";
        return os;
    }

    /**
     * @brief Get HT
     * @return 0.25 + 2.5 * (s/ (512.0 * 1024)) + 0.025 * (bs/ 16.0) + 0.025 * as;
     */
    double GetHT() {
        return 0.25 + 2.5 * (s/ (512.0 * 1024)) + 0.025 * (bs/ 16.0) + 0.025 * as;
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
    }

    TIO AddrParser(uint32_t addr){
        uint32_t offset = addr & om;
        uint32_t index = (addr & im) >> o;
        uint32_t tag = addr >> (o+i);
        return {tag, index, offset};
    }

    string GetHelp(){return help;}

};


#endif // !__PARA_H__
