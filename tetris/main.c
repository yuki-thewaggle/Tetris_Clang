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

int minoX = 5;
int minoY = 0;
int minoAngle = 0;
int minoType = 0;

int field[FIELD_WIDTH][FIELD_HEIGHT];
int backBuffer[FIELD_WIDTH][FIELD_HEIGHT] = {};

char minoShapes[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_HEIGHT][MINO_WIDTH] =
    {
        //MINO_TYPE_I
        {
            //MINO_ANGLE_0
            {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0}},
            {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
            {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}},
            {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
        },
        //MINO_TYPE_O
        {
            //MINO_ANGLE_0
            {
                {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
        },
        //MINO_TYPE_S
        {
            //MINO_ANGLE_0
            {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
        },
        //MINO_TYPE_Z
        {
            //MINO_ANGLE_0
            {
                {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
        },
        //MINO_TYPE_J
        {
            //MINO_ANGLE_0
            {
                {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
        },
        //MINO_TYPE_L
        {
            //MINO_ANGLE_0
            {
                {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
        },
        //MINO_TYPE_T
        {
            //MINO_ANGLE_0
            {
                {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
            {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
        }};

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

//fieldをbackBufferにコピー
void copyField()
{
    memset(backBuffer, 0, sizeof(backBuffer));

    for (int y = 0; y < FIELD_HEIGHT; y++)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            backBuffer[x][y] |= field[x][y];
        }
    }
}

//ミノを第一引数に書き出し
// void writeMino(int *target, int _nextX, int _nextY, int _minoType, int _minoAngle)
// {
//     for (int w = 0; w < MINO_WIDTH; w++)
//     // for (int h = 0; h < MINO_HEIGHT; h++)
//     {
//         for (int h = 0; h < MINO_HEIGHT; h++)
//         // for (int w = 0; w < MINO_WIDTH; w++)
//         {
//             // target[_nextX+w][_nextY+h] |= minoShapes[_minoType][_minoAngle][r][c];
//             *((int *)(target) + ((_nextX + w) * MINO_WIDTH) + (_nextY + h)) |= minoShapes[_minoType][_minoAngle][h][w];
//         }
//     }
// }
void writeMino(int _nextX, int _nextY, int _minoType, int _minoAngle)
{
    for (int w = 0; w < MINO_WIDTH; w++)
    // for (int h = 0; h < MINO_HEIGHT; h++)
    {
        for (int h = 0; h < MINO_HEIGHT; h++)
        // for (int w = 0; w < MINO_WIDTH; w++)
        {
            backBuffer[_nextX+w][_nextY+h] |= minoShapes[_minoType][_minoAngle][h][w];
        }
    }
}

//ミノの移動チェック
bool check(int _minoX, int _minoY, int _minoType, int _minoAngle)
{
    int nextX;
    int nextY;
    // for (int w = 0; w < MINO_WIDTH; w++)
    for (int w = MINO_WIDTH; w > 0 ; w--)
    {
        // for (int h = 0; h < MINO_HEIGHT; h++)
        for(int h = MINO_HEIGHT; h > 0; h--)
        {
            // int position = minoShapes[_minoType][_minoAngle][w][h] + w;
            // nextX = _minoX + position;
            // nextY = _minoY + position;
            nextX = w + _minoX;
            nextY = h + _minoY;
            printf("nextX, nextY : %d, %d    field[nextX][nextY] : %d\n", nextX, nextY, field[nextX][nextY]);

            //フィールド外判定
            if ((nextX < 0) || (nextX > FIELD_WIDTH - 1) || (nextY > FIELD_HEIGHT))
            {
                printf("out\n");
                return false;
            }

            //移動判定
            if (nextY >= 0 && field[nextX][nextY] != 0)
            {
                printf("cannot move\n");
                return false;
            }
        }
    }
    return true;
}

//ミノを移動
void moveMino(int* _minoX, int* _minoY, int* _minoAngle, int _dx, int _dy, int _da)
{
    *_minoX += _dx;
    *_minoY += _dy;
    *_minoAngle = (*_minoAngle + _da) % 4;
}

bool isCollision(int _minoX, int _minoY, int _minoType, int _minoAngle)
{
    int nextX;
    int nextY;
    for (int r = 0; r < MINO_HEIGHT; r++)
    {
        for (int c = 0; c < MINO_WIDTH; c++)
        {
            int position = minoShapes[_minoType][_minoAngle][r][c] + c;
            nextX = _minoX + position;
            nextY = _minoY + position;

            //フィールド外判定
            if ((nextX < 1) || (nextX > FIELD_WIDTH - 2))
            {
                printf("out\n");
                return true;
            }
            //ボトム判定
            if (_minoY >= FIELD_HEIGHT)
            {
                printf("bottom\n");
                // writeMino(*field, minoX, minoY, minoType, minoAngle);
                minoX = 5;
                minoY = 0;
                return true;
            }
            //衝突判定
            if ((minoShapes[_minoType][_minoAngle][r][nextY] == 1) && (nextY >= 0) && field[nextX][nextY] == 1)
            {
                printf("collision\n");
                // writeMino(*field, minoX, minoY, minoType, minoAngle);
                minoX = 5;
                minoY = 0;
                return true;
            }
        }
    }

    return false;
}

// //一番左、一番右、一番上、一番下の座標を返す
// int* getLimit(int _minoX, int _minoY, int _minoType, int _minoAngle)
// {
//     int crd[8] = {}; //{一番上のx, 一番上のy, 一番上のx, 一番上のx, 一番上のx, 一番上のx, 一番上のx, 一番上のx}
//     int i = 0;
//     for (int r = 0; r < MINO_HEIGHT; r++)
//     {
//         for (int c = 0; c < MINO_WIDTH; c++)
//         {
//             int current = minoShapes[_minoType][_minoAngle][r][c] + c;
//             int _x = _minoX + position;
//             int _y = _minoY + position;
//             if (current == 1)
//             {
//                 crd()
//             }
//         }
//     }
//     return crd;
// }

int main(void)
{
    system("chcp 65001");

    time_t t = time(NULL); //tに現在時刻を入力

    initField();

    while (1)
    {
        int dx = 0;
        int dy = 0;
        int da = 0;

        if (_kbhit())
        {
            switch (_getch())
            {
                case 'a':
                    if (!isCollision(minoX - 1, minoY, minoType, minoAngle))
                        //minoX--;
                        dx = -1;
                    break;
                case 's':
                    if (!isCollision(minoX, minoY + 1, minoType, minoAngle))
                        //minoY++;
                        dy = 1;
                    break;
                case 'd':
                    if (!isCollision(minoX + 1, minoY, minoType, minoAngle))
                        //minoX++;
                        dx = 1;
                    break;
                case 0x20:
                    // minoAngle = (minoAngle + 1) % MINO_ANGLE_MAX;
                    da = 1;
                    break;
            }
        }
        else
        {
            //描画する処理の前に現在時刻と異なっていたら描画する（1秒ごと更新）
            //backBufferを描画
            if (t != time(NULL))
            {
                t = time(NULL);
                //minoY++;
                dy = 1;

                if (check(minoX, minoY, minoType, minoAngle)) //みのが移動できれば
                {
                    //みのの座標を移動
                    moveMino(&minoX, &minoY, &minoAngle, dx, dy, da);
                }

                //描画
                copyField();
                writeMino(minoX, minoY, minoType, minoAngle);

                for (int y = 0; y < FIELD_HEIGHT; ++y)
                {
                    for (int x = 0; x < FIELD_WIDTH; x++)
                    {
                        printf("%s", backBuffer[x][y] ? "回" : "・");
                    }
                    printf("\n");
                }
                copyField();
            }
        }

        // else if(dy)//秒数が経過していれば
        // {
        //     //画面描画の準備
        //     //固定みのを描画
        //     //minoX,minoY,を初期値にする
        //     //angleを適当に変える

        //     //ゲームオーバー判定

        //     //行が揃っていたら消す
        //     //次のみのを決める

        //     //画面に描画する
        // }

            // printf("minoX = %d, minoY = %d, minoType = %d, minoAngle = %d\n", minoX, minoY, minoType, minoAngle); //debug

            // for (int y = 0; y < FIELD_HEIGHT; ++y)
            // {
            //     for (int x = 0; x < FIELD_WIDTH; x++)
            //     {
            //         printf("%s", backBuffer[x][y] ? "回" : "・");
            //     }
            //     printf("\n");
            // }
            // if (minoY >= FIELD_HEIGHT - 2)
            // {
            //     printf("bottom !\n");
            //     writeMino(*field, minoX, minoY, minoType, minoAngle);
            //     minoX = 5;
            //     minoY = 0;
            // }

            // for (int y = 0; y < FIELD_HEIGHT; ++y)
            // {
            //     for (int x = 0; x < FIELD_WIDTH; x++)
            //     {
            //         printf("%s", field[x][y] ? "口" : "　");
            //     }
            //     printf("\n");
            // }

            //backBufferへminoを追加
            // for (int y = 0; y < MINO_HEIGHT; y++)
            // {
            //     for (int x = 0; x < MINO_WIDTH; x++)
            //     {
            //         backBuffer[minoX + x][minoY + y] |= minoShapes[minoType][minoAngle][x][y];
            //     }
            // }

            //ボトム判定
            // for (int w = 0; w < MINO_WIDTH; w++)
            // {
            //     for (int h = MINO_HEIGHT - 1; h < 0; h--)
            //     {
            //         if(minoShapes[minoType][minoAngle][w][h])
            //         {
            //             if(h+minoY > FIELD_HEIGHT-1) {
            //                 printf("bottom\n");
            //                 writeMino(*field, minoX, minoY, minoType, minoAngle);
            //                 minoX = 5;
            //                 minoY = 0;
            //             }
            //         }
            //     }
            // }
        }
    return 0;
}