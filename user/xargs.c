#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" // Chứa MAXARG 

int main(int argc, char *argv[]) {
    // Kiểm tra tham số đầu vào
    if (argc < 2) {
        fprintf(2, "Usage: xargs command...\n");
        exit(1);
    }

    char *xargs_argv[MAXARG];
    // TODO 1: Copy các lệnh ban đầu (từ argv[1] trở đi) vào mảng xargs_argv

    char buf;
    // Lặp đọc từng ký tự từ stdin 
    while (read(0, &buf, 1) > 0) {
        // TODO 2: Tích lũy các ký tự vào một mảng bộ đệm tạm thời (buffer)
        
        // Khi gặp khoảng trắng hoặc \n thì chốt một tham số
        if (buf == '\n') {
            // TODO 3: Trỏ phần tử tiếp theo của xargs_argv vào buffer vừa tạo
            
            // TODO 4: Dùng fork() để tạo process mới 
            int pid = fork();
            if (pid == 0) {
                // Tiến trình con: gọi exec() 
                // exec(xargs_argv[0], xargs_argv);
            } else {
                // Tiến trình cha: gọi wait() 
                wait(0);
            }
            
            // TODO 5: Reset lại mảng tham số chuẩn bị cho dòng tiếp theo
        }
    }
    exit(0);
}