#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
//#include <conio.h>
#include "conio.h"  //for mac
#include <curses.h> //for mac

#define FIELD_HEIGHT 21 //枠を含む
#define FIELD_WIDTH 12  //枠を含む

#define MINO_HEIGHT 4
#define MINO_WIDTH 4

#define DEFAULT_POS_X 5
#define DEFAULT_POS_Y (-MINO_HEIGHT + 1)

#define PLAYER_NAME_SIZE 1024

enum
{
    MINO_ANGLE_0,
    MINO_ANGLE_90,
    MINO_ANGLE_180,
    MINO_ANGLE_270,
    MINO_ANGLE_MAX
};

enum
{
    MINO_TYPE_I,
    MINO_TYPE_O,
    MINO_TYPE_S,
    MINO_TYPE_Z,
    MINO_TYPE_J,
    MINO_TYPE_L,
    MINO_TYPE_T,
    MINO_TYPE_MAX
};

int nextMinoX = DEFAULT_POS_X;
int nextMinoY = DEFAULT_POS_Y;
int nextMinoAngle = 0;
int nextMinoType = 0;

int minoX = 0;
int minoY = 0;
int minoAngle = 0;
int minoType = 0;

int score = 0;
char playerName[PLAYER_NAME_SIZE] = "player";

int field[FIELD_WIDTH][FIELD_HEIGHT];
int displayBuffer[FIELD_WIDTH][FIELD_HEIGHT] = {};

char minoShapes[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_HEIGHT][MINO_WIDTH] =
    {
        //MINO_TYPE_I
        {
            //MINO_ANGLE_0
            {
                0, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 0, 0},
            //MINO_ANGLE_90
            {
                0, 0, 0, 0,
                1, 1, 1, 1,
                0, 0, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_180
            {
                0, 0, 1, 0,
                0, 0, 1, 0,
                0, 0, 1, 0,
                0, 0, 1, 0},
            //MINO_ANGLE_270
            {
                0, 0, 0, 0,
                0, 0, 0, 0,
                1, 1, 1, 1,
                0, 0, 0, 0},
        },
        //MINO_TYPE_O
        {
            //MINO_ANGLE_0
            {
                0, 0, 0, 0,
                0, 1, 1, 0,
                0, 1, 1, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_90
            {
                0, 0, 0, 0,
                0, 1, 1, 0,
                0, 1, 1, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_180
            {
                0, 0, 0, 0,
                0, 1, 1, 0,
                0, 1, 1, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_270
            {
                0, 0, 0, 0,
                0, 1, 1, 0,
                0, 1, 1, 0,
                0, 0, 0, 0},
        },
        //MINO_TYPE_S
        {
            //MINO_ANGLE_0
            {
                0, 0, 0, 0,
                0, 0, 1, 1,
                0, 1, 1, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_90
            {
                0, 0, 0, 0,
                0, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 1, 0},
            //MINO_ANGLE_180
            {
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 1, 1,
                0, 1, 1, 0},
            //MINO_ANGLE_270
            {
                0, 0, 0, 0,
                0, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 1, 0},
        },
        //MINO_TYPE_Z
        {
            //MINO_ANGLE_0
            {
                0, 0, 0, 0,
                1, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_90
            {
                0, 0, 1, 0,
                0, 1, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_180
            {
                0, 0, 0, 0,
                1, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_270
            {
                0, 0, 1, 0,
                0, 1, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 0},
        },
        //MINO_TYPE_J
        {
            //MINO_ANGLE_0
            {
                0, 1, 0, 0,
                0, 1, 0, 0,
                1, 1, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_90
            {
                1, 0, 0, 0,
                1, 1, 1, 0,
                0, 0, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_180
            {
                0, 1, 1, 0,
                0, 1, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_270
            {
                0, 0, 0, 0,
                1, 1, 1, 0,
                0, 0, 1, 0,
                0, 0, 0, 0},
        },
        //MINO_TYPE_L
        {
            //MINO_ANGLE_0
            {
                0, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 1, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_90
            {
                0, 0, 0, 0,
                1, 1, 1, 0,
                1, 0, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_180
            {
                1, 1, 0, 0,
                0, 1, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_270
            {
                0, 0, 0, 0,
                0, 0, 1, 0,
                1, 1, 1, 0,
                0, 0, 0, 0},
        },
        //MINO_TYPE_T
        {
            //MINO_ANGLE_0
            {
                0, 0, 0, 0,
                1, 1, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_90
            {
                0, 1, 0, 0,
                1, 1, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_180
            {
                0, 0, 0, 0,
                0, 1, 0, 0,
                1, 1, 1, 0,
                0, 0, 0, 0},
            //MINO_ANGLE_270
            {
                0, 1, 0, 0,
                0, 1, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 0}}};

//fieldの初期化
void initField()
{
    memset(field, 0, sizeof(field));

    for (int y = 0; y < FIELD_HEIGHT; y++)
    {
        field[0][y] = field[FIELD_WIDTH - 1][y] = 1;
    }
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        field[x][FIELD_HEIGHT - 1] = 1;
    }
}

//fieldをdisplayBufferにコピー
void copyField()
{
    memset(displayBuffer, 0, sizeof(displayBuffer));

    for (int y = 0; y < FIELD_HEIGHT; y++)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            displayBuffer[x][y] |= field[x][y];
        }
    }
}

//タイトルを表示
void displayTitle()
{
    system("clear");
    printf("\n\n\n----- TETRiS -----\n\n\n");
}

//プレイヤー名を入力・セット
void setPlayerName()
{
    printf("プレイヤー名を入力してください：");
    scanf("%s", playerName);
    _getch();

    printf("プレイヤー名は「%s」でよろしいですか？よろしければ y を入力してください\n", playerName);

    char res;
    scanf("%c", &res);

    if (res == 'y')
    {
        return;
    }
    setPlayerName();
}

//コンソール画面をクリア
void clearDisplay()
{
    system("clear");
}

//プレイヤー名とスコアを表示
void displayData()
{
    printf("プレイヤー名：%s\t", playerName);
    printf("スコア：%d\n", score);
}

//次のミノを画面に表示
void displayNext()
{
    for (int w = 0; w < MINO_WIDTH; w++)
    {
        for (int h = 0; h < MINO_HEIGHT; h++)
        {
            printf("%s", minoShapes[nextMinoType][nextMinoAngle][w][h] ? "回" : "・");
        }
        printf("\n");
    }
}

//スコア加算
void addScore(int _count)
{
    switch(_count){
        case 1:
            score += 40;
            break;
        case 2:
            score += 100;
            break;
        case 3:
            score += 300;
            break;
        case 4:
            score += 1200;
            break;            
        }
}

//次のミノをランダムに生成
void selectNext()
{
    // nextMinoType = rand() % MINO_TYPE_MAX;
    // nextMinoAngle = rand() % MINO_ANGLE_MAX;
    nextMinoType = nextMinoAngle = 0;   //debug
}

//次のミノを現在のミノに設定
void setCurrent()
{
    minoX = DEFAULT_POS_X;
    minoY = DEFAULT_POS_Y;
    minoType = nextMinoType;
    minoAngle = nextMinoAngle;
}

//ミノの移動チェック
bool check(int _minoX, int _minoY, int minoType, int _minoAngle)
{
    int _nextX;
    int _nextY;
    int _nextA = _minoAngle % MINO_ANGLE_MAX;

    for (int h = 0; h < MINO_HEIGHT; h++)
    {
        for (int w = 0; w < MINO_WIDTH; w++)
        {
            _nextX = _minoX + w;
            _nextY = _minoY + h;

            if (_nextY < 0 || minoShapes[minoType][_nextA][h][w] == 0)
            {
                continue;
            }

            if (minoShapes[minoType][_nextA][h][w] && field[_nextX][_nextY] == 1)
            {
                // printf("cannot move\n");
                return false;
            }
        }
    }
    return true;
}

//ミノを移動
void moveMino(int *minoX, int *minoY, int *minoAngle, int _dx, int _dy, int _da)
{
    *minoX += _dx;
    *minoY += _dy;
    *minoAngle = (*minoAngle + _da) % MINO_ANGLE_MAX;
}

//ミノを第一引数に書き出し
void writeMino(int target[FIELD_WIDTH][FIELD_HEIGHT], int _nextX, int _nextY, int minoType, int minoAngle)
{
    for (int w = 0; w < MINO_WIDTH; w++)
    {
        for (int h = 0; h < MINO_HEIGHT; h++)
        {
            if (_nextY + h < 0 && minoShapes[minoType][minoAngle][h][w] == 1)
            {
                continue;
            }
            target[_nextX + w][_nextY + h] |= minoShapes[minoType][minoAngle][h][w];
        }
    }
}

//コンソールに描画
void draw()
{
    clearDisplay();

    copyField();
    writeMino(displayBuffer, minoX, minoY, minoType, minoAngle);

    for (int y = 0; y < FIELD_HEIGHT; ++y)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            printf("%s", displayBuffer[x][y] ? "回" : "・");
        }
        printf("\n");
    }

    displayNext();
    displayData();
}

void gameStart()
{
    initField();

    selectNext();
    setCurrent();
    selectNext();

    clearDisplay();

    displayData();
    printf("Enterキーを押してゲームスタート！\n");
    _getch();
    _getch();

    clearDisplay();
}

void gameOver()
{
    clearDisplay();
    printf("GameOver\n");

    displayData();
}

double getElapsedTime(clock_t stop, clock_t start)
{
    return (double)(stop - start) / CLOCKS_PER_SEC;
}

int main(void)
{
    system("chcp 65001");

    displayTitle();

    setPlayerName();

    gameStart();

    // time_t t = time(NULL); //tに現在時刻を入力
    clock_t startTime = clock();

    while (1)
    {
        int dx = 0; //横移動
        int dy = 0; //縦移動
        int da = 0; //回転

        if (_kbhit())
        {
            switch (_getch())
            {
            case 'a':
            case 0x44:
                if (check(minoX - 1, minoY, minoType, minoAngle))
                    dx = -1;
                break;
            case 'd':
            case 0x43:
                if (check(minoX + 1, minoY, minoType, minoAngle))
                    dx = 1;
                break;
            case 'w':
            case 0x41:
                if (check(minoX, minoY, minoType, minoAngle + 1))
                {
                    da = 1;
                }
                else
                {
                    int _dx = minoX < FIELD_WIDTH / 2 ? 1 : -1;
                    if (check(minoX + _dx, minoY, minoType, minoAngle + 1))
                    {
                        dx = _dx;
                        da = 1;
                    }
                    else
                    {
                        _dx = _dx > 0 ? _dx + 1 : _dx - 1;
                        if (check(minoX + _dx, minoY, minoType, minoAngle + 1))
                        {
                            dx = _dx;
                            da = 1;
                        }
                    }
                }
                break;
            case 0x20:
            case 0x42:
                if (check(minoX, minoY + 1, minoType, minoAngle))
                    dy = 1;
                break;
            }

            moveMino(&minoX, &minoY, &minoAngle, dx, dy, da);
            draw();
        }

        // if (t != time(NULL))
        if (getElapsedTime(clock(), startTime) > 1)
        {
            // t = time(NULL);
            startTime = clock();
            dy++;

            if (check(minoX + dx, minoY + dy, minoType, minoAngle + da))
            {
                moveMino(&minoX, &minoY, &minoAngle, dx, dy, da);
            }
            else
            {
                // printf("fix\n");
                writeMino(field, minoX, minoY, minoType, minoAngle);

                if(minoY < 0)
                {
                    break;
                }

                int x, y;
                int count = 0;
                for (y = 0; y < FIELD_HEIGHT - 1; y++)
                {
                    bool lineFill = true;

                    for (x = 1; x < FIELD_WIDTH - 1; x++)
                    {
                        if (!field[x][y])
                            lineFill = false;
                    }
                    if (lineFill)
                    {
                        count++;
                        // printf("fill\n");
                        for (int i = y; 0 < i; i--)
                        {
                            for (int j = 0; j < FIELD_WIDTH; j++)
                            {
                                field[j][i] = field[j][i - 1];
                            }
                            y--;
                            printf("count : %d\n", count);
                        }
                    }
                }
                addScore(count);
                setCurrent();
                selectNext();
            }

            draw();
        }
    }

    gameOver();

    return 0;
}