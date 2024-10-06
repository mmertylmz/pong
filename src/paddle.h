#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

struct Paddle {
    int x, y;
    int width, height;
    int speed;
    Color color;
};

#endif // PADDLE_H
