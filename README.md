# ltnc25 - Tetris Clone (C++/SDL2)

A fully functional Tetris clone built using C++ and the SDL2 library. This project was developed as a technical project during my freshman year at the University of Engineering and Technology, VNU.

## Project Structure
The project is organized to separate source code, assets, and dependencies for better maintainability.

```text
ltnc25/
├── assets/             # Contains images, sounds, and fonts
├── libs/               # Contains necessary SDL2 .dll files
├── src/                # Contains all C++ source code (.cpp, .h)
├── GAME.sln            # Visual Studio Solution file
├── GAME.vcxproj        # Visual Studio Project configuration
└── README.md           # Project documentation
```
## Core Features
* **Dual-Input System:** Supports both mouse-click navigation and keyboard controls (Arrow Keys, Space, Enter) for the Menu and Game UI.
* **Game Mechanics:** * Dynamic difficulty scaling across 20 levels.
    * Real-time tracking of score, level, and cleared lines.
    * Custom scoring formula: `Points = Cleared Lines * (Level + 9) / 10`.
* **Game Loop & Logic:** Modular design separating game states, rendering, and input handling.
* **Responsive UI:** Includes a live game timer, pause/resume sound functionality (M key), and post-game results screen.

## Module Descriptions (src/)
* **main.cpp**: Application entry point.
* **game.h/cpp**: Orchestrates the main game loop, font rendering, and audio management.
* **board.h/cpp**: Manages the grid logic and state of the playing field.
* **tetromino.h/cpp**: Handles piece movement, rotation, and position validation.
* **queue.h/cpp**: Implements the "Next Piece" generation logic.
* **pieces.h**: Defines the geometry and rotation matrices for all tetromino shapes.
* **GameUI.h/cpp**: Handles rendering the HUD, scoreboards, and game-over screens.
* **constants.h**: Defines global configuration, window dimensions, and game constants.

## Getting Started
1. Open **GAME.sln** using Visual Studio.
2. Ensure the project **Include Directories** are set to point to the `src/` folder.
3. Set the **Working Directory** to the project root so the game can locate the `assets/` folder.
4. Build and run the solution.

## Credits & Resources
* **Sound**: [Tetris Theme Remix](https://www.youtube.com/watch?v=Ci5squuWW3Q)
* **Graphics**: Original assets created via ChatGPT.
* **Font**: [Press Start 2P](https://fonts.google.com/specimen/Press+Start+2P)
