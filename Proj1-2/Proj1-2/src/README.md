# Simulator

## File Structure

```tree
.
├── args.h          - Arguments Parser
├── cache.cc
├── cache.h
├── main.cc
├── Makefile
├── monitor.h       - Data Collector Master
├── para.h
├── README.md
├── simulator.h     - Cache Wrapper
├── statistic.h     - Data Collector
├── world.cc        - Deprecated
└── world.h         - Deprecated
```

## 伪代码

```c++
void Cache::Visit(uint32_t addr, bool isWrite){
    if(IsDisk) return;
    C = cache[parser(addr).index];
    if(C.Hit(addr, isWrite)){
        // Cache Hit
    }else{
        // Cache Miss
        if(HasVic()){
            // Has Victim (L1)
            if(Vic.Hit(addr, isWrte)){
                // Victim Cache Hit
                SWAP(C.LRU, Vic.LastHit);
            }else{
                // Victim Cache Miss
                if(C.HasEmpty()){
                    // L1 has Idle Block
                    NextLevel.Visit(addr, false);
                    C.Empty = Block(addr, isWrite);
                }else{
                    // L1 is FULL
                    if(Vic.LRU.D) // Write Back Victim
                        NextLevel.Visit(Vic.LRU.Addr, true); 
                    NextLevel.Visit(addr, false);
                    Vic.LRU = C.LRU;
                    C.LRU = Block(addr, isWrite);
                }
            }
        }else{
            // No Victim (L2)
            if(C.LRU.D) // Write Back
                NextLevel.Visit(C.LRU.Addr, true);
            NextLevel.Visit(addr, false);
            C.LRU = Block(addr, isWrite);
        }
    }
}
```
