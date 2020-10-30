#include <iostream>
//#include <fstream>
//#include <unistd.h>
//#include <ctime>

#include "15_puzzle.h"

using std::cin;
using std::cout;
using std::endl;

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0
//void process_mem_usage(double &vm_usage, double &resident_set) {
//    using std::ios_base;
//    using std::ifstream;
//    using std::string;
//
//    vm_usage = 0.0;
//    resident_set = 0.0;
//
//    // 'file' stat seems to give the most reliable results
//    //
//    ifstream stat_stream("/proc/self/stat", ios_base::in);
//
//    // dummy vars for leading entries in stat that we don't care about
//    //
//    string pid, comm, state, ppid, pgrp, session, tty_nr;
//    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
//    string utime, stime, cutime, cstime, priority, nice;
//    string O, itrealvalue, starttime;
//
//    // the two fields we want
//    //
//    unsigned long vsize;
//    long rss;
//
//    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
//                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
//                >> utime >> stime >> cutime >> cstime >> priority >> nice
//                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
//
//    stat_stream.close();
//
//    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
//    vm_usage = vsize / 1024.0;
//    resident_set = rss * page_size_kb;
//}


/*
 * Acadêmico: Maicon Henrique Rodrigues do Nascimento RA102504
 *
 * Programa desenvolvido com base em sistema Linux.
 *
 */
int main() {
//    clock_t begin = clock();

    auto board = new short[4][4];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cin >> board[i][j];
        }
    }

    auto result = calc_lowest_move_number_to_final_board(board);

    cout << result.second << endl;

//    clock_t end = clock();

//    cout << endl << "Time: " << double(end - begin) / CLOCKS_PER_SEC << "s" << endl;

//    double vm, rss;
//    process_mem_usage(vm, rss);
//    cout << "VM: " << vm << "KB" << endl << "RSS: " << rss << "KB" << endl;

    return 0;
}