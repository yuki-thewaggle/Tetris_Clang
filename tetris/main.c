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

int field[FIELD_WIDTH][FIELD_HEIGHT];
int backBuffer[FIELD_WIDTH][FIELD_HEIGHT] = {};

char minoShapes[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_HEIGHT][MINO_WIDTH] =
    {
        //MINO_TYPE_I
        {
            //MINO_ANGLE_0
            {{0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0}},
            //MINO_ANGLE_90
            {{0, 0, 0, 0},
             {0, 0, 0, 0},
             {1, 1, 1, 1},
             {0, 0, 0, 0}},
            //MINO_ANGLE_180
            {{0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0}},
            //MINO_ANGLE_270
            {{0, 0, 0, 0},
             {1, 1, 1, 1},
             {0, 0, 0, 0},
             {0, 0, 0, 0}},
        },
        //MINO_TYPE_O
        {
            {{0, 0, 0, 0},
             {0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}},
            {{0, 0, 0, 0},
             {0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}},
            {{0, 0, 0, 0},
             {0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}},
            {{0, 0, 0, 0},
             {0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}},
        }, //MINO_TYPE_S
        {
            {{0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0}},
            {{0, 0, 0, 0},
             {1, 1, 1, 0},
             {0, 0, 1, 1},
             {0, 0, 0, 0}},
            {{0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0}},
            {{0, 0, 0, 0},
             {1, 1, 1, 0},
             {0, 0, 1, 1},
             {0, 0, 0, 0}},
        },
        //MINO_TYPE_Z
        {
            //MINO_ANGLE_0
            {{0, 0, 0, 0},
             {1, 1, 0, 0},
             {0, 0, 1, 1},
             {0, 0, 0, 0}},
            {{0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0}},
            {{0, 0, 0, 0},
             {1, 1, 0, 0},
             {0, 0, 1, 1},
             {0, 0, 0, 0}},
            {{0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0}},
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

            if (_nextY < 0 || minoShapes [minoType][_nextA][h][w] == 0)
            {
                continue;
            }

            if (minoShapes[minoType][_nextA][h][w])
            {
                //フィールド外判定
                if (_nextX < 1 || _nextX > FIELD_WIDTH - 2 || _nextY > FIELD_HEIGHT - 1)
                {
                    printf("out\n");
                    return false;
                }

                // printf("field[%d][%d] = %d\n", _nextX, _nextY, field[_nextX][_nextY]);
                //移動判定
                if (/*_nextY < 1 ||*/ field[_nextX][_nextY] == 1 /*|| _nextX < 1 || _nextX > FIELD_WIDTH - 2*/)
                {
                    printf("cannot move\n");
                    return false;
                }
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
    *minoAngle = (*minoAngle + _da) % 4;
}

// bool isCollision(int _minoX, int _minoY, int _minoType, int _minoAngle)
// {
//     int nextX;
//     int nextY;
//     for (int h = 0; h < MINO_HEIGHT; h++)
//     {
//         for (int w = 0; w < MINO_WIDTH; w++)
//         {
//             int position = minoShapes[_minoType][_minoAngle][w][h] + w;
//             nextX = _minoX + position;
//             nextY = _minoY + position;

//             //フィールド外判定
//             if ((nextX < 0) || (nextX > FIELD_WIDTH))
//             {
//                 printf("out\n");
//                 return true;
//             }
//             //ボトム判定
//             if (_minoY >= FIELD_HEIGHT)
//             {
//                 printf("bottom\n");
//                 // writeMino(*field, minoX, minoY, minoType, minoAngle);
//                 minoX = 5;
//                 minoY = 0;
//                 return true;
//             }
//             //衝突判定
//             if ((minoShapes[_minoType][_minoAngle][h][nextY] == 1) && (nextY >= 0) && field[nextX][nextY] == 1)
//             {
//                 printf("collision\n");
//                 // writeMino(*field, minoX, minoY, minoType, minoAngle);
//                 minoX = 5;
//                 minoY = 0;
//                 return true;
//             }
//         }
//     }

//     return false;
// }

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

void draw()
{
    copyField();
    writeMino(backBuffer, minoX, minoY, minoType, minoAngle);

    for (int y = 0; y < FIELD_HEIGHT; ++y)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            printf("%s", backBuffer[x][y] ? "回" : "・");
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
        int dx = 0;
        int dy = 0;
        int da = 0;

        if (_kbhit())
        {
            switch (_getch())
            {
                //TODO:checkまとめられそう
            case 'a':
                if (check(minoX - 1, minoY, minoType, minoAngle))
                    //minoX--;
                    dx = -1;
                break;
            case 's':
                if (check(minoX, minoY + 1, minoType, minoAngle))
                    //minoY++;
                    dy = 1;
                break;
            case 'd':
                if (check(minoX + 1, minoY, minoType, minoAngle))
                    //minoX++;
                    dx = 1;
                break;
            case 0x20:
                if (check(minoX, minoY, minoType, minoAngle + 1))
                    // minoAngle = (minoAngle + 1) % MINO_ANGLE_MAX;
                    da = 1;
                break;
            }

            //from
            //TODO:描画まとめる
            // printf("%d, %d, %d) -> ", minoX, minoY, minoAngle);
            moveMino(&minoX, &minoY, &minoAngle, dx, dy, da);
            // // printf("%d, %d, %d)\n", minoX, minoY, minoAngle);

            draw();
           // //to
            // continue;
        }

        if (t != time(NULL))
        {
            t = time(NULL);

            dy++;

            //from
            printf("dx, dy, da : %d, %d, %d\n", dx, dy, da);
            if (check(minoX + dx, minoY + dy, minoType, minoAngle + da))
            {
                moveMino(&minoX, &minoY, &minoAngle, dx, dy, da);
            }
            else
            {
                writeMino(field, minoX, minoY, minoType, minoAngle);

                minoX = DEFAULT_POS_X;
                minoY = DEFAULT_POS_Y;
                minoAngle = 0;
                minoType = 0;
            }

            draw();
        }
    }

    //to

    // {
    //描画する処理の前に現在時刻と異なっていたら描画する（1秒ごと更新）
    //backBufferを描画
    // }

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
    return 0;
}