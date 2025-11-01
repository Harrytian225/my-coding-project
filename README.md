A command-line Go (Weiqi) game implemented in C++, supporting two-player gameplay with complete Go rules.

## 🎮 Features

- **Two-player mode** - Black and white take turns to place stones
- **Complete Go rules** - Including ko, capture, suicide rule detection
- **Real-time board display** - Graphical interface showing board state
- **Pass functionality** - Players can choose to skip their turn
- **Resign functionality** - Players can resign to end the game
- **Ko rule enforcement** - Complete ko detection and infinite loop prevention
- **Compilation Note:** The source code utilizes C++11 features including `auto` type deduction. Please ensure your compiler supports C++11 or later standards.

## 🚀 Quick Start

### Compilation & Execution
```bash
# Compile the project
g++ -std=c++11 -O2 main.cpp board.cpp game.cpp player.cpp -o go_game

# Run the game
./go_game
