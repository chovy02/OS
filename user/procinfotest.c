#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

int main() {
    struct procinfo info; // [cite: 199]
    if(procinfo(getpid(), &info) == 0){ // [cite: 200]
        printf("Process: %s\n", info.name); // [cite: 201]
        printf("PID: %d, PPID: %d\n", info.pid, info.ppid); // [cite: 202]
        printf("State: %d\n", info.state); // (Sửa %s thành %d vì state là số nguyên) [cite: 203]
        printf("Memory: %ld bytes\n", info.sz); // [cite: 204]
    } else {
        printf("Failed to get procinfo\n");
    }
    exit(0);
}