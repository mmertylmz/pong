#ifndef BALL_H
#define BALL_H

#include "raylib.h"
#include "paddle.h"

struct Ball {
    int x, y;
    int size;
    int speedX, speedY;
    Color color;
};

#endif // BALL_H
