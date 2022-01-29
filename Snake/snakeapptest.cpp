// snakeapptest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <deque>
#include <Windows.h>
#include <math.h>
#include <ctime>
#include <conio.h>
#include <map>
#include <future>

/* Black */
#define BACKGROUNDCOLOR 0
#define RED 4 + BACKGROUNDCOLOR     /* Red */
#define GREEN 2 + BACKGROUNDCOLOR   /* Green */
#define YELLOW 6 + BACKGROUNDCOLOR  /* Yellow */
#define BLUE 1 + BACKGROUNDCOLOR    /* Blue */
#define MAGENTA 5 + BACKGROUNDCOLOR /* Magenta */
#define CYAN 3 + BACKGROUNDCOLOR    /* Cyan */
#define WHITE 7 + BACKGROUNDCOLOR   /* white */
#define BIGHTYELLOW 14 + BACKGROUNDCOLOR
#define BIGHTRED 12 + BACKGROUNDCOLOR

#define WHITENOBACK 7 /* white */
#define TROPHYCOLOR WHITE
#pragma warning(disable : 4996)
#define MAPWIDTH 16
#define MAPHEIGHT 16

using namespace std;



class Position
{
private:
    int x, y;

public:
    Position()
    {
        Position(0, 0);
    }
    Position(int x, int y) : x(x), y(y) {}
    void SetX(int X)
    {
        x = X;
    }
    void SetY(int Y)
    {
        y = Y;
    }
    int GetX()
    {
        return x;
    }
    int GetY()
    {
        return y;
    }
};
class Trophy
{
private:
    Position position;

public:
    int color = TROPHYCOLOR;
    Trophy()
    {
    }

    void setPosition(Position a)
    {
        position = a;
    }

    Position GetPosition()
    {
        return position;
    }
};
class Snake
{
private:
    int length;
    deque<Position> partsPosition;
    int direction = 5;
    map<char, int> MP;
    future<char> future;
    pair< Position, Position> headandtailposition;
public:

    Snake()
    {
        MP.insert({ 'w', 8 });
        MP.insert({ 'a', 4 });
        MP.insert({ 's', 2 });
        MP.insert({ 'd', 6 });

        length = 5;
        for (int i = 1; i <= length; i++)
        {
            partsPosition.push_back(Position(i, 1));
        }
        future = async(launch::async, getAnswer);

    }
    deque<Position>* GetPartsPosition()
    {
        return &partsPosition;
    }

    int GetLength()
    {
        return length;
    }

    void IncrementLength()
    {
        length++;
    }

    void move(int direction)
    {
        switch (direction)
        {
        case 8:
        {
            partsPosition.pop_front();
            int newpos = partsPosition.at(length - 2).GetY() - 1;
            if (newpos == 0)
                newpos = MAPHEIGHT - 2;
            partsPosition.push_back(Position(partsPosition.at(length - 2).GetX(), newpos));
            break;
        }
        case 2:
        {
            partsPosition.pop_front();
            int newpos = partsPosition.at(length - 2).GetY() + 1;
            if (newpos == MAPHEIGHT - 1)
                newpos = 1;
            partsPosition.push_back(Position(partsPosition.at(length - 2).GetX(), newpos));
            break;
        }
        case 6:
        {
            partsPosition.pop_front();
            int newpos = partsPosition.at(length - 2).GetX() + 1;
            if (newpos == MAPWIDTH - 1)
                newpos = 1;
            partsPosition.push_back(Position(newpos, partsPosition.at(length - 2).GetY()));
            break;
        }
        case 4:
        {
            partsPosition.pop_front();
            int newpos = partsPosition.at(length - 2).GetX() - 1;
            if (newpos == 0)
                newpos = MAPWIDTH - 2;
            partsPosition.push_back(Position(newpos, partsPosition.at(length - 2).GetY()));
            break;
        }
        case 5:
        {
            cout << "Pres 2, 4, 6, or 8 to start." << endl;
            break;
        }
        default:
            break;
        }
    }

    char getdirection(char prevchar)
    {
        // cout << "Enter direction (W,A,S,D) :  ";

        char x = prevchar;



        chrono::milliseconds timeout(200);
        if (future.wait_for(timeout) == future_status::ready)
        {
            x = future.get();
            future = async(launch::async, getAnswer);
        }

        x = tolower(x);
        cout << "\n";
        move(MP[x]);
        return x;
    }

    static char getAnswer()
    {
        char answer;
        answer = getch();
        return answer;
    }
};
class ColoerdChar
{
public:
    char Char;
    int color = BIGHTYELLOW;
};
class Map
{
private:

    Snake A;
    Trophy trophy;
    bool trophyexist = false;
    bool trophyappear = false;

    int score = 0;
    ColoerdChar mapmatrix[MAPHEIGHT][MAPWIDTH];
    vector<Position> Emptyplaces;
    char prevdirection='q';
    void GenerateTrophy(vector<Position> Emptyplaces)
    {
        if (Emptyplaces.size() == 0)
        {
            HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
            Won = true;
            throw range_error("YOU WON!!");
            return;
        }
        else
        {
            int pos = rand() % Emptyplaces.size();
            Position trophypos = Emptyplaces.at(pos);
            trophy.setPosition(trophypos);
        }
    }
   
    COORD PositionToCoord(Position B) {
        COORD A;
        A.X = B.GetX();
        A.Y = B.GetY();
        return A;
    }
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE); 


public:
    bool Won = false;
    Map() {};

    void printmap()
    {

        for (int i = 0; i < MAPHEIGHT; i++)
        {
            for (int j = 0; j < MAPWIDTH; j++)
            {
                mapmatrix[i][j].Char = ' ';
            }
        }
        for (int i = 0; i < MAPHEIGHT; i++)
        {
            mapmatrix[i][0].Char = 219;
            mapmatrix[i][0].color = RED;
            mapmatrix[i][MAPWIDTH - 1].Char = 219;
            mapmatrix[i][MAPWIDTH - 1].color = RED;
        }
        for (int j = 0; j < MAPWIDTH; j++)
        {
            mapmatrix[0][j].Char = 219;
            mapmatrix[0][j].color = RED;
            mapmatrix[MAPHEIGHT - 1][j].Char = 219;
            mapmatrix[MAPHEIGHT - 1][j].color = RED;
        }
        mapmatrix[MAPHEIGHT - 1][0].Char = 219;
        mapmatrix[MAPHEIGHT - 1][MAPWIDTH - 1].Char = 219;

        mapmatrix[MAPHEIGHT - 1][0].color = RED;
        mapmatrix[MAPHEIGHT - 1][MAPWIDTH - 1].color = RED;

        HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE); //just once


        deque<Position>* R = A.GetPartsPosition();

        char dafualt = 219;
        //223 top //220bottom" //219 vertical
        // i==0 tail

        Position h = R->at(R->size() - 1);
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;

        for (int i = 0; i < R->size(); i++)
        {
            Position e = R->at(i);
            //2486
            mapmatrix[e.GetY()][e.GetX()].Char = dafualt;
            mapmatrix[e.GetY()][e.GetX()].color = BIGHTYELLOW;
        }
        mapmatrix[h.GetY()][h.GetX()].color = BIGHTRED;
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;





        for (int i = 0; i < MAPHEIGHT; i++)
        {
            for (int j = 0; j < MAPWIDTH; j++)
            {
                SetConsoleTextAttribute(color, mapmatrix[i][j].color);

                cout << mapmatrix[i][j].Char;
                SetConsoleTextAttribute(color, 7);
            }
            cout << "\n";
        }
        cout << "Score: " << score << endl;


    }
    void changemap() {

        deque<Position>* R = A.GetPartsPosition();
        Position oldtail = R->at(0);

        prevdirection = A.getdirection(prevdirection);

        char dafualt = 219;
        //223 top //220bottom" //219 vertical
        // i==0 tail

        Position h = R->at(R->size() - 1);
        if(prevdirection =='w' || prevdirection == 'a' || prevdirection == 's' || prevdirection == 'd'){
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;
        mapmatrix[oldtail.GetY()][oldtail.GetX()].Char = ' ';
        mapmatrix[oldtail.GetY()][oldtail.GetX()].color = BACKGROUNDCOLOR;
        }
        mapmatrix[h.GetY()][h.GetX()].color = BIGHTRED;
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;

        if (!trophyexist)
        {
            for (int i = 0; i < MAPHEIGHT; i++)
            {
                for (int j = 0; j < MAPWIDTH; j++)
                {
                    if (mapmatrix[i][j].Char == ' ')
                    {
                        Emptyplaces.push_back(Position(j, i));
                    }
                }
            }
            GenerateTrophy(Emptyplaces);
            Emptyplaces.clear();
            trophyexist = true;
        }




        //nesscisert changes to map instead of reprinting it
        Position beforehead = R->at(R->size() - 2);
        //2486
        mapmatrix[beforehead.GetY()][beforehead.GetX()].color = BIGHTYELLOW;
        SetConsoleTextAttribute(color, mapmatrix[beforehead.GetY()][beforehead.GetX()].color);
        SetConsoleCursorPosition(color, PositionToCoord(beforehead));
        cout << mapmatrix[beforehead.GetY()][beforehead.GetX()].Char;


        mapmatrix[h.GetY()][h.GetX()].color = BIGHTRED;
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;



        SetConsoleTextAttribute(color, mapmatrix[oldtail.GetY()][oldtail.GetX()].color);
        SetConsoleCursorPosition(color, PositionToCoord(oldtail));
        cout << mapmatrix[oldtail.GetY()][oldtail.GetX()].Char;

        if (!trophyappear) {

            mapmatrix[trophy.GetPosition().GetY()][trophy.GetPosition().GetX()]
                .Char = 233;
            mapmatrix[trophy.GetPosition().GetY()][trophy.GetPosition().GetX()]
                .color = TROPHYCOLOR;
            SetConsoleTextAttribute(color, TROPHYCOLOR);
            SetConsoleCursorPosition(color, PositionToCoord(trophy.GetPosition()));
            cout << mapmatrix[trophy.GetPosition().GetY()]
                [trophy.GetPosition().GetX()].Char;


        }
        mapmatrix[h.GetY()][h.GetX()].color = BIGHTRED;
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;
        SetConsoleTextAttribute(color, mapmatrix[h.GetY()][h.GetX()].color);
        SetConsoleCursorPosition(color, PositionToCoord(h));
        cout << mapmatrix[h.GetY()][h.GetX()].Char;



        SetConsoleTextAttribute(color, WHITENOBACK);
        SetConsoleCursorPosition(color, PositionToCoord(Position(0, MAPHEIGHT)));

        cout << "Score: " << score << endl;
        cout << "Enter direction (W,A,S,D) :  ";
        int len = A.GetLength();
        Position head = R->at(len - 1);
        for (int i = 0; i < len - 1; i++)
        {
            Position e = R->at(i);
            if (e.GetY() == head.GetY() && e.GetX() == head.GetX())
            {
                throw invalid_argument("Game Over");
            }
        }
        if (trophy.GetPosition().GetX() == head.GetX() &&
            trophy.GetPosition().GetY() == head.GetY())
        {
            trophyexist = false;
            trophyappear = false;
            A.IncrementLength();
            score++;

            R->push_front(Position(R->at(0).GetX(), R->at(0).GetY()));
        }

    }

    void moveAutomatically(int direction) {
        deque<Position>* R = A.GetPartsPosition();
        Position oldtail = R->at(0);

        A.move(direction);

        char dafualt = 219;
        //223 top //220bottom" //219 vertical
        // i==0 tail

        Position h = R->at(R->size() - 1);

        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;
        mapmatrix[oldtail.GetY()][oldtail.GetX()].Char = ' ';
        mapmatrix[oldtail.GetY()][oldtail.GetX()].color = BACKGROUNDCOLOR;
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;


        if (!trophyexist)
        {
            for (int i = 0; i < MAPHEIGHT; i++)
            {
                for (int j = 0; j < MAPWIDTH; j++)
                {
                    if (mapmatrix[i][j].Char == ' ')
                    {
                        Emptyplaces.push_back(Position(j, i));
                    }
                }
            }
            GenerateTrophy(Emptyplaces);
            Emptyplaces.clear();
            trophyexist = true;
        }



        Position beforehead = R->at(R->size() - 2);

        //nesscisert changes to map instead of reprinting it
        mapmatrix[beforehead.GetY()][beforehead.GetX()].color = BIGHTYELLOW;
        SetConsoleTextAttribute(color, mapmatrix[beforehead.GetY()][beforehead.GetX()].color);
        SetConsoleCursorPosition(color, PositionToCoord(beforehead));
        cout << mapmatrix[beforehead.GetY()][beforehead.GetX()].Char;
        mapmatrix[h.GetY()][h.GetX()].color = BIGHTRED;
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;



        SetConsoleTextAttribute(color, mapmatrix[oldtail.GetY()][oldtail.GetX()].color);
        SetConsoleCursorPosition(color, PositionToCoord(oldtail));
        cout << mapmatrix[oldtail.GetY()][oldtail.GetX()].Char;

        if (!trophyappear) {

            mapmatrix[trophy.GetPosition().GetY()][trophy.GetPosition().GetX()]
                .Char = 233;
            mapmatrix[trophy.GetPosition().GetY()][trophy.GetPosition().GetX()]
                .color = TROPHYCOLOR;
            SetConsoleTextAttribute(color, TROPHYCOLOR);
            SetConsoleCursorPosition(color, PositionToCoord(trophy.GetPosition()));
            cout << mapmatrix[trophy.GetPosition().GetY()]
                [trophy.GetPosition().GetX()].Char;


        }
        mapmatrix[h.GetY()][h.GetX()].color = BIGHTRED;
        mapmatrix[h.GetY()][h.GetX()].Char = dafualt;
        SetConsoleTextAttribute(color, mapmatrix[h.GetY()][h.GetX()].color);
        SetConsoleCursorPosition(color, PositionToCoord(h));
        cout << mapmatrix[h.GetY()][h.GetX()].Char;



        SetConsoleTextAttribute(color, WHITENOBACK);
        SetConsoleCursorPosition(color, PositionToCoord(Position(0, MAPHEIGHT)));

        cout << "Score: " << score << endl;
        cout << "Enter direction (W,A,S,D) :  ";
        int len = A.GetLength();
        Position head = R->at(len - 1);
        for (int i = 0; i < len - 1; i++)
        {
            Position e = R->at(i);
            if (e.GetY() == head.GetY() && e.GetX() == head.GetX())
            {
                throw invalid_argument("Game Over");
            }
        }
        if (trophy.GetPosition().GetX() == head.GetX() &&
            trophy.GetPosition().GetY() == head.GetY())
        {
            trophyexist = false;
            trophyappear = false;
            A.IncrementLength();
            score++;
            R->push_front(Position(R->at(0).GetX(), R->at(0).GetY()));
        }
    }
    Snake* GetSnake()
    {
        return &A;
    }
};



int main()
{

    srand(time(0));

    HWND A = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(A, &ConsoleRect);

    MoveWindow(A, 0, 0, 500, 600, 1);

    COORD D;
    COORD* L = new COORD();
    D.X = 20;
    D.Y = 20;

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    //SetConsoleDisplayMode(console, CONSOLE_FULLSCREEN_MODE, L);

    PCONSOLE_FONT_INFOEX a = new CONSOLE_FONT_INFOEX;

    a->cbSize = sizeof(CONSOLE_FONT_INFOEX);
    a->dwFontSize = D;
    a->FontWeight = 100;
    a->FontFamily = TMPF_VECTOR;
    SetCurrentConsoleFontEx(console, false, a);

    cout << "Welcome.   1)automatic -  2) maniual\n";

    Sleep(500);
    char x;
    x = getch();
    COORD corrd;
    corrd.X = 0;
    corrd.Y = 0;

    SetConsoleCursorPosition(console, corrd);

    cout << "                                                          ";
    SetConsoleCursorPosition(console, corrd);


    //cout << "For better experince, maximize the window size.\n";
    //Sleep(500);
    Map map;

    map.printmap();

    if (x == '1')
    {

        SetConsoleCursorPosition(console, corrd);

        //solve automatic
        for (int i = 5; i <= MAPWIDTH - 2; i++)
        {
            SetConsoleCursorPosition(console, corrd);

            map.moveAutomatically(6);

        }
        int length = map.GetSnake()->GetLength();
        int max = (MAPWIDTH - 2) * (MAPHEIGHT - 3) ;
        while (length < max)
        {
            length = map.GetSnake()->GetLength();
            //cout << " length:  " << length << "/" << max<<endl;
            for (int i = 1; i < MAPWIDTH - 2; i++)
            {
                map.moveAutomatically(6);

            }
            map.moveAutomatically(2);

            for (int i = 1; i < MAPWIDTH - 2; i++)
            {
                map.moveAutomatically(4);

            }

            map.moveAutomatically(2);

        }
    }

    COORD cong;
    cong.X = 5;
    cong.Y = MAPHEIGHT + 5;
    char prevchar = 'd';
    SetConsoleCursorPosition(console, corrd);


    while (!map.Won)
    {

        try
        {
            if (!map.Won)
            {
                map.changemap();
            }
        }
        catch (invalid_argument& e)
        {

            SetConsoleTextAttribute(console, RED - BACKGROUNDCOLOR);
            COORD cong;
            cong.X = 5;
            cong.Y = MAPHEIGHT + 5;

            SetConsoleCursorPosition(console, cong);

            for (int i = 0; i < 3; i++)
            {
                SetConsoleCursorPosition(console, cong);
                cout << e.what();
                Sleep(700);
                SetConsoleCursorPosition(console, cong);
                cout << "         ";
                Sleep(700);
            }
            SetConsoleTextAttribute(console, WHITENOBACK);
            break;
        }
        catch (range_error& e)
        {

            SetConsoleTextAttribute(console, GREEN - BACKGROUNDCOLOR);

            SetConsoleCursorPosition(console, cong);

            for (int i = 0; i < 3; i++)
            {
                SetConsoleCursorPosition(console, cong);
                cout << e.what();
                Sleep(700);
                SetConsoleCursorPosition(console, cong);
                cout << "         ";
                Sleep(700);
            }
            break;
        }
    }

    SetConsoleTextAttribute(console, WHITENOBACK);
    cong.X = 0;
    SetConsoleCursorPosition(console, cong);
    system("pause");
}
