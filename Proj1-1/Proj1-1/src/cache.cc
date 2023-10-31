#include "cache.h"
#include "para.h"
#include <algorithm>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
using namespace std;
Cache::Cache(int argc, char *argv[]){
    p.Extract(argc, argv);
    st.SetCP(&p);
    c.resize(p.GetMaxIndex());
    for (auto &b : c) b.bs.resize(p.GetAs());
}

void Cache::Read(uint32_t rawAddr){
    return Visit(rawAddr, false);
}

void Cache::Write(uint32_t rawAddr){
    return Visit(rawAddr, true);
}

void Cache::Visit(uint32_t rawAddr, bool isWrite){

    isWrite ? st.Write() : st.Read();

    auto addr = p.AddrParser(rawAddr);
    auto s = &c[addr.index];
    auto tag = addr.tag;

    auto hit = find_if(
        s->bs.begin(), 
        s->bs.end(), 
        [tag](const Block& b) { 
            return b.v && b.tag == tag; 
        }
    );

    if (hit != s->bs.end()){
        // Cache Hit!
        hit->cb ++;
        hit->lu = lru ++;
        if (isWrite && p.WritePolicy() == CacheParam::WBWA) {
            hit->d = true;
        }
        return;
    }

    isWrite ? st.WriteMiss() : st.ReadMiss();

    if (isWrite && p.WritePolicy() == CacheParam::WTNA) 
        return;

    auto toReplace = find_if(
        s->bs.begin(),
        s->bs.end(),
        [](const Block& b){
            return !b.v;
        }
    );

    if (toReplace == s->bs.end()){
        // Replace
        toReplace = min_element(
            s->bs.begin(), 
            s->bs.end(), 
            p.ReplicementPolicy() == CacheParam::LRU 
            ? 
            [](const Block &a, const Block &b){ return a.lu < b.lu; }
            : 
            [](const Block&a, const Block&b){ return a.cb < b.cb; }
        );
    }

    if (toReplace->d) {
        // Write to the L2 or Disk
        st.WriteBack();
    }

    s->cs = toReplace->cb;

    *toReplace = {addr.tag, s->cs + 1, lru ++, isWrite, true};

    // TODO: L2 Cache

    // if (isWrite){
    //     if (p.WritePolicy() == CacheParam::WBWA) {
    //         // Write through
    //         st.WriteBack();
    //     }
    // } else {
    //     // Bring from Disk or L2
    // }

}

void Cache::Run(){
    ifstream trace("../traces/" + p.TraceFile());
    string line;
    char op;
    unsigned addr;
    while(getline(trace, line) && (istringstream(line) >> op >> hex >> addr)) {
        Visit(addr, op == 'w');
    }
    trace.close();
}

