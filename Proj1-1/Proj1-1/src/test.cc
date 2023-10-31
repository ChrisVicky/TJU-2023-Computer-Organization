#include "para.h"
#include <exception>
#include <iostream>
using namespace std;
int main(int argc, char *argv[]) {
    ///////////////////////////// Test Extraction
    CacheParam p;
    try {
        p.Extract(argc, argv);
    }
    catch (const exception &e) {
        cout << e.what() << endl;
        cout << p.GetHelp() << endl;
        return 1;
    }
    cout << p << endl;
}
