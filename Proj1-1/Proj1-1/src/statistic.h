#ifndef __STATUS_H__
#define __STATUS_H__
#include "para.h"
#include <ostream>
class Statistic{
private:
    int r = 0;      // Read Count
    int rm = 0;     // Read Miss
    int w = 0;      // Write Count
    int wm = 0;     // Write Miss
    int wb = 0;     // Write Back

    CacheParam *p;


public:
    void SetCP(CacheParam *cp)  {   p = cp; }

    void Read()                 {   r++;    }
    void ReadMiss()             {   rm++;   }
    void Write()                {   w++;    }
    void WriteMiss()            {   wm++;   }
    void WriteBack()            {   wb++;   }
    int GetCommunication()      {   return rm+wm+wb;    }

    double GetMR() {
        return (rm + wm) / (double)(r+w);
    }

    /**
     * @brief Get Average Access Time
     * @return 
     */
    double GetAAT() {
        double HT = p->GetHT();
        double MP = p->GetMP();
        double MR = GetMR();
        return HT + (MP*MR);
    }

    friend ostream& operator<<(ostream &os, const Statistic& st){

        os << "\n  ====== Simulation results (raw) ======\n";
        os << "  a. number of L1 reads:" << setw(16) << st.r << endl;
        os << "  b. number of L1 read misses:" << setw(10) << st.rm << endl;
        os << "  c. number of L1 writes:" << setw(15) << st.w << endl;
        os << "  d. number of L1 write misses:" << setw(9) << st.wm << endl;
        double mr = (st.rm + st.wm) / (double)(st.r + st.w);
        os << "  e. L1 miss rate:" << setw(22) << fixed << setprecision(4) << mr << endl;
        os << "  f. number of writebacks from L1:" << setw(6) << st.wb << endl;
        os << "  g. total memory traffic:" << setw(14) << ((st.p->WritePolicy()) ? (st.rm + st.w) : (st.rm + st.wm + st.wb));
        os << "\n\n  ==== Simulation results (performance) ====\n";
        double ht = st.p->GetHT();
        double mp = st.p->GetMP();
        os << "  1. average access time:" << setw(15) << ht + mr * mp << " ns";
        return os;
    }

};
#endif // !__STATUS_H__
