/*
Note: In CodeBlocks: In Setting, Complier ,Linker Setting, "-lwinmm" Paste It In Linker Box And Press Ok
*/
#include <iostream>
#include "console.h"
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
//#pragma comment (lib, "winmm.lib")
using namespace std;
#define maxn 100
#define maxm 5
#define DE_WIDTH 100
#define DE_HEIGHT 30
#define WIDTH_ROW1 30
#define WIDTH_ROW2 70
#define WIDTH 40
#define HEIGHT 30

enum Status { UP, DOWN, LEFT, RIGHT};
int kun=1;
time_t start = time(NULL);
struct ToaDo {
    int x,y;
};
struct Snake {
    ToaDo dot[maxn];
    int n;
    Status status;
    int death;
    int score;
};

struct Apple {
    int x,y;
    int num;
};
struct PerSon {
    string name = "personname";
    int score = 0;
};

void SoundMove() {
    PlaySound(TEXT("Audio\\KeyMove.wav"), NULL, SND_ASYNC);
}
void SoundChoose() {
    PlaySound(TEXT("Audio\\Enter.wav"), NULL, SND_ASYNC);
}

string s[4] = { "Play", "High Score", "Huong Dan", "Close"  };
int a[4] = { DE_WIDTH/2-2, DE_WIDTH/2-5, DE_WIDTH/2-5, DE_WIDTH/2-2};
int b[4] = { HEIGHT/2-5, HEIGHT/2-3, HEIGHT/2-1, HEIGHT/2+1};
int c[4] = { 15, 15, 15, 15};

void SetConsole() {
    SetWindowSize(DE_WIDTH,DE_HEIGHT);
    DisableResizeWindow();
    DisableCtrButton(0,0,1);
    //ShowScrollbar(1);
    //DisableSelection();
    SetConsoleTitle("Snake Game !");
    //SetConsoleOutputCP(65020);
}
void ShowTime(int x,int y) {

    time_t tim;
    struct tm *info;
    char buffer[20];
    time(&tim);
    info =  localtime(&tim);
    strftime(buffer,20,"%X %d/%m/%Y",info);
    TextColor(15);
    gotoXY(x,y);
    cout<<buffer;
    //
    gotoXY(5,2);
    time_t tg  = time(&tim) - start;
    cout<<"Time Run: ";
    printf("%02d:%02d:%02d",tg/3600,tg/60,tg%60);
}
// ham lam nhay string
void Press(int color,int x,int y,string s,int sleep,int showt) {
    int k=0;
    string s2="";
    for (int i=0;i<s.size();i++) s2+=" ";
    while (1) {
        if (showt==1) ShowTime(WIDTH_ROW2+6,2);
        TextColor(color);
        gotoXY(x,y);
        if (k%2==1)  cout<<s;
        else cout<<s2;
        k++;
        Sleep(sleep);
        if (_kbhit()) {
            SoundChoose();
            while(!_getch());
            break;
        }
    }
}
//In logo
void ShowHome() {
    Sleep(200);
    //PlaySound(TEXT("Audio\\CountDown.wav"), NULL, SND_ASYNC);
    char c[255];
    for (int i=0;i<DE_HEIGHT;i++) {
        clrscr();
        int temp = i;
        int line = 0;
        fstream logo("Text\\LogoGame.txt");
        TextColor(8+rand()%8);
        if (i==DE_HEIGHT-1) {
            temp = 1;
            PlaySound(TEXT("Audio\\Trailer.wav"), NULL, SND_ASYNC);
            Sleep(1800);
            TextColor(14);
        }
        gotoXY(0,temp);
        while (logo && DE_HEIGHT-temp-1>line) {
            logo.getline(c,255);
            cout<<c<<endl;
            line++;
        }
        logo.close();
        Sleep(60+i);
    }
    Sleep(1000);
    Press(15,DE_WIDTH/2-11,DE_HEIGHT-3,"Press a key to continue",700,0);
}

//In menu
void ShowMenu() {
    clrscr();
    TextColor(12);
    gotoXY(DE_WIDTH/2-6,HEIGHT/2-8);
    cout<<">>> MENU <<<";
    //Hien thi "play"
    TextColor(10);
    gotoXY(a[0],b[0]);
    cout<<s[0];
    //Hien thi diem cao
    TextColor(c[1]);
    gotoXY(a[1],b[1]);
    cout<<s[1];
    //Hien thi huong dan
    TextColor(c[2]);
    gotoXY(a[2],b[2]);
    cout<<s[2];
    //Hien thi close
    TextColor(c[3]);
    gotoXY(a[3],b[3]);
    cout<<s[3];
}
// Lua chon menu
int SelectMenu() {
    //ShowCur(0);
    int choo=0, oldchoo;
    while (1) {
        oldchoo = choo;
        if (_kbhit()) {
            char key = _getch();
            if (key==-32) {
                char c = _getch();
                if (c == 72 ) {
                    SoundMove();
                    if (choo==0) choo = 3;
                    else choo--;
                }
                if (c == 80 ) {
                    SoundMove();
                    if (choo==3) choo = 0;
                    else choo++;
                }
            }
            if (key=='W' || key == 'w' ) {
                SoundMove();
                if (choo==0) choo = 3;
                else choo--;
            }
            if (key=='S' || key == 's' ){
                SoundMove();
                if (choo==3) choo = 0;
                else choo++;
            }
            gotoXY(a[oldchoo],b[oldchoo]);
            TextColor(c[oldchoo]);
            cout<<s[oldchoo];

            gotoXY(a[choo],b[choo]);
            TextColor(10);
            cout<<s[choo];
            if (key==13) {
                SoundChoose();
                return choo;
            }
        }
    }
}
// In huong dan
void ShowHD() {
    clrscr();
    TextColor(12);
    gotoXY(WIDTH_ROW1, 5);
    cout<<"SPACE : Pause, Continue";
    gotoXY(WIDTH_ROW1, 8);
    cout<<"Di Chuyen:";
    TextColor(13);
    gotoXY(DE_WIDTH/2-10, 10);
    cout<<"W";
    gotoXY(DE_WIDTH/2-14, 12);
    cout<<"A   S   D";
    gotoXY(DE_WIDTH/2+4, 10);
    cout<<"OR         Phim Mui Ten";
    Press(15,DE_WIDTH/2-12, HEIGHT-4,"Press a key to return",700,0);
}
// In diem cao
void ShowHighScore(PerSon person[]) {
    clrscr();
    /*
    fstream filescore("highscore.txt");
    for (int i=0;i<maxm;i++) {
        filescore>>person[i].name>>person[i].score;
    }
    filescore.close();
    */
    ShowTime(WIDTH_ROW2+6,2);
    TextColor(14);
    gotoXY(DE_WIDTH/2-8,5);
    cout<<"HIGHT SCORE";
    TextColor(15);
    gotoXY(WIDTH_ROW1+7,8);
    cout<<left<<setw(17)<<"Name"<<"Score";
    for (int i=0;i<maxm;i++) {
        gotoXY(WIDTH_ROW1+7,(i+5)*2);
        TextColor(15);
        if (i==0) TextColor(10);
        if (i==1) TextColor(12);
        if (i==2) TextColor(13);
        cout<<left<<setw(17)<<person[i].name<<person[i].score;
    }
    Press(15,DE_WIDTH/2-12, HEIGHT-4,"Press a key to return",700,1);
}
// Ham khoi tao
void Init(Snake &snake,Apple &apple,int &timesleep,PerSon person[]) {
    //Khoi tao snake
    for (int i=0;i<maxn;i++) {
        snake.dot[0].x = DE_WIDTH-2;
        snake.dot[1].y = DE_HEIGHT-2;
    }
    snake.n = 3;
    snake.dot[0].x = WIDTH_ROW1+3;
    snake.dot[0].y = 1;
    snake.dot[1].x = WIDTH_ROW1+2;
    snake.dot[1].y = 1;
    snake.dot[2].x = WIDTH_ROW1+1;
    snake.dot[2].y = 1;
    snake.status = RIGHT;
    snake.death = 0;
    snake.score = 0;
    //Khoi tao apple
    apple.x = DE_WIDTH/2;
    apple.y = DE_HEIGHT/2;
    apple.num = 0;
    //
    timesleep = 300;
    //Khoi tao person
    fstream filescore("Text\\highscore.txt");
    for (int i=0;i<maxm;i++) {
        filescore>>person[i].name>>person[i].score;
    }
    filescore.close();
}
// Ham hien thi man hinh choi
void ShowScr(Snake snake,Apple apple,PerSon person[]) {
    clrscr();
    //Hien thi tuong bao
    TextColor(15);
    for (int i=0;i<HEIGHT;i++) {
        gotoXY(WIDTH_ROW1,i);
        putchar(124);
        gotoXY(WIDTH_ROW2,i);
        putchar(124);
    }
    for (int i=0;i<DE_WIDTH;i++) {
        gotoXY(i,HEIGHT-1);
        putchar(196);
        gotoXY(i,0);
        putchar(196);
    }
    // Hiển thị điểm
    TextColor(10);
    gotoXY(WIDTH_ROW1-10,5);
    cout<<"SCORE";
    TextColor(10);
    gotoXY(WIDTH_ROW1-8,7);
    cout<<snake.score;
    //
    TextColor(13);
    gotoXY(11,HEIGHT/2-1);
    cout<<"-- GAME --";
    //Hien thi apple
    TextColor(15);
    gotoXY(apple.x,apple.y);
    putchar(169);
    // Hien thi diem cao
    TextColor(14);
    gotoXY(80,7);
    cout<<"HIGHT SCORE";
    TextColor(15);
    gotoXY(75,10);
    cout<<left<<setw(17)<<"Name"<<"Score";
    for (int i=0;i<maxm;i++) {
        gotoXY(75,(i+6)*2);
        TextColor(15);
        if (i==0) TextColor(10);
        if (i==1) TextColor(12);
        if (i==2) TextColor(13);
        cout<<left<<setw(17)<<person[i].name<<person[i].score;
    }
}

// Hien thi snake
void ShowSnake(Snake snake) {
    gotoXY(snake.dot[snake.n].x,snake.dot[snake.n].y);
    putchar(' ');
    for (int i=snake.n-1;i>=0;i--) {
        if (i==0) {
            TextColor(11);
            if (snake.n>23) {
                int co = 13;
                while (co==13) co = rand()%8 + 8;
                TextColor(co);
            }
        }
        else {
            TextColor(13);
        }
        gotoXY(snake.dot[i].x,snake.dot[i].y);
        putchar('@');
    }
}
// Hien thi apple
void ShowApple(Apple apple) {
    gotoXY(apple.x,apple.y);
    if (apple.num != 5) {
        TextColor(15);
        putchar(232);
    }
    else {
        TextColor(8+ rand()%8);
        putchar(233);
    }
}
// Ham di chuyen
void Move(Snake &snake,Apple apple) {
    for (int i=snake.n;i>0;i--) {
        snake.dot[i]=snake.dot[i-1];
    }
    if (snake.status == UP)
        snake.dot[0].y--;
    if (snake.status == DOWN)
        snake.dot[0].y++;
    if (snake.status == LEFT)
        snake.dot[0].x--;
    if (snake.status == RIGHT)
        snake.dot[0].x++;
}
// Ham dieu khien game
void ControlGame(Snake &snake,int &k) {
    if (_kbhit()) {
        char key = _getch();
        if (key==-32) {
            SoundMove();
            char c = _getch();
            if (c == 75 && snake.status!= RIGHT )
                snake.status = LEFT;
            if (c == 77 && snake.status!= LEFT )
                snake.status = RIGHT;
            if (c == 72 && snake.status!= DOWN )
                snake.status = UP;
            if (c == 80 && snake.status!= UP )
                snake.status = DOWN;
        }
        if ((key=='A' || key == 'a' ) && snake.status!= RIGHT )
            snake.status = LEFT;
        if ((key=='D' || key == 'd' ) && snake.status!= LEFT )
            snake.status = RIGHT;
        if ((key=='W' || key == 'w' ) && snake.status!= DOWN )
            snake.status = UP;
        if ((key=='S' || key == 's' ) && snake.status!= UP )
            snake.status = DOWN;
        if (key==32) {
            TextColor(12);
            gotoXY(12,HEIGHT/2+1);
            cout<<" PAUSE       ";
            gotoXY(DE_WIDTH-2,DE_HEIGHT-2);
            //while(getch()!=32);
            while (1) {
                ShowTime(WIDTH_ROW2+6,2);
                if (_kbhit() && getch()==32 ) break;
            }
        }
    }
    TextColor(10);
    gotoXY(12,HEIGHT/2+1);
    cout<<"RUNNING";
    if (k%4==0) cout<<"   ";
    if (k%4==1) cout<<".  ";
    if (k%4==2) cout<<".. ";
    if (k%4==3) cout<<"...";
}
// Ham xu ly an apple
void Proc(Snake &snake,Apple &apple,int &TimeSleep) {
    int check=0;
    for (int i=0;i<snake.n;i++)
        if (snake.dot[i].x==apple.x && snake.dot[i].y == apple.y) {
            PlaySound(TEXT("Audio\\SnakeEat.wav"), NULL, SND_ASYNC);
            check = 1;
            break;
        }
    if (check) {
        snake.n++;
        if (apple.num == 5) {
            snake.score+=30;
            apple.num = 0;
        }
        else {
            snake.score+=10;
            apple.num++;
        }
        TextColor(10);
        gotoXY(WIDTH_ROW1-9,7);
        cout<<snake.score;

        apple.x = WIDTH_ROW1 + 1 + rand() % (WIDTH-2);
        apple.y = 1 + rand() % (HEIGHT-2);
        //
        if (TimeSleep>=150)
            TimeSleep-=10;
    }
}
// Ham them nguoi diem cao
void AddHighScore(Snake &snake,PerSon person[]) {
    for (int i=0;i<maxm;i++) {
        //Neu la diem cao thi them vao danh sach
        if (snake.score>person[i].score) {
            for (int j=maxm-1;j>i;j--)
                person[j]=person[j-1];
            person[i].score = snake.score;
            TextColor(14);
            gotoXY(8,HEIGHT/2+3);
            cout<<"Name: ";
            fflush(stdin);
            cin>>person[i].name;
            fstream filescore("Text\\highscore.txt");
            for (int i=0;i<maxm;i++) {
                filescore<<left<<setw(15)<<person[i].name<<person[i].score<<endl;
            }
            filescore.close();
            ShowHighScore(person);
            clrscr();
            //while(!getch());
            break;
        }
    }

}
// Ham xu ly game over, play again
void GameOver(Snake &snake,PerSon person[],Apple &apple,int &timesleep,int &x) {
    for (int i=1;i<snake.n;i++) {
        if (snake.dot[0].x==snake.dot[i].x && snake.dot[0].y == snake.dot[i].y) {
            snake.death = 1;
            break;
        }
    }
    if  (snake.dot[0].x<=WIDTH_ROW1 || snake.dot[0].x >=WIDTH_ROW2 || snake.dot[0].y <=0 || snake.dot[0].y>=HEIGHT-1 )
        snake.death = 1;
    if (snake.death) {
        PlaySound(TEXT("Audio\\GameOver.wav"), NULL, SND_ASYNC);
        Press(12,11,HEIGHT/2+1,"GAME OVER!   ",500,1);
        TextColor(12);
        gotoXY(11,HEIGHT/2+1);
        cout<<"GAME OVER!   ";
        //Add diem cao
        AddHighScore(snake,person);

        //Choi lai or return menu
        for (int i=WIDTH_ROW1+1;i<WIDTH_ROW2;i++)
            for (int j=HEIGHT/2+1;j<HEIGHT-1;j++) {
                gotoXY(i,j);
                putchar(' ');
            }
        TextColor(10);
        gotoXY(WIDTH_ROW1+1,HEIGHT/2+6);
        cout<<"          Key 1 : Play Again          ";
        gotoXY(WIDTH_ROW1+1,HEIGHT/2+7);
        cout<<"                                      ";
        TextColor(12);
        gotoXY(WIDTH_ROW1+1,HEIGHT/2+8);
        cout<<"          Enter : Return Menu         ";
        while (1) {
            ShowTime(WIDTH_ROW2+6,2);
            if(kbhit()) {
                char key= getch();
                if (key == '1') {
                    SoundChoose();
                    Init(snake,apple,timesleep,person);
                    ShowScr(snake,apple,person);
                    return;
                }
                if (key == 13) {
                    SoundChoose();
                    while (1) {
                        ShowMenu();
                        int choo = SelectMenu();
                        if (choo==0) {
                            Init(snake,apple,timesleep,person);
                            ShowScr(snake,apple,person);
                            return;
                        }
                        if (choo==1) ShowHighScore(person);
                        if (choo==2) ShowHD();
                        if (choo==3) {
                            x = 0;
                            clrscr();
                            return;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    //PlaySound(TEXT("Audio\\BenTrenTangLau.wav"), NULL, SND_ASYNC);
    SetConsole();
    srand(time(NULL));
    int x=1;
    Snake snake;
    Apple apple;
    PerSon person[maxm];
    int TimeSleep;
    ShowCur(0);
    Init(snake,apple,TimeSleep,person);
    ShowHome();
    while (1) {
        ShowMenu();
        int choo = SelectMenu();
        if (choo==0) break;
        if (choo==1) ShowHighScore(person);
        if (choo==2) ShowHD();
        if (choo==3) {
            clrscr();
            return 0;
        }
    }
    ShowScr(snake,apple,person);
    while (x) {
        ShowTime(WIDTH_ROW2+6,2);
        ShowSnake(snake);           //Hien thi snake lan 1

        ShowApple(apple);           //Hien thi apple

        ControlGame(snake,x);       //ControlGame

        Move(snake,apple);           //Di chuyen

        ShowSnake(snake);           //Hien thi snake lan 2

        Proc(snake,apple,TimeSleep);     //Xu ly
        //ShowSnake(snake);

        ShowApple(apple);           //Hien thi apple
        // Game over, Play again
        x++;
        GameOver(snake,person,apple,TimeSleep,x);
        Sleep(TimeSleep);
        ShowCur(0);
    }
    return 0;
}
