#include "print.h"
#include "pong.h" 

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    
    print_str("========================================\n");
    print_str("||          Welcome to DarkOS         ||\n");
    print_str("||        Created by Sai-Sys-Cmd      ||\n");
    print_str("========================================\n\n");

    start_pong_game();
    while(1);
}
