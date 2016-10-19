# Pacman
A simple Pacman based console game written in C with ncurses library.

## Build and Run Instructions

1. Open a terminal.
2. Install ncurses header files: `sudo apt-get install ncurses-dev`
2. Clone repository: `git clone https://github.com/marianodato/Pacman.git`
3. Change to project directory: `cd Pacman`
4. Maximize the terminal.
5. Change SCREEN_WIDTH and SCREEN_HEIGHT variables in Pacman.c according to your display resolution.
6. Run makefile: `make`
7. Run the game: `./Pacman`

## How to Play

- You are the yellow pacman.
- Your enemy is the white pacman.
- You move with arrow keys.
- Your goal is to eat the green fruit.
- Game finishes when you or your enemy reach 100 points. 
