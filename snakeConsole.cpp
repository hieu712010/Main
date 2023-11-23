#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>

#define KEY_NONE

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPostion;

int whereX(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.dwCursorPosition.X;
    return -1;
}
int whereY(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.dwCursorPosition.Y;
    return -1;
}
void gotoxy(int x, int y){
    CursorPostion.X = x;
    CursorPostion.Y = y;
    SetConsoleCursorPosition(console, CursorPostion);
}
//Dat mau cho chu
void SetColor(WORD color){
    SetConsoleTextAttribute(console, color);
}
//An con tro chuot
void SetCursor(bool visible, DWORD size){
    if(size == 0)
        size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}   
//Tra ve ma phim nguoi dung bam
int inputKey(){
    if(_kbhit()){
        int key = _getch();
        if(key == 244){
            key = _getch();
            return key + 1000;
        }
        return key;
    }
}

using namespace std;

#define MAX 100

int doDai = 4, score = 0;

void drawWallUp();
void drawWallDown();
void drawWallLeft();
void drawWallRight();
void drawWall();
void createSnake(int toaDoX[], int toaDoY[]);
void delDuLieuCu(int toaDoX[], int toaDoY[]);
void veRan(int toaDoX[], int toaDoY[]);
void xuLi(int toaDoX[], int toaDoY[], int x, int y, int &xf, int &yf);
void add(int a[], int x);
void del(int a[], int vt);
bool checkLoseWall(int x0, int y0);
bool checkLoseBody(int toaDoX[], int toaDoY[]);
bool checkLose(int toaDoX[], int toaDoY[]);
void Food(int &xf, int &yf, int toaDoX[], int toaDoY[]);
bool checkFood(int xf, int yf,int toaDoX[], int toaDoY[]);
bool checkBoost(int xb, int yb, int xf, int yf, int toaDoX[], int toaDoY[]);
bool checkEatFood(int xf, int yf, int x0, int y0);
void hd();
void doKho(int &dokho);

int main(){
    //SetConsoleOutputCP(65001);
    //system("cls");
    printf("\033[H\033[J"); 
    SetCursor(0, 0);
    srand(time(NULL));
    bool running = false;
    int toaDoX[MAX], toaDoY[MAX], x = 50, y = 13, check = 2, xf = 0, yf = 0, dokho = 300;
    
    while(1){
        SetColor(FOREGROUND_GREEN);
        gotoxy(70, 13);  cout << "|------------------|";
        gotoxy(70, 14);  cout << "|    SNAKE GAME    |";
        gotoxy(70, 15);  cout << "|------------------|";
        gotoxy(70, 16);  cout << "1.Bat dau choi";
        gotoxy(70, 17);  cout << "2.Huong dan choi";
        gotoxy(70, 18);  cout << "3.Chon do kho";
        gotoxy(70, 19);  cout << "4.Thoat";
        gotoxy(70, 20);  cout << "Nhap lua chon";   
        if(_kbhit()){
            char kitu = _getch();
            if(kitu == '1'){
                running = true;
                printf("\033[H\033[J"); 
                drawWall();
                createSnake(toaDoX, toaDoY);
                veRan(toaDoX, toaDoY); 
                Food(xf, yf, toaDoX, toaDoY);
            }else if(kitu == '2')  hd();
            else if(kitu == '3') doKho(dokho);
            else if(kitu == '4'){printf("\033[H\033[J");exit(0);}
        }
        while(running){
            delDuLieuCu(toaDoX, toaDoY);
            if(_kbhit()){
                int kitu = _getch();
                if     (kitu == 'w' || kitu == 72 && check != 0)     check = 1;
                else if(kitu == 'a' || kitu == 75 && check != 2)     check = 3;
                else if(kitu == 's' || kitu == 80 && check != 1)     check = 0;
                else if(kitu == 'd' || kitu == 77 && check != 3)     check = 2;
            }
            if      (check == 0)    y++;
            else if (check == 1)    y--;
            else if (check == 2)    x++;
            else if (check == 3)    x--; 
            xuLi(toaDoX, toaDoY, x, y, xf, yf);
            gotoxy(10, 27);
            cout << "Diem hien tai: " << score;
            if(checkLose(toaDoX, toaDoY)){
                printf("\033[H\033[J");
                gotoxy(50, 13);
                cout << "Ban Thua clm";
                gotoxy(46, 14);
                cout << "Diem cua ban la: " << score;
                gotoxy(41, 15);
                cout << "Nhap phim bat ki de quay lai menu";
                _getch();
                printf("\033[H\033[J"); 
                running = false;
                x = 50, y = 13, check = 2;
            }
            Sleep(dokho);
        }
    }
    _getch();
    return 0;
}
void drawWallUp(){
    int x = 10, y = 1;
    while(x <= 100){
        gotoxy(x, y);
        cout << "-";
        x++;
    }
}
void drawWallDown(){
    int x = 10, y = 26;
    while(x <= 100){
        gotoxy(x, y);
        cout << "_";
        x++;
    }
}
void drawWallLeft(){
    int x = 10, y = 1;
    while(y <= 26){
        gotoxy(x, y);
        cout << "|";
        y++;
    }
}
void drawWallRight(){
    int x = 100, y = 1;
    while(y <= 26){
        gotoxy(x, y);
        cout << "|";
        y++;
    }
}
void drawWall(){
    SetColor(FOREGROUND_RED);
    drawWallUp();
    drawWallDown();
    drawWallLeft();
    drawWallRight();
}
void createSnake(int toaDoX[], int toaDoY[]){
    int x = 50, y = 13;
    for(int i = 0; i < doDai; i++){
        toaDoX[i] = x; toaDoY[i] = y;
        x--;
    }
}
void veRan(int toaDoX[], int toaDoY[]){
    SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN);
    for(int i = 0; i < doDai; i++){
        gotoxy(toaDoX[i], toaDoY[i]);
        if(i == 0)
            cout << (char)2;
        else
            cout << "#";
    }
}
void delDuLieuCu(int toaDoX[], int toaDoY[]){
    for(int i = 0; i < doDai; i++){
        gotoxy(toaDoX[i], toaDoY[i]);
        cout << " ";
    }
}
void add(int a[], int x){
    for(int i = doDai; i > 0; i--)
        a[i] = a[i - 1];
    a[0] = x;
    doDai++;
}
void del(int a[], int vt){
    for(int i = vt; i < doDai; i++)
        a[i] = a[i + 1];
    doDai--;
}
bool checkLoseWall(int x0, int y0){
    if(y0 == 1 && (x0 >= 10  && x0 <= 100))
        return true;
    else if(y0 == 26 && (x0 >= 10  && x0 <= 100))
        return true;
    if(x0 == 100 && (y0 >= 1  && y0 <= 26))
        return true;
    if(x0 == 10 && (y0 >= 1  && y0 <= 26))
        return true;
    return false;
}
bool checkLoseBody(int toaDoX[], int toaDoY[]){
    for(int i = 1; i < doDai; i++){
        if((toaDoX[0] == toaDoX[i]) && (toaDoY[0] == toaDoY[i]))
            return true;
    }
    return false;
}
bool checkLose(int toaDoX[], int toaDoY[]){
    bool c1 = checkLoseWall(toaDoX[0], toaDoY[0]), c2 = checkLoseBody(toaDoX, toaDoY);
    if(c1 || c2)
        return true; 
    return false;
}
void Food(int &xf, int &yf, int toaDoX[], int toaDoY[]){
    do{
        xf = rand() % 89 + 11;
        yf = rand() % 24 + 2;
    }while(checkFood(xf, yf, toaDoX, toaDoY));
    int i = rand() % 15 + 1;
    SetColor(i);
    gotoxy(xf, yf);
    cout << "*";
}
void Boost(int &xb, int &yb, int &xf, int &yf, int toaDoX[], int toaDoY[]){
    do{
        xb = rand() % 89 + 11;
        yb = rand() % 24 + 2;
    }while(checkBoost(xb, yb, xf, yf, toaDoX, toaDoY));
    SetColor(FOREGROUND_RED);
    gotoxy(xb,yb);
    cout << "O";
}
bool checkBoost(int xb, int yb, int xf, int yf, int toaDoX[], int toaDoY[]){
    for(int i = 0; i < doDai; i++)
        if((xb == xf && yb == yf) || (xb == toaDoX[i]) && (yb == toaDoY[i]))
            return true;
    return false;
}
bool checkFood(int xf, int yf,int toaDoX[], int toaDoY[]){
    for(int i = 0; i < doDai; i++)
        if((xf == toaDoX[i]) && (yf == toaDoY[i]))
            return true;
    return false;
}
bool checkEatFood(int xf, int yf, int x0, int y0){
    if(x0 == xf && y0 == yf)
        return true;
    return false;
} 
void xuLi(int toaDoX[], int toaDoY[], int x, int y, int &xf, int &yf){
    add(toaDoX, x);
    add(toaDoY, y);
    if((checkEatFood(xf, yf, toaDoX[0], toaDoY[0])) == false){
        del(toaDoX, doDai - 1);
        del(toaDoY, doDai - 1);
    }
    else{
        score++;
        del(toaDoY, doDai - 1);
        Food(xf, yf, toaDoX, toaDoY);
    }
    veRan(toaDoX, toaDoY);
}
void doKho(int &dokho){
    printf("\033[H\033[J"); 
        SetColor(14);
        gotoxy(70, 16); cout << "1.De";
        gotoxy(70, 17); cout << "2.Trung Binh";
        gotoxy(70, 18); cout << "3.Kho";
        gotoxy(70, 19); cout << "4.Ve menu";
        if(_kbhit){
            char s = _getch();
                if(s == '1'){  
                    printf("\033[H\033[J"); 
                    dokho = 300;
                    gotoxy(70, 18);  cout << "Da chon De";
                    gotoxy(70, 19);  cout << "Ve menu sau 3s";
                    Sleep(3000);
                    gotoxy(70, 19);  cout << "              ";
                }else if(s == '2'){  
                    printf("\033[H\033[J"); 
                    dokho = 200;
                    gotoxy(70, 18);  cout << "Da chon Trung Binh";
                    gotoxy(70, 19);  cout << "Ve menu sau 3s";
                    Sleep(3000);
                    gotoxy(70, 18);  cout << "                  ";
                    gotoxy(70, 19);  cout << "                  ";
                }else if(s == '3'){  
                    printf("\033[H\033[J"); 
                    dokho = 50;
                    gotoxy(70, 18);  cout << "Da chon Kho";
                    gotoxy(70, 19);  cout << "Ve menu sau 3s";
                    Sleep(3000);
                    gotoxy(70, 19);  cout << "              ";
                }else
                    printf("\033[H\033[J"); 
        }
}
void hd(){
    printf("\033[H\033[J"); 
    SetColor(13);
    gotoxy(70, 13); cout << "'A' - Sang trai";
    gotoxy(70, 14); cout << "'D' - Sang phai";
    gotoxy(70, 15); cout << "'W' - Len tren";
    gotoxy(70, 16); cout << "'S' - Xuong duoi";
    gotoxy(68, 17); cout << "An thuc an de tang diem";
    gotoxy(60, 18); cout << "Cham vao tuong hoac can vao nguoi la die";
    gotoxy(66, 19); cout << "Bam phim bat ki quay lai menu";
    _getch();
    printf("\033[H\033[J");
}


