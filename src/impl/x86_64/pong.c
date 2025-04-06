#include "pong.h"
#include "print.h"

// Game dimensions: 80 columns x 25 rows
static int ball_x = 40, ball_y = 12;
// Set constant ball velocity: horizontal is faster, vertical is slower.
static int dx = 2, dy = 1;
static int p1_y = 12, p2_y = 12;  // Paddle centers (allowed rows: 2 to 22)
static int score1 = 0, score2 = 0;  // Score for player 1 and 2

// Basic port reading (assumes x86 I/O instructions work)
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Poll the keyboard nonblocking and return an ASCII key for our keys.
static char get_key(void) {
    if (!(inb(0x64) & 0x01))
         return 0;
    unsigned char scancode = inb(0x60);
    if (scancode & 0x80)
         return 0;
    switch(scancode) {
         case 0x11: return 'w';  // left paddle up
         case 0x1F: return 's';  // left paddle down
         case 0x17: return 'i';  // right paddle up
         case 0x25: return 'k';  // right paddle down
         default:   return 0;
    }
}

// Process paddle input (each frame, one move per key press)
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

// Reset the ball to center with fixed velocity.
static void reset_ball(void) {
    ball_x = 40;
    ball_y = 12;
    dx = 2;
    dy = 1;
}

// Render the game to an 80x25 text screen.
// Row 0 is reserved for scoreboard; game area is rows 1 to 24.
static void draw_game(void) {
    char screen[25][81];
    int i, j;
    // Fill screen with spaces.
    for(i = 0; i < 25; i++) {
         for(j = 0; j < 80; j++)
              screen[i][j] = ' ';
         screen[i][80] = '\0';
    }
    // Draw scoreboard in row 0.
    // Format: P1: X   P2: Y
    {
         const char *label1 = "P1:";
         const char *label2 = "  P2:";
         for(j = 0; label1[j] && j < 10; j++)
              screen[0][j] = label1[j];
         screen[0][4] = ' ';
         screen[0][5] = '0' + score1;  // works for single-digit scores
         int pos = 7;
         for(j = 0; label2[j] && pos < 80; j++, pos++)
              screen[0][pos] = label2[j];
         if(score2 < 10)
              screen[0][pos] = '0' + score2;
         else
              screen[0][pos] = '9';
    }
    // Draw left paddle at column 2 (occupies rows p1_y-1, p1_y, p1_y+1)
    for (int offset = -1; offset <= 1; offset++) {
         int y = p1_y + offset;
         if (y >= 1 && y < 25)
              screen[y][2] = '|';
    }
    // Draw right paddle at column 77.
    for (int offset = -1; offset <= 1; offset++) {
         int y = p2_y + offset;
         if (y >= 1 && y < 25)
              screen[y][77] = '|';
    }
    // Draw the ball (only in game area, rows 1-24).
    if(ball_y >= 1 && ball_y < 25 && ball_x >= 0 && ball_x < 80)
         screen[ball_y][ball_x] = 'O';
    // Clear the screen and output each line.
    print_clear();
    for(i = 0; i < 25; i++) {
         print_str(screen[i]);
         print_str("\n");
    }
}

// Update the ball position.
// Horizontal movement is updated every frame,
// Vertical movement is updated only every 3 frames to slow its effect.
static void update_game(void) {
    static int frame_count = 0;
    frame_count++;
    ball_x += dx;
    if(frame_count % 3 == 0)
         ball_y += dy;

    // Bounce off top/bottom in game area (rows 1 to 24).
    if(ball_y < 1) { ball_y = 1; dy = -dy; }
    if(ball_y > 24) { ball_y = 24; dy = -dy; }

    // Left paddle collision at column 3.
    if(ball_x <= 3) {
         if(ball_y >= p1_y - 1 && ball_y <= p1_y + 1) {
              dx = -dx;
              ball_x = 4; // Nudge out of paddle
         }
    }
    // Right paddle collision at column 76.
    if(ball_x >= 76) {
         if(ball_y >= p2_y - 1 && ball_y <= p2_y + 1) {
              dx = -dx;
              ball_x = 75;
         }
    }
    // Out of bounds: left/right edges score.
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

// Main game loop.
static void pong_loop(void) {
    while(1) {
         process_input();
         update_game();
         draw_game();
         // Delay loop for timing adjustment.
         for(volatile int delay = 0; delay < 300000; delay++);
    }
}

void start_pong_game(void) {
    pong_loop();
}