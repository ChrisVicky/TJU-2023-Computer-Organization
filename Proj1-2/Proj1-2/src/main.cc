#include "simulator.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    auto s = Simulator(argc, argv);
    s.Run();
    return 0;
}
