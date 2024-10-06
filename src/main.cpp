#include <iostream>

#include "raylib.h"
#include <cstdlib>
#include <ctime>

#include "paddle.h"
#include "ball.h"
#include "particle.h"
#include "game.h"

int main(void)
{
	// Initialize Window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");

	//Icon
	Image icon = LoadImage("assets/icon/pong.png");
	SetWindowIcon(icon);
	UnloadImage(icon);

	SetTargetFPS(60);

	// Game objects
	Paddle playerPaddle = { 50, (SCREEN_HEIGHT - 175) / 2, 25, 175, 5, ORANGE };
	Paddle aiPaddle = { SCREEN_WIDTH - 75, (SCREEN_HEIGHT - 175) / 2, 25, 175, 4, BLUE };
	Ball ball = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 15, 7, 7, BLACK };

	int playerScore = 0;
	int aiScore = 0;

	int countdown = 3;
	float countdownTimer = 0;
	float countdownAlpha = 1.0f;
	float countdownFontSize = 300;

	bool gameStarted = false;
	float gameTimer = 60.0f;

	// Random Ball Movement Generate
	srand(static_cast<unsigned>(time(NULL)));
	ball.speedX = (rand() % 2 == 0) ? ball.speedX : -ball.speedX;
	ball.speedY = (rand() % 2 == 0) ? ball.speedY : -ball.speedY;

	// Reset Game when Starts
	ResetGame(playerPaddle, aiPaddle, ball, gameStarted, countdown);

	GameScreen currentScreen = MENU;
	Difficulty currentDifficulty = MEDIUM;
	GameMode currentMode = SCORE_LIMITED;

	// Main Loop
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(LIGHTGRAY);		

		switch (currentScreen) {
		case MENU:
			//Menu Drawings
			DrawText("PONG GAME", (SCREEN_WIDTH - MeasureText("PONG GAME", 60)) / 2, SCREEN_HEIGHT / 2 - 200, 60, DARKGRAY);
			DrawText("Press ENTER to Play", (SCREEN_WIDTH - MeasureText("Press ENTER to Play", 40)) / 2, SCREEN_HEIGHT / 2 - 100, 40, DARKGRAY);
			DrawText("Press ESC to Quit", (SCREEN_WIDTH - MeasureText("Press ESC to Quit", 30)) / 2, SCREEN_HEIGHT / 2 - 50, 30, DARKGRAY);
			DrawText("Press F to Fullscreen", (SCREEN_WIDTH - MeasureText("Press F to Fullscreen", 30)) / 2, SCREEN_HEIGHT / 2 + 100, 30, DARKGRAY);
			DrawText("Press T for Tutorial", SCREEN_WIDTH / 2 - MeasureText("Press T for Tutorial", 30) / 2, SCREEN_HEIGHT / 2 + 150, 30, DARKGRAY);


			if (IsKeyPressed(KEY_ENTER)) {
				currentScreen = MODE_SELECT;
			}

			if (IsKeyPressed(KEY_F)) {
				ToggleFullscreen();
			}

			if (IsKeyPressed(KEY_T)) {
				currentScreen = TUTORIAL;
			}

			break;

		case PLAY:
			UpdateGame(playerPaddle, aiPaddle, ball, playerScore, aiScore, gameStarted, countdown, countdownTimer, countdownAlpha, countdownFontSize, currentDifficulty);
			UpdateParticles();

			DrawGame(playerPaddle, aiPaddle, ball, playerScore, aiScore, gameStarted, countdown, countdownAlpha, countdownFontSize);
			DrawParticles();

			

			if (currentMode == TIME_LIMITED) {
				if (countdown <= 0) {
					gameTimer -= GetFrameTime();				
				}					

				if (gameTimer <= 0.0f && playerScore == aiScore) {
					DrawText("Golden Score!", (SCREEN_WIDTH - MeasureText("Golden Score!", 50)) / 2, 20, 50, RED);
				}
				else if (gameTimer <= 0.0f) {
					currentScreen = END;
				}
				else {
					DrawText(TextFormat("Time: %.1f", gameTimer), 20, 20, 30, DARKGRAY);
				}
			}

			//Pause
			if (IsKeyPressed(KEY_P)) {
				currentScreen = PAUSE;
			}

			//Reset
			if (IsKeyPressed(KEY_R)) {
				ResetGame(playerPaddle, aiPaddle, ball, gameStarted, countdown);
				playerScore = 0;
				aiScore = 0;
				gameTimer = 60.0f;
				currentScreen = MENU;				
			}

			//Quit
			if (IsKeyPressed(KEY_Q)) {
				currentScreen = END;
			}

			if (currentMode == SCORE_LIMITED && (playerScore >= 5 || aiScore >= 5)) {
				currentScreen = END;
			}

			break;

		case END:
			//Game Over Screen
			DrawText("GAME OVER", (SCREEN_WIDTH - MeasureText("GAME OVER", 60)) / 2, SCREEN_HEIGHT / 2 - 100, 60, DARKGRAY);
			DrawText(TextFormat("Final Score: %d - %d", playerScore, aiScore), (SCREEN_WIDTH - MeasureText(TextFormat("Final Score: %d - %d", playerScore, aiScore), 40)) / 2, SCREEN_HEIGHT / 2, 40, DARKGRAY);
			DrawText("Press ENTER to return Menu", (SCREEN_WIDTH - MeasureText("Press ENTER to return Menu", 30)) / 2, SCREEN_HEIGHT / 2 + 50, 30, DARKGRAY);

			if (IsKeyPressed(KEY_ENTER)) {
				ResetGame(playerPaddle, aiPaddle, ball, gameStarted, countdown);
				playerScore = 0; 
				aiScore = 0;
				gameTimer = 60.0f;
				currentScreen = MENU;
			}

			if (IsKeyPressed(KEY_F)) {
				ToggleFullscreen();
			}

			break;
		case DIFFICULTY_SELECT:
			DrawText("SELECT DIFFICULTY", (SCREEN_WIDTH - MeasureText("SELECT DIFFICULTY", 50)) / 2, SCREEN_HEIGHT / 2 - 150, 50, DARKGRAY);
			DrawText("1. EASY", (SCREEN_WIDTH - MeasureText("1. EASY", 30)) / 2, SCREEN_HEIGHT / 2 - 50, 30, DARKGRAY);
			DrawText("2. MEDIUM", (SCREEN_WIDTH - MeasureText("2. MEDIUM", 30)) / 2, SCREEN_HEIGHT / 2, 30, DARKGRAY);
			DrawText("3. HARD", (SCREEN_WIDTH - MeasureText("3. HARD", 30)) / 2, SCREEN_HEIGHT / 2 + 50, 30, DARKGRAY);

			if (IsKeyPressed(KEY_ONE)) {
				currentDifficulty = EASY;
				currentScreen = PLAY;
			}
			else if (IsKeyPressed(KEY_TWO)) {
				currentDifficulty = MEDIUM;
				currentScreen = PLAY;
			}
			else if (IsKeyPressed(KEY_THREE)) {
				currentDifficulty = HARD;
				currentScreen = PLAY;
			}

			break;

		case MODE_SELECT:
			DrawText("SELECT GAME MODE", (SCREEN_WIDTH - MeasureText("SELECT GAME MODE", 50)) / 2, SCREEN_HEIGHT / 2 - 150, 50, DARKGRAY);
			DrawText("1. TIME LIMITED", (SCREEN_WIDTH - MeasureText("1. TIME LIMITED", 30)) / 2, SCREEN_HEIGHT / 2 - 50, 30, DARKGRAY);
			DrawText("2. SCORE LIMITED", (SCREEN_WIDTH - MeasureText("2. SCORE LIMITED", 30)) / 2, SCREEN_HEIGHT / 2, 30, DARKGRAY);

			if (IsKeyPressed(KEY_ONE)) {
				currentMode = TIME_LIMITED;
				currentScreen = DIFFICULTY_SELECT;
			}
			else if (IsKeyPressed(KEY_TWO)) {
				currentMode = SCORE_LIMITED;
				currentScreen = DIFFICULTY_SELECT;
			}
			break;

		case PAUSE:
			DrawText("PAUSED", SCREEN_WIDTH / 2 - MeasureText("PAUSED", 60) / 2, SCREEN_HEIGHT / 2 - 100, 60, DARKGRAY);
			DrawText("Press P to Resume", SCREEN_WIDTH / 2 - MeasureText("Press P to Resume", 30) / 2, SCREEN_HEIGHT / 2, 30, DARKGRAY);
			DrawText("Press R to Restart", SCREEN_WIDTH / 2 - MeasureText("Press R to Restart", 30) / 2, SCREEN_HEIGHT / 2 + 50, 30, DARKGRAY);

			//Continue
			if (IsKeyPressed(KEY_P)) {
				currentScreen = PLAY;
			}

			//Reset
			if (IsKeyPressed(KEY_R)) {
				ResetGame(playerPaddle, aiPaddle, ball, gameStarted, countdown);
				playerScore = 0;
				aiScore = 0;
				gameTimer = 60.0f;
				currentScreen = MENU;
			}

			break;

		case TUTORIAL:
			DrawText("TUTORIAL", SCREEN_WIDTH / 2 - MeasureText("TUTORIAL", 50) / 2, 20, 50, DARKGRAY);

			int startY = 100;
			int lineSpacing = 40;

			DrawText("1. Press W and S keys to move the paddle.", 20, startY, 30, DARKGRAY);
			DrawText("2. Press F to toggle Fullscreen mode.", 20, startY + lineSpacing, 30, DARKGRAY);
			DrawText("3. Press R to reset the game during play or pause.", 20, startY + 2 * lineSpacing, 30, DARKGRAY);
			DrawText("4. Press P to pause the game and press P again to resume.", 20, startY + 3 * lineSpacing, 30, DARKGRAY);
			DrawText("5. Press ENTER to start the game.", 20, startY + 4 * lineSpacing, 30, DARKGRAY);

			DrawText("Press T to return Main Screen.", 20, startY + 6 * lineSpacing, 30, DARKGRAY);

			if (IsKeyPressed(KEY_T)) {
				currentScreen = MENU;
			}

		}
		EndDrawing();

	}

	
	CloseWindow();

	return 0;
}
