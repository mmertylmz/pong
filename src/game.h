#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "paddle.h"
#include "ball.h"

// Screen Resolution
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

//Game Screen
enum GameScreen { MENU, PLAY, END, DIFFICULTY_SELECT, MODE_SELECT, PAUSE, TUTORIAL };
enum Difficulty { EASY, MEDIUM, HARD };
enum GameMode {TIME_LIMITED, SCORE_LIMITED};

// Functions
void ResetGame(Paddle& playerPaddle, Paddle& aiPaddle, Ball& ball, bool& gameStarted, int& countdown);
void UpdateGame(Paddle& playerPaddle, Paddle& aiPaddle, Ball& ball, int& playerScore, int& aiScore, bool& gameStarted, int& countdown, float& countdownTimer, float& countdownAlpha, float& countdownFontSize, Difficulty difficulty);
void DrawGame(const Paddle& playerPaddle, const Paddle& aiPaddle, const Ball& ball, int playerScore, int aiScore, bool gameStarted, int countdown, float countdownAlpha, float countdownFontSize);

#endif // GAME_H
