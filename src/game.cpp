#include "game.h"
#include "particle.h"


#include <cstdlib>
#include <ctime>

const int MAX_BALL_SPEED = 15;

// Function to reset the game state
void ResetGame(Paddle& playerPaddle, Paddle& aiPaddle, Ball& ball, bool& gameStarted, int& countdown)
{
	ball.x = SCREEN_WIDTH / 2;
	ball.y = SCREEN_HEIGHT / 2;
	ball.speedX = (rand() % 2 == 0) ? 5 : -5;
	ball.speedY = (rand() % 2 == 0) ? 5 : -5;

	// Center the paddles
	playerPaddle.y = (SCREEN_HEIGHT - playerPaddle.height) / 2;
	aiPaddle.y = (SCREEN_HEIGHT - aiPaddle.height) / 2;

    // Reset the countdown
	countdown = 3;
	gameStarted = false;
}

// Function to update the game state
void UpdateGame(Paddle& playerPaddle, Paddle& aiPaddle, Ball& ball, int& playerScore, int& aiScore, bool& gameStarted, int& countdown, float& countdownTimer, float& countdownAlpha, float& countdownFontSize, Difficulty difficulty)
{
    // Timer to reduce the margin of error over time
    static float elapsedTime = 0.0f;
    elapsedTime += GetFrameTime();

    if (!gameStarted)
    {
        countdownTimer += GetFrameTime();
        if (countdownTimer >= 1.0f)
        {
            countdown--;
            countdownTimer = 0;
            countdownAlpha = 1.0f;
            countdownFontSize = 300;
        }

        countdownAlpha -= 0.5f * GetFrameTime();
        countdownFontSize -= 50 * GetFrameTime();

        if (countdown <= 0)
        {
            gameStarted = true;
        }
    }

    if (gameStarted)
    {
        // Player paddle movement
        if (IsKeyDown(KEY_W) && playerPaddle.y > 0)
            playerPaddle.y -= playerPaddle.speed;
        if (IsKeyDown(KEY_S) && (playerPaddle.y + playerPaddle.height) < SCREEN_HEIGHT)
            playerPaddle.y += playerPaddle.speed;

        // AI paddle movement
        float aiPaddleSpeed;
        switch (difficulty) {
        case EASY:
            aiPaddleSpeed = 2.0f;
            break;
        case MEDIUM:
            aiPaddleSpeed = 4.0f;
            break;
        case HARD:
            aiPaddleSpeed = 6.0f;
            break;
        }

        // Dynamic margin of error
        float maxOffset = 30.0f - (elapsedTime * 0.5f); // Predict the future position of the ball
        if (maxOffset < 5.0f) maxOffset = 5.0f; // Minimum margin of error

        int randomOffset = rand() % static_cast<int>(maxOffset) - static_cast<int>(maxOffset) / 2;

        int predictedBallY = ball.y + ball.speedY * 10;

        // AI paddle movement limitation
        float targetY = predictedBallY - aiPaddle.height / 2 + randomOffset;

        if (aiPaddle.y < targetY && aiPaddle.y + aiPaddle.height < SCREEN_HEIGHT)
        {
            aiPaddle.y += aiPaddleSpeed;
        }
        else if (aiPaddle.y > targetY && aiPaddle.y > 0)
        {
            aiPaddle.y -= aiPaddleSpeed;
        }

        // Ball movement
        ball.x += ball.speedX;
        ball.y += ball.speedY;

        // Ball - Wall Collision
        if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT) {
            ball.speedY = -ball.speedY;
            CreateParticles({ static_cast<float>(ball.x), static_cast<float>(ball.y) }, abs(ball.speedX) + abs(ball.speedY));
        }

        // Ball - Player Paddle Collision
        if (ball.x - ball.size <= playerPaddle.x + playerPaddle.width &&
            ball.x + ball.size >= playerPaddle.x &&
            ball.y + ball.size >= playerPaddle.y &&
            ball.y - ball.size <= playerPaddle.y + playerPaddle.height)
        {
            ball.speedX = -ball.speedX;

            CreateParticles({ static_cast<float>(ball.x), static_cast<float>(ball.y) }, abs(ball.speedX) + abs(ball.speedY));


            // Ball speed increases with each paddle collision
            ball.speedX += (ball.speedX > 0) ? 1 : -1;

            // Calculate collision position
            float hitPos = (ball.y - playerPaddle.y) / static_cast<float>(playerPaddle.height);
            ball.speedY = (hitPos - 0.5f) * 15; // Angle

            // Prevent perfectly vertical movement
            if (abs(ball.speedY) < 2.0f) {
                ball.speedY += (ball.speedY > 0) ? 2.0f : -2.0f; // Angle
            }

            ball.x = playerPaddle.x + playerPaddle.width + ball.size; // Prevent stickness
        }

        // Ball speed limit
        if (ball.speedX > MAX_BALL_SPEED) ball.speedX = MAX_BALL_SPEED;
        else if (ball.speedX < -MAX_BALL_SPEED) ball.speedX = -MAX_BALL_SPEED;

        if (ball.speedY > MAX_BALL_SPEED) ball.speedY = MAX_BALL_SPEED;
        else if (ball.speedY < -MAX_BALL_SPEED) ball.speedY = -MAX_BALL_SPEED;

        // Ball - AI Paddle Collision
        if (ball.x + ball.size >= aiPaddle.x &&
            ball.x - ball.size <= aiPaddle.x + aiPaddle.width &&
            ball.y + ball.size >= aiPaddle.y &&
            ball.y - ball.size <= aiPaddle.y + aiPaddle.height)
        {
            ball.speedX = -ball.speedX;

            CreateParticles({ static_cast<float>(ball.x), static_cast<float>(ball.y) }, abs(ball.speedX) + abs(ball.speedY));

            // Ball speed increases with each paddle collision
            ball.speedX += (ball.speedX > 0) ? 1 : -1;

            // Calculate collision position
            float hitPos = (ball.y - aiPaddle.y) / static_cast<float>(aiPaddle.height);
            ball.speedY = (hitPos - 0.5f) * 15; // Daha açılı yap

            // Prevent perfectly vertical movement
            if (abs(ball.speedY) < 2.0f) {
                ball.speedY += (ball.speedY > 0) ? 2.0f : -2.0f; // Angle
            }

            ball.x = aiPaddle.x - ball.size; // Prevent stickness
        }

        // Score Update
        if (ball.x <= 0)
        {
            aiScore++;
            ResetGame(playerPaddle, aiPaddle, ball, gameStarted, countdown);
            elapsedTime = 0.0f; // Game Reset
            
        }
        else if (ball.x >= SCREEN_WIDTH)
        {
            playerScore++;
            ResetGame(playerPaddle, aiPaddle, ball, gameStarted, countdown);
            elapsedTime = 0.0f; // Game Reset
        }
    }
}


// Draw Game
void DrawGame(const Paddle& playerPaddle, const Paddle& aiPaddle, const Ball& ball, int playerScore, int aiScore, bool gameStarted, int countdown, float countdownAlpha, float countdownFontSize)
{
	//Player Paddle
	DrawRectangleRounded(
		Rectangle{ static_cast<float>(playerPaddle.x), static_cast<float>(playerPaddle.y), static_cast<float>(playerPaddle.width), static_cast<float>(playerPaddle.height) },
		0.5f,
		10,
		playerPaddle.color
	);

	//AI Paddle
	DrawRectangleRounded(
		Rectangle{ static_cast<float>(aiPaddle.x), static_cast<float>(aiPaddle.y), static_cast<float>(aiPaddle.width), static_cast<float>(aiPaddle.height) },
		0.5f,
		10,
		aiPaddle.color
	);

	// Ball
	DrawCircle(ball.x, ball.y, ball.size, ball.color);

	// Scoreboard
	DrawText(TextFormat("%i", playerScore), SCREEN_WIDTH / 4, 20, 40, DARKGRAY);
	DrawText(TextFormat("%i", aiScore), 3 * SCREEN_WIDTH / 4, 20, 40, DARKGRAY);

	// Remaining Time
	if (!gameStarted && countdown > 0)
	{
		Color countdownColor = Fade(RED, countdownAlpha);
		DrawText(TextFormat("%i", countdown),
			SCREEN_WIDTH / 2 - MeasureText(TextFormat("%i", countdown), static_cast<int>(countdownFontSize)) / 2,
			SCREEN_HEIGHT / 2 - countdownFontSize / 2,
			static_cast<int>(countdownFontSize),
			countdownColor);
	}
}
