#include "print.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);

    print_str("\n");
    print_str("\n");
    print_str("\n");
    print_str("========================================\n");
    print_str("||          Welcome to DarkOS         ||\n");
    print_str("||        Created by Sai-Sys-Cmd      ||\n");
    print_str("========================================\n\n");

    print_str("System Information:\n");
    print_str("-------------------\n");
    print_str("Hostname: user-hostname\n");
    print_str("IP Address: 192.168.1.1\n");
    print_str("Memory: 8192 MB total\n");
    print_str("Free Memory: 4096 MB\n");
    print_str("Used Memory: 4096 MB\n");
    print_str("Filesystem: /dev/sda1\n");
    print_str("Disk Usage: 50% used, 50% free\n");
    print_str("Mounted on: /home\n\n");

    print_str("Available Commands:\n");
    print_str("-------------------\n");
    print_str("echo, cd, ls, pwd, touch, cat, rm, cp, mv\n");
    print_str("exit, help, clear, date, whoami, hostname\n");
    print_str("ps, top, kill, env, mkdir, rmdir, chmod, chown\n");
    print_str("ln, df, du, uptime, reboot, ip, free, find, history\n\n");

    print_str("Type 'help' for a full list of commands.\n");
    print_str("Type 'exit' to quit the shell.");
}
