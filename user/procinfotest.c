#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

void print_info(struct procinfo *info) {
    printf("Process: %s\n", info->name); 
    printf("PID: %d, PPID: %d\n", info->pid, info->ppid); 
    printf("State: %d\n", info->state); 
    printf("Memory: %ld bytes\n", info->sz); 
}

int main(int argc, char *argv[]) {
    struct procinfo info;
    int ret;

    // Test Case 1
    printf("[Test 1] Extract information of current process (PID %d)\n", getpid());
    ret = procinfo(getpid(), &info);
    if (ret == 0) {
        printf("  [PASS] Success!\n");
        print_info(&info);
    } else {
        printf("  [FAIL]\n");
    }
    printf("\n");

    // TEST 2: PID does not exist (Edge Case)
    printf("[Test 2] PID does not exit (PID 9999)\n");
    ret = procinfo(9999, &info);
    if (ret < 0) {
        printf("  [PASS] Error (Return %d).\n", ret);
    } else {
        printf("  [FAIL]\n");
    }
    printf("\n");

    // TEST 3: Use NULL pointer for info storage
    printf("[Test 3] Use NULL pointer for info storage\n");
    // 0 represents NULL pointer in User Space.
    ret = procinfo(getpid(), (struct procinfo *)0); 
    if (ret < 0) {
        printf("  [PASS] Copyout() refuses to write in NULL (Return %d).\n", ret);
    } else {
        printf("  [FAIL]\n");
    }
    printf("\n");

    // TEST 4: Get infomation from parent process
    printf("[Test 4] Get infomation from parent process\n");
    ret = procinfo(info.ppid, &info); // Use ppid from Test Case 1
    if (ret == 0) {
        printf("  [PASS] Success!\n");
        print_info(&info);
    } else {
        printf("  [FAIL]\n");
    }
    printf("\n");

    printf("[Test 5] Get information from process with PID = 1 (init)\n");
    ret = procinfo(1, &info);
    if (ret == 0) {
        printf("  [PASS] Success\n");
        print_info(&info);
    } else {
        printf("  [FAIL]!\n");
    }
    printf("\n");

    // TEST 6: Get information from child process
    printf("[Test 6] Using fork to create a child process\n");
    int child_pid = fork();
    if (child_pid == 0) {
        sleep(10);
        exit(0);
    } else if (child_pid > 0) {
        ret = procinfo(child_pid, &info);
        if (ret == 0) {
            printf("  [PASS] Success\n");
            print_info(&info);
        } else {
            printf("  [FAIL]\n");
        }
        wait(0); 
    }
    printf("\n");
    exit(0);
}