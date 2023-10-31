#include "cache.h"
#include "para.h"
#include <algorithm>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
using namespace std;

Cache::Cache(int bs, int s, int as, int vs, string l, int rp, int wp){

    p = CacheParam(bs, s, as, vs, rp, wp);
    st.SetCP(&p);
    c.resize(p.GetMaxIndex());
    level = l;
    for (auto &b : c) b.resize(p.GetAs());

    if (vs) vc.resize(p.GetVictimAs());
}

Cache::Cache(int bs, int s, int as, int vs, Cache* n, string l, int rp, int wp){

    p = CacheParam(bs, s, as, vs, rp, wp);
    st.SetCP(&p);
    c.resize(p.GetMaxIndex());
    for (auto &b : c) b.resize(p.GetAs());

    if (vs) vc.resize(p.GetVictimAs());

    NextCache = n;
    level = l;
}

void Cache::Read(uint32_t rawAddr){
    Visit(rawAddr, false);
}

void Cache::Write(uint32_t rawAddr){
    Visit(rawAddr, true);
}

void Cache::Visit(uint32_t rawAddr, bool isWrite){

    isWrite ? st.Write() : st.Read();

    auto addr = p.AddrParser(rawAddr);
    auto s = &c[addr.index];

    // Find in Cache
    auto cl = find_if(
        s->begin(), 
        s->end(), 
        [=](const Block& b) { 
            return b.v && b.tag == addr.tag; 
        }
    );

    if (cl != s->end()){
        // Cache Hit!
        for(auto &b:(*s)) b.lru += (b.lru < cl->lru);
        cl->lru = 0;
        cl->d |= isWrite;
        return ;
    }

    // Main Cache Miss
    cl = find_if(
        s->begin(),
        s->end(),
        [](const Block& b){
            return !b.v;
        }
    );

    if (cl == s->end()){
        // Replace
        cl = max_element(
            s->begin(), 
            s->end()
        );
    }

    if(!vc.size()){
        isWrite ? st.WriteMiss() : st.ReadMiss();

        // No Victim Cache
        if (cl->v && cl->d) {
            // Write to the L2 or Disk
            st.WriteBack();
            if(NextCache!=nullptr) {
                NextCache->Visit(p.UnParser(cl->tag, addr), true);
            }
        }

        if(NextCache!=nullptr){
            // 2. Find it from Next Level
            NextCache->Visit(rawAddr, false);
        }

        for(auto& b:(*s)) b.lru ++;
        *cl = {addr.tag, 0, isWrite, true};

    }else{
        // Has Victim Cache: Replace with Victim

        // Miss Write or Read Miss, Go to Next Level
        // 1. Find it from Victim Cache
        auto vaddr = p.VictimAddrParser(rawAddr);
        auto vcp = &vc;

        auto vic = find_if(
            vcp->begin(),
            vcp->end(),
            [=](const Block&b){
                return b.v && b.tag == vaddr.tag;
            }
        );

        if(vic != vcp->end()){
            // V. Cache Hit from Victim!
            auto tmp = *vic;
            if(cl->v){
                for(auto &b:*vcp) b.lru += (b.lru < tmp.lru);
                *vic = {p.VictimAddrParser(p.UnParser(cl->tag, addr)).tag, 0, cl->d, true};
            }

            for(auto &b:(*s)) b.lru ++;
            *cl = {addr.tag, 0, isWrite || tmp.d, true};
            st.Exchange();
            return ;
        } 

        // X. Victim Cache Miss!
        isWrite ? st.WriteMiss() : st.ReadMiss();


        if (!cl->v){
            if(NextCache!=nullptr){
                // 2. Find it from Next Level
                NextCache->Visit(rawAddr, false);
            }
            // Empty Level 1
            for(auto &b:(*s)) b.lru ++;
            *cl = {addr.tag, 0, isWrite , true};
            return;
        }

        vic = find_if(
            vcp->begin(),
            vcp->end(),
            [=](const Block&b){
                return !b.v;
            }
        );

        if(vic == vcp->end()){
            // Vic Full, find one to deprecate
            vic = max_element(
                vcp->begin(),
                vcp->end()
            );
            if (vic->v && vic->d){
                st.WriteBack();
                if(NextCache!=nullptr) {
                    NextCache->Visit(p.UnParserVic(vic->tag, vaddr), true);
                }
            }
        }
        // New Victim Cache 
        for(auto &b:*vcp) b.lru ++;
        *vic = {p.VictimAddrParser(p.UnParser(cl->tag, addr)).tag, 0, cl->d, true};

        for(auto &b:(*s)) b.lru ++;
        *cl = {addr.tag, 0, isWrite , true};
        if(NextCache!=nullptr){
            // 2. Find it from Next Level
            NextCache->Visit(rawAddr, false);
        }

    }


    return ;
}

