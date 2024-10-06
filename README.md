
# Pong Game

This repository contains a classic Pong game developed using C++ and the Raylib library. The project aims to provide a fun, interactive, and customizable version of the retro game with additional features such as AI-controlled paddles, particle effects, and multiple game modes.

## Features
- **Single-Player Mode**: Play against an AI paddle with adjustable difficulty levels (Easy, Medium, Hard).
- **Two Game Modes**: Choose between:
  - **Time-Limited Mode**: Play until the timer runs out.
  - **Score-Limited Mode**: First to reach a set score wins.
- **Countdown Timer**: The game includes a countdown before each round begins.
- **Particle Effects**: Collisions between the ball and paddles generate visual particle effects.
- **Pause, Reset, and Fullscreen Options**: In-game controls allow for pausing, restarting, and toggling fullscreen mode.

## Code Overview

### Main Components
1. **Main Game Loop (`main.cpp`)**:  
   The entry point of the game. Initializes the game window, manages different game screens (Menu, Play, End, etc.), and handles user inputs for navigation and gameplay.

2. **Game Logic (`game.cpp` & `game.h`)**:  
   Contains the core game logic, including the main update loop, paddle and ball collision detection, score updates, and drawing of game elements on the screen.

3. **Paddle (`paddle.cpp` & `paddle.h`)**:  
   Defines the paddle structure and its properties (position, size, speed, color). Handles player and AI paddle movements.

4. **Ball (`ball.cpp` & `ball.h`)**:  
   Defines the ball structure, its properties, and behavior, including collision responses with walls and paddles.

5. **Particle System (`particle.cpp` & `particle.h`)**:  
   Implements particle effects that are triggered during collisions, adding a visual flair to the game.

### Controls
- **Movement**: Use `W` and `S` keys to move the player's paddle up and down.
- **Pause**: Press `P` to pause the game.
- **Reset**: Press `R` to reset the game and return to the main menu.
- **Quit**: Press `Q` to quit the current game and return to the main menu.
- **Fullscreen**: Press `F` to toggle fullscreen mode.
- **Menu Navigation**: Use `ENTER` to select options.

### How to Build and Run
1. **Requirements**: 
   - Make sure you have C++ and Raylib installed on your system.
2. **Building**:
   - Compile all `.cpp` files and link against the Raylib library.
     ```bash
     g++ main.cpp game.cpp paddle.cpp ball.cpp particle.cpp -o pong -lraylib
     ```
3. **Running the Game**:
   - Execute the compiled binary to start the game.
     ```bash
     ./pong
     ```

## Project Structure
```
.
├── assets/             # Folder for assets like the game icon.
├── main.cpp            # Entry point of the game.
├── game.cpp/.h         # Core game logic.
├── paddle.cpp/.h       # Paddle structure and behavior.
├── ball.cpp/.h         # Ball structure and collision logic.
├── particle.cpp/.h     # Particle effect system.
└── README.md           # Project documentation (this file).
```

## Customization
- **Difficulty Levels**: Change the AI difficulty by modifying the `Difficulty` enum and its corresponding behavior in `game.cpp`.
- **Game Modes**: Customize the `GameMode` enum to introduce new play styles or adjust the current modes.

## Credits
- Developed by Mert.
- Built with [Raylib](https://www.raylib.com/).

Feel free to modify the code and enhance the game as you wish!
