#include "pong.h"
#include "print.h"


static int ball_x = 40, ball_y = 12;

static int dx = 2, dy = 1;
static int p1_y = 12, p2_y = 12; 
static int score1 = 0, score2 = 0; 

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static char get_key(void) {
    if (!(inb(0x64) & 0x01))
         return 0;
    unsigned char scancode = inb(0x60);
    if (scancode & 0x80)
         return 0;
    switch(scancode) {
         case 0x11: return 'w';  
         case 0x1F: return 's';  
         case 0x17: return 'i';  
         case 0x25: return 'k';  
         default:   return 0;
    }
}

static void process_input(void) {
    char key = get_key();
    if(key == 'w' && p1_y > 3)
         p1_y--;
    if(key == 's' && p1_y < 21)
         p1_y++;
    if(key == 'i' && p2_y > 3)
         p2_y--;
    if(key == 'k' && p2_y < 21)
         p2_y++;
}

static void reset_ball(void) {
    ball_x = 40;
    ball_y = 12;
    dx = 2;
    dy = 1;
}

static void draw_game(void) {
    char screen[25][81];
    int i, j;

    for(i = 0; i < 25; i++) {
         for(j = 0; j < 80; j++)
              screen[i][j] = ' ';
         screen[i][80] = '\0';
    }


    {
         const char *label1 = "P1:";
         const char *label2 = "  P2:";
         for(j = 0; label1[j] && j < 10; j++)
              screen[0][j] = label1[j];
         screen[0][4] = ' ';
         screen[0][5] = '0' + score1;
         int pos = 7;
         for(j = 0; label2[j] && pos < 80; j++, pos++)
              screen[0][pos] = label2[j];
         if(score2 < 10)
              screen[0][pos] = '0' + score2;
         else
              screen[0][pos] = '9';
    }

    for (int offset = -1; offset <= 1; offset++) {
         int y = p1_y + offset;
         if (y >= 1 && y < 25)
              screen[y][2] = '|';
    }

    for (int offset = -1; offset <= 1; offset++) {
         int y = p2_y + offset;
         if (y >= 1 && y < 25)
              screen[y][77] = '|';
    }
.
    if(ball_y >= 1 && ball_y < 25 && ball_x >= 0 && ball_x < 80)
         screen[ball_y][ball_x] = 'O';

    print_clear();
    for(i = 0; i < 25; i++) {
         print_str(screen[i]);
         print_str("\n");
    }
}

static void update_game(void) {
    static int frame_count = 0;
    frame_count++;
    ball_x += dx;
    if(frame_count % 3 == 0)
         ball_y += dy;

    if(ball_y < 1) { ball_y = 1; dy = -dy; }
    if(ball_y > 24) { ball_y = 24; dy = -dy; }

    if(ball_x <= 3) {
         if(ball_y >= p1_y - 1 && ball_y <= p1_y + 1) {
              dx = -dx;
              ball_x = 4;
         }
    }
    if(ball_x >= 76) {
         if(ball_y >= p2_y - 1 && ball_y <= p2_y + 1) {
              dx = -dx;
              ball_x = 75;
         }
    }

    if(ball_x < 0) {
         score2++;
         reset_ball();
         frame_count = 0;
    }
    if(ball_x > 79) {
         score1++;
         reset_ball();
         frame_count = 0;
    }
}

static void pong_loop(void) {
    while(1) {
         process_input();
         update_game();
         draw_game();
         for(volatile int delay = 0; delay < 300000; delay++);
    }
}

void start_pong_game(void) {
    pong_loop();
}