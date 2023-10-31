#include "cache.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    auto c = Cache(argc, argv);
    c.Run();
    cout << c ;
}
