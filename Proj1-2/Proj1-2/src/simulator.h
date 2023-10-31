#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "args.h"
#include "cache.h"
#include "monitor.h"
#include <cstdint>
#include <fstream>
#include <iostream>
class Simulator{
    Args *a;
    Cache *c;
    Monitor *m;

public:
    Simulator(int argc, char**argv){
        a = new Args(argc, argv);
        if (a->L2_SIZE) {
            Cache *l2 = new Cache(a->BLOCKSIZE, a->L2_SIZE, a->L2_ASSOC, 0, "L2") ;
            c = new Cache(a->BLOCKSIZE, a->L1_SIZE, a->L1_ASSOC, a->Victim_Cache_SIZE, l2, "L1");
            m = new Monitor(c->GetStatis(), l2->GetStatis(), a->L2_SIZE, a->Victim_Cache_SIZE);
        }else{
            c = new Cache(a->BLOCKSIZE, a->L1_SIZE, a->L1_ASSOC, a->Victim_Cache_SIZE, nullptr, "L1");
            m = new Monitor(c->GetStatis(), nullptr, a->L2_SIZE, a->Victim_Cache_SIZE);
        }
    }
    ~Simulator(){
        delete a;
        delete c;
        delete m;
    }

    void Run(){
        ifstream trace("../traces/" + a->trace_file);
        string line;
        char op;
        unsigned addr;
        while(getline(trace, line) && (istringstream(line) >> op >> hex >> addr)) {
            c->Visit(addr, op == 'w');
            // cout << (*a);
            // cout << (*c);
            // if (m->Has2) cout << (*(c->NextCache));
            // cout << (*m);
        }
        trace.close();

        cout << (*a);
        cout << (*c);
        if (m->Has2) cout << (*(c->NextCache));
        cout << (*m);
    }

};


#endif // !__SIMULATOR_H__j
