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
    
    int base_argc = 0;
    for (int i = 1; i < argc; i++) {
        xargs_argv[base_argc] = argv[i];
        base_argc++;
    }
    
    int current_argc = base_argc; // Số lượng tham số hiện tại trong xargs_argv

    char param_buf[512];
    int buf_idx = 0;

    xargs_argv[current_argc] = &param_buf[buf_idx]; // Trỏ phần tử tiếp theo của xargs_argv vào buffer để nhận tham số từ stdin

    char buf;
    // Lặp đọc từng ký tự từ stdin 
    while (read(0, &buf, 1) > 0) {
        
        // Khi gặp khoảng trắng hoặc \n thì chốt một tham số
        if (buf == ' ' || buf == '\n') {
            param_buf[buf_idx] = '\0'; // Kết thúc chuỗi tham số

            current_argc++;
            buf_idx++;

            if (buf == '\n') {
                xargs_argv[current_argc] = 0; // Kết thúc mảng tham số với NULL
            
                // TODO 4: Dùng fork() để tạo process mới 
                int pid = fork();
                if (pid == 0) {
                    // Tiến trình con: gọi exec() 
                    exec(xargs_argv[0], xargs_argv);
                    fprintf(2, "xargs: exec failed\n");
                    exit(1);
                }
                else {
                    // Tien trình cha: gọi wait()
                    wait(0);
                }
                
                // TODO 5: Reset lại mảng tham số chuẩn bị cho dòng tiếp theo
                current_argc = base_argc; // Reset số lượng tham số về ban đầu
                buf_idx = 0; // Reset buffer index để nhận tham số mới
                xargs_argv[current_argc] = &param_buf[buf_idx]; // Trỏ lại phần tử tiếp theo của xargs_argv vào buffer
            }
            else {
                // Nếu là khoảng trắng, chỉ cần kết thúc tham số hiện tại và tiếp tục nhận tham số mới
                xargs_argv[current_argc] = &param_buf[buf_idx]; // Trỏ phần tử tiếp theo của xargs_argv vào buffer để nhận tham số mới
            }
        }
        else {
            param_buf[buf_idx] = buf; // Thêm ký tự vào buffer
            buf_idx++;
        }
    }
    exit(0);
}