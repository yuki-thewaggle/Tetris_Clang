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

int minoX = DEFAULT_POS_X;
int minoY = DEFAULT_POS_Y;
int minoAngle = 0;
int minoType = 0;
int score = 0;

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
            0, 0, 0, 0}
    }
};

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
                printf("cannot move\n");
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
}

int main(void)
{
    system("chcp 65001");

    time_t t = time(NULL); //tに現在時刻を入力

    initField();

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
                if (check(minoX - 1, minoY, minoType, minoAngle))
                    dx = -1;
                break;
            case 'd':
                if (check(minoX + 1, minoY, minoType, minoAngle))
                    dx = 1;
                break;
            case 'w':
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
                }
                break;
            case 0x20:
                if (check(minoX, minoY + 1, minoType, minoAngle))
                    dy = 1;
                break;
            }

            moveMino(&minoX, &minoY, &minoAngle, dx, dy, da);
            draw();
        }

        if (t != time(NULL))
        {
            t = time(NULL);
            dy++;

            if (check(minoX + dx, minoY + dy, minoType, minoAngle + da))
            {
                moveMino(&minoX, &minoY, &minoAngle, dx, dy, da);
            }
            else
            {
                printf("fix\n");
                writeMino(field, minoX, minoY, minoType, minoAngle);

                int x, y;
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
                        // printf("fill\n");
                        for (int i = y; 0 < i; i--)
                        {
                            for (int j = 0; j < FIELD_WIDTH; j++)
                            {
                                field[j][i] = field[j][i - 1];
                            }
                            y--;
                        }
                    }
                }

                minoX = DEFAULT_POS_X;
                minoY = DEFAULT_POS_Y;
                minoAngle = rand() % MINO_ANGLE_MAX;
                minoType = rand() % MINO_TYPE_MAX;
                // minoType = (minoType + 1) % MINO_TYPE_MAX;
            }

            draw();
        }
    }

    return 0;
}