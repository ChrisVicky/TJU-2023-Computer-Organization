#ifndef __MONITOR_H__
#define __MONITOR_H__

#include "statistic.h"
#include <ostream>
#include <iomanip>
class Monitor{

public:
    Statistic *l1, *l2;
    bool Has2, HasV;
    Monitor(){}
    Monitor(Statistic* s1, Statistic* s2, bool h2, bool hv){
        l1 = s1, l2 = s2;
        Has2 = h2, HasV = hv;
    }
    friend ostream& operator<<(ostream& os, Monitor& m){
        os << "====== Simulation results (raw) ======" << endl;
        os << setw(38) << "a. number of L1 reads:               " << m.l1->r << endl;
        os << setw(38) << "b. number of L1 read misses:         " << m.l1->rm << endl;
        os << setw(38) << "c. number of L1 writes:"               << m.l1->w << endl;
        os << setw(38) << "d. number of L1 write misses:        " << m.l1->wm << endl;
        os << setw(38) << "e. L1 miss rate:                     " << fixed << setprecision(4) << m.l1->GetMR() << endl;
        os << setw(38) << "f. number of swaps:                  " << m.l1->ex << endl;
        os << setw(38) << "g. number of victim cache writeback: " << (m.HasV ? m.l1->wb : 0) << endl;
        if (m.Has2) {
            os << setw(38) << "h. number of L2 reads:               " << m.l2->r << endl;
            os << setw(38) << "i. number of L2 read misses:         " << m.l2->rm << endl;
            os << setw(38) << "j. number of L2 writes:              " << m.l2->w << endl;
            os << setw(38) << "k. number of L2 write misses:        " << m.l2->wm << endl;
            os << setw(38) << "l. L2 miss rate:                     " << fixed << setprecision((m.Has2) ? 4:0) <<  m.l2->GetMR2() << endl;
            os << setw(38) << "m. number of L2 writebacks:          " << m.l2->wb << endl;
            os << setw(38) << "n. total memory traffic:             " << (m.l2->rm+m.l2->wm+m.l2->wb)  << endl;
            os << "==== Simulation results (performance) ====" << endl;
            double HT1 = m.l1->p->GetHT(), MR1 = m.l1->GetMR();
            double HT2 = m.l2->p->GetHT2(), MR2 = m.l2->GetMR2();
            double MP = m.l2->p->GetMP();
            os << setw(32) << "1. average access time:" << fixed << setprecision(4) << HT1 + (MR1*(HT2+MR2*MP)) << " ns" << endl;

        }else{
            os << setw(38) << "h. number of L2 reads:               " << 0 << endl;
            os << setw(38) << "i. number of L2 read misses:         " << 0 << endl;
            os << setw(38) << "j. number of L2 writes:              " << 0 << endl;
            os << setw(38) << "k. number of L2 write misses:        " << 0 << endl;
            os << setw(38) << "l. L2 miss rate:                     " << 0 << endl;
            os << setw(38) << "m. number of L2 writebacks:          " << 0 << endl;
            os << setw(38) << "n. total memory traffic:             " << (m.l1->rm+m.l1->wm+m.l1->wb) << endl;
            os << "==== Simulation results (performance) ====" << endl;
            double ht = m.l1->p->GetHT();
            double mp = m.l1->p->GetMP();
            double mr = (m.l1->rm + m.l1->wm) / (double)(m.l1->r + m.l1->w);
            os << setw(32) << "1. average access time:" << fixed << setprecision(4) << ht + mr * mp << " ns" << endl;
        }

        return os;
    }

};
#endif // !__MONITOR_H__j
