# MinefieldGame_C_multithreading

Introduction:

The minefield game is a popular puzzle game where players navigate a grid filled with hidden mines. The objective is to reveal all the safe tiles without triggering a mine. This project aims to develop a multi-threaded implementation of the minefield game using the C programming language on the Ubuntu operating system.

Problem Statement:

The problem at hand is to design and implement a minefield
game with the following requirements:

● Create a grid of tiles with a specified size.
● Randomly place a predefined number of mines in the grid.
● Allow players to reveal tiles by entering the row and column
coordinates.
● Update the game state and display the grid after each tile
reveal.
● Calculate the score based on the revealed tiles.
● Implement multi-threading to handle user input and grid
processing concurrently.
● Ensure thread safety and avoid race conditions while
accessing shared data structures.

Functionalities

The minefield game project offers the following functionalities:
Start a New Game:
● Initializes the grid with tiles and randomly places mines.
● Creates threads for user input and grid processing.
● Allows players to reveal tiles by entering row and column
coordinates.
● Updates the game state, including revealing tiles, calculating
score, and checking for game over conditions.
● Displays the grid after each tile reveal.
● Ends the game and displays the final score when all safe
tiles are revealed or a mine is triggered.
Game Instructions:
● Provides instructions on how to play the minefield game.
● Explains the objective, rules, and scoring system of the
game.
● Helps users understand the game mechanics and make
informed decisions while playing.
Exit:
● Allows the user to exit the game.
● Cleans up resources and terminates the program.

Implementation Details

● Threads: The program utilizes multiple threads to handle
user input and grid processing concurrently, enhancing the
responsiveness and performance of the game.
● Mines: The grid is populated with hidden mines, randomly
distributed throughout the game board, adding an element of
challenge and unpredictability to the gameplay.
● Mutexes: Mutexes (mutual exclusion locks) are employed to
ensure thread safety and avoid race conditions when
accessing and modifying shared data structures such as the
game grid and score. Mutexes provide synchronization and
exclusive access to critical sections of the code, preventing
simultaneous access and potential conflicts.
● Grid Processing: A dedicated thread is responsible for
processing the grid, calculating the number of adjacent
mines for each tile that is not a mine. This information is
crucial for determining the displayed number on each
revealed tile and helps the player strategize their moves.
● User Input: Another thread handles user input, allowing
players to select a tile by specifying its row and column. The
input is validated, and the corresponding tile is revealed. If a
mine is triggered, the game ends, and the final score is
displayed.
