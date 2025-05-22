#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 30
#define WIN_WIDTH 70
#define ROAD_WIDTH 50

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
int enemySpeed[3];

// More recognizable bike design
char bike[4][7] = {
    {' ', ' ', 'O', ' ', 'O', ' ', ' '},
    {' ', '/', '|', '_', '|', '\\', ' '},
    {'/', ' ', '|', '_', '|', ' ', '\\'},
    {'=', '=', 'O', ' ', 'O', '=', '='}
};

// Enhanced obstacle design
char obstacle[4][5] = {
    {'/', '=', '=', '=', '\\'},
    {'|', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', '|'},
    {'\\', '=', '=', '=', '/'}
};

int bikePos = WIN_WIDTH / 2 - 3; // Centered bike position
int score = 0;
int lives = 3;
int level = 1;
int speed = 50; // Base speed
bool gameRunning = true;

// Enhanced colors
enum COLOR {
    BLACK = 0,
    DARK_BLUE = 1,
    DARK_GREEN = 2,
    DARK_CYAN = 3,
    DARK_RED = 4,
    DARK_MAGENTA = 5,
    DARK_YELLOW = 6,
    GRAY = 7,
    DARK_GRAY = 8,
    BLUE = 9,
    GREEN = 10,
    CYAN = 11,
    RED = 12,
    MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

void SetColor(int foreground, int background = BLACK) {
    SetConsoleTextAttribute(console, foreground + (background * 16));
}

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
    if (size == 0)
        size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder() {
    SetColor(GREEN);
    
    // Left border
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        gotoxy(0, i); cout << "║";
        gotoxy(ROAD_WIDTH, i); cout << "║";
    }
    
    // Top and bottom borders
    for (int i = 0; i <= ROAD_WIDTH; i++) {
        gotoxy(i, 0); cout << "═";
        gotoxy(i, SCREEN_HEIGHT-1); cout << "═";
    }
    
    // Corners
    gotoxy(0, 0); cout << "╔";
    gotoxy(ROAD_WIDTH, 0); cout << "╗";
    gotoxy(0, SCREEN_HEIGHT-1); cout << "╚";
    gotoxy(ROAD_WIDTH, SCREEN_HEIGHT-1); cout << "╝";
    
    // Road divider
    for (int i = 1; i < SCREEN_HEIGHT-1; i++) {
        gotoxy(ROAD_WIDTH/2, i);
        if (i % 2 == 0) cout << "|";
        else cout << " ";
    }
    
    // Right info panel
    SetColor(CYAN);
    gotoxy(ROAD_WIDTH + 7, 2); cout << "BIKE RACING";
    gotoxy(ROAD_WIDTH + 5, 3); cout << "==============";
    
    SetColor(WHITE);
}

void genEnemy(int ind) {
    enemyX[ind] = 5 + rand() % (ROAD_WIDTH - 13);
    enemySpeed[ind] = 1 + rand() % 3; // Varying speeds for obstacles
}

void drawEnemy(int ind) {
    if (enemyFlag[ind]) {
        SetColor(RED);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 5; j++) {
                gotoxy(enemyX[ind] + j, enemyY[ind] + i);
                cout << obstacle[i][j];
            }
        }
    }
    SetColor(WHITE);
}

void eraseEnemy(int ind) {
    if (enemyFlag[ind]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 5; j++) {
                gotoxy(enemyX[ind] + j, enemyY[ind] + i); cout << " ";
            }
        }
    }
}

void resetEnemy(int ind) {
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

void drawBike() {
    SetColor(CYAN);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 7; j++) {
            gotoxy(j + bikePos, i + 22);
            cout << bike[i][j];
        }
    }
    SetColor(WHITE);
}

void eraseBike() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 7; j++) {
            gotoxy(j + bikePos, i + 22); cout << " ";
        }
    }
}

int collision() {
    for (int i = 0; i < 3; i++) {
        if (enemyFlag[i] && enemyY[i] + 4 >= 22) {
            if (enemyX[i] + 5 > bikePos && enemyX[i] < bikePos + 7) {
                return 1;
            }
        }
    }
    return 0;
}

void updateScore() {
    SetColor(YELLOW);
    gotoxy(ROAD_WIDTH + 7, 5); cout << "Score: " << score;
    gotoxy(ROAD_WIDTH + 7, 6); cout << "Lives: " << lives;
    gotoxy(ROAD_WIDTH + 7, 7); cout << "Level: " << level;
    SetColor(WHITE);
}

void gameover() {
    gameRunning = false;
    system("cls");
    SetColor(RED);
    cout << endl << endl;
    cout << "\t\t╔══════════════════════════╗" << endl;
    cout << "\t\t║        GAME OVER         ║" << endl;
    cout << "\t\t╚══════════════════════════╝" << endl << endl;
    
    SetColor(YELLOW);
    cout << "\t\t   Final Score: " << score << endl;
    cout << "\t\t   Level Reached: " << level << endl << endl;
    
    SetColor(GREEN);
    cout << "\t\tPress any key to return to menu...";
    SetColor(WHITE);
    getch();
}

void levelUp() {
    SetColor(GREEN);
    gotoxy(ROAD_WIDTH/4, SCREEN_HEIGHT/2);
    cout << "LEVEL UP! Speed Increased!";
    Sleep(1000);
    speed = max(10, speed - 5); // Increase game speed
    level++;
    
    // Clear the message
    gotoxy(ROAD_WIDTH/4, SCREEN_HEIGHT/2);
    cout << "                           ";
    SetColor(WHITE);
}

void instructions() {
    system("cls");
    SetColor(CYAN);
    cout << "\n\n\t\t╔═══════════════════════════════╗" << endl;
    cout << "\t\t║          INSTRUCTIONS          ║" << endl;
    cout << "\t\t╚═══════════════════════════════╝" << endl << endl;
    
    SetColor(WHITE);
    cout << "\t► Avoid obstacles by moving left or right." << endl;
    cout << "\t► Each obstacle you avoid gives you points." << endl;
    cout << "\t► You have 3 lives to start with." << endl;
    cout << "\t► The game gets faster as you level up." << endl << endl;
    
    SetColor(YELLOW);
    cout << "\t  CONTROLS:" << endl;
    cout << "\t  ▬▬▬▬▬▬▬▬▬" << endl;
    cout << "\t  'A' or '←' - Move Left" << endl;
    cout << "\t  'D' or '→' - Move Right" << endl;
    cout << "\t  'P' - Pause Game" << endl;
    cout << "\t  'ESC' - Exit to Menu" << endl << endl;
    
    SetColor(GREEN);
    cout << "\t  Press any key to return to menu...";
    SetColor(WHITE);
    getch();
}

void play() {
    bikePos = WIN_WIDTH / 2 - 3;
    score = 0;
    lives = 3;
    level = 1;
    speed = 50;
    gameRunning = true;
    
    // Initialize enemies
    for (int i = 0; i < 3; i++) {
        enemyFlag[i] = 0;
        enemyY[i] = 1;
        genEnemy(i);
    }
    
    enemyFlag[0] = 1; // Start with one enemy

    system("cls");
    drawBorder();
    updateScore();

    SetColor(CYAN);
    gotoxy(ROAD_WIDTH + 7, 12); cout << "CONTROLS";
    gotoxy(ROAD_WIDTH + 7, 13); cout << "▬▬▬▬▬▬▬▬";
    
    SetColor(WHITE);
    gotoxy(ROAD_WIDTH + 2, 14); cout << "'A' - Left";
    gotoxy(ROAD_WIDTH + 2, 15); cout << "'D' - Right";
    gotoxy(ROAD_WIDTH + 2, 16); cout << "'P' - Pause";
    gotoxy(ROAD_WIDTH + 2, 17); cout << "ESC - Exit";

    SetColor(YELLOW);
    gotoxy(18, 10); cout << "Press any key to start";
    SetColor(WHITE);
    getch();
    gotoxy(18, 10); cout << "                      ";

    while (gameRunning) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A' || ch == 75) { // 75 is left arrow
                if (bikePos > 2)
                    bikePos -= 2;
            }
            if (ch == 'd' || ch == 'D' || ch == 77) { // 77 is right arrow
                if (bikePos < ROAD_WIDTH - 9)
                    bikePos += 2;
            }
            if (ch == 'p' || ch == 'P') {
                SetColor(YELLOW);
                gotoxy(18, 10); cout << "GAME PAUSED - Press any key to continue";
                getch();
                gotoxy(18, 10); cout << "                                      ";
                SetColor(WHITE);
            }
            if (ch == 27) { // ESC
                break;
            }
        }

        drawBike();
        
        // Draw enemies
        for (int i = 0; i < 3; i++) {
            if (enemyFlag[i] == 1)
                drawEnemy(i);
        }

        if (collision()) {
            lives--;
            updateScore();
            
            // Flash bike to indicate collision
            for (int i = 0; i < 3; i++) {
                SetColor(RED);
                drawBike();
                Sleep(100);
                SetColor(WHITE);
                drawBike();
                Sleep(100);
            }
            
            if (lives <= 0) {
                gameover();
                return;
            }
            
            // Reset last enemy that hit the bike
            for (int i = 0; i < 3; i++) {
                if (enemyFlag[i] && enemyY[i] + 4 >= 22) {
                    if (enemyX[i] + 5 > bikePos && enemyX[i] < bikePos + 7) {
                        resetEnemy(i);
                        break;
                    }
                }
            }
        }

        Sleep(speed);
        eraseBike();
        
        // Update enemies
        for (int i = 0; i < 3; i++) {
            if (enemyFlag[i] == 1) {
                eraseEnemy(i);
                enemyY[i] += enemySpeed[i];
                
                if (enemyY[i] > SCREEN_HEIGHT - 6) {
                    resetEnemy(i);
                    score += 10 * enemySpeed[i]; // Higher speed = more points
                    updateScore();
                    
                    // Level up after every 100 points
                    if (score % 100 == 0) {
                        levelUp();
                        updateScore();
                    }
                    
                    // Add more enemies as score increases
                    if (score >= 50 && enemyFlag[1] == 0)
                        enemyFlag[1] = 1;
                    if (score >= 150 && enemyFlag[2] == 0)
                        enemyFlag[2] = 1;
                }
            }
        }
    }
}

int main() {
    setcursor(0, 0);
    srand((unsigned)time(NULL));

    // Set console size for better display
    system("mode con: cols=95 lines=35");
    system("title Bike Racing Game");

    while (1) {
        system("cls");
        SetColor(CYAN);
        gotoxy(10, 5);  cout << "╔═══════════════════════════════════════╗";
        gotoxy(10, 6);  cout << "║              BIKE RACING              ║";
        gotoxy(10, 7);  cout << "╚═══════════════════════════════════════╝";
        
        SetColor(WHITE);
        gotoxy(10, 9);  cout << "1. Start Game";
        gotoxy(10, 10); cout << "2. Instructions";
        gotoxy(10, 11); cout << "3. Quit";
        
        SetColor(YELLOW);
        gotoxy(10, 13); cout << "Select option: ";
        SetColor(WHITE);
        
        char op = _getch();

        if (op == '1') play();
        else if (op == '2') instructions();
        else if (op == '3') exit(0);
    }

    return 0;
}