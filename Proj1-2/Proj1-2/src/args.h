#ifndef __ARGS_H__
#define __ARGS_H__

#include <ostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
using namespace std;
class Args{

    class Exception: public exception{
    private: 
        string message;
    public:
        Exception(string msg, string help): message("\033[31m" + msg + "\n" + help + "\033[0m") {};
        const char * what() const throw() {
            return message.c_str();
        }
    };

public:
    int BLOCKSIZE;
    int L1_SIZE;
    int L2_SIZE;
    int L1_ASSOC;
    int L2_ASSOC;
    int Victim_Cache_SIZE;
    string trace_file;

    friend ostream& operator<<(ostream& os, Args &a){
        os << setiosflags(ios::left);
        os << "===== Simulator configuration =====" << endl;
        os << setw(30) << "BLOCKSIZE:" << a.BLOCKSIZE << endl;
        os << setw(30) << "L1_SIZE:" << a.L1_SIZE << endl;
        os << setw(30) << "L1_ASSOC:" << a.L1_ASSOC<< endl;
        os << setw(30) << "Victim_Cache_SIZE:" << a.Victim_Cache_SIZE << endl;
        os << setw(30) << "L2_SIZE:" << a.L2_SIZE << endl;
        os << setw(30) << "L2_ASSOC:" << a.L2_ASSOC << endl;
        os << setw(30) << "trace_file:" << a.trace_file << endl;
        os << "===================================" << endl;
        return os;
    }

    Args(){}

    /**
     * @brief Extract Cache Parameter p
     *
     * @param argc
     * @param argv 
     * @return 
     *  1: Error (throw)
     *  0: Success
     */
    Args(int argc, char* argv[]){

        std::stringstream strh;
        strh << "Usage: " << argv[0] << " <BLOCKSIZE> <L1_SIZE> <L2_SIZE> <L1_ASSOC> <L2_ASSOC> <Victim_Cache_SIZE> <trace_file>" << endl;

        std::stringstream stre;
        if(argc != 8) {
            stre << "Error! Must have all 7 Arguments!";
            throw Exception(stre.str(), strh.str());
        }

        BLOCKSIZE = atoi(argv[1]);
        if ((BLOCKSIZE < 0) || !(BLOCKSIZE ^ (BLOCKSIZE-1))) {
            stre << "Block Size: '" << BLOCKSIZE << "' INVALID! Must be (2^x) && > 0";
            throw Exception(stre.str(), strh.str());
        }

        L1_SIZE = atoi(argv[2]);
        if (L1_SIZE <= 0) {
            stre << "L1 Size: '"<< L1_SIZE << "' INVALID! Must be > 0";
            throw Exception(stre.str(), strh.str());
        }

        L2_SIZE = atoi(argv[3]);
        if (L2_SIZE < 0) {
            stre << "L2 Size: '"<< L2_SIZE << "' INVALID! Must be >= 0";
            throw Exception(stre.str(), strh.str());
        }

        L1_ASSOC = atoi(argv[4]);
        if (L1_ASSOC < 1) {
            stre << "L1 Association: '" << L1_ASSOC << "' INVALID! Must be >= 1";
            throw Exception(stre.str(), strh.str());
        }

        L2_ASSOC = atoi(argv[5]);
        if (L2_ASSOC < 0) {
            stre << "L2 Association: '" << L2_ASSOC << "' INVALID! Must be >= 0";
            throw Exception(stre.str(), strh.str());
        }

        Victim_Cache_SIZE = atoi(argv[6]);
        if(Victim_Cache_SIZE < 0) {
            stre << "Victim Cache SIZE: '" << Victim_Cache_SIZE << "' INVALID! Must be >= 0";
            throw Exception(stre.str(), strh.str());
        }

        trace_file = std::string(argv[7]);
        struct stat buffer;
        if (stat(("../traces/"+trace_file).c_str(), &buffer)) {
            stre << "Trace File: '" << trace_file << "' INVALID! Cannot Open it";
            throw Exception(stre.str(), strh.str());
        }

    }
};

#endif // !__ARGS_H__j
