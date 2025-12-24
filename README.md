Terminal Chess Engine Technical Report
======================================

#### **CSE211: Programming I**

#### [Github Repository](https://github.com/mohandsherief82/Chess)

Project Contributors & Supervision
----------------------------------

### Supervision

*   **Prof. Dr. Marwan Torki**
*   **Eng. Karim Alaa**

### Contributors

*   **Mohand Sherief Mohamed Ali**
*   **Moaz Gaballah Ahmed**

Table of Contents
-----------------

*   [1\. Overview](#overview)
*   [2\. Technical Features](#technical)
*   [3\. Build System](#build)
*   [4\. Movement Logic](#movement)
*   [5\. Game End States](#endstates)
*   [6\. Persistence](#persistence)
*   [7\. User Manual](#manual)
*   [8\. Snapshots](#snapshots)
*   [9\. References](#references)

1\. Overview
------------

The **Terminal Chess Engine** is a comprehensive board game application implemented in C. It translates the high-level complexity of Chess into a modular, command-line interface. The engine supports full rules, including specialized movements like castling, en passant, and pawn promotion.

Key technical highlights include a recursive-style move validation system, binary file persistence for saving and loading game states, and a robust "Undo" feature that uses file truncation to safely revert moves back to the starting position.

The main game logic involves asking user whether they want to play a new game or load an old one, then it gives the turn for the white player and keeps alternating the turns between black and white untill checkmate, stalemate or the user choose to save and end the game

2\. Technical Features & Data Structures
----------------------------------------

### 2.1 Piece Architecture

To maintain a high level of abstraction, each chess piece is represented by its own `struct`. This allows the movement logic to treat different pieces through a different interface. However, in some function a unified struct would be better, so the following struct was created as a base struct that all pieces follow, except specialized pieces

    typedef struct
    {
        char symbol;      // e.g., 'p', 'r', 'N'
        PieceColor color; // Enum for WHITE or BLACK
        int rowPosition; 
        int colPosition;
        bool isActive;    // Tracks if the piece is still on the board
        bool isPinned;    // Flag used by Check-detection
    } Piece;

#### Specialized Pieces

Certain pieces require state-tracking beyond the base structure:

*   **Pawn Piece**: Includes promoted (to handle movement changes) and firstMove (to allow the initial 2-square jump and En Passant logic).
*   **King Piece**: Includes `isChecked` for UI alerts and `firstMove` to validate castling rights.
*   **Rook Piece**: Includes `firstMove` to validate castling rights.

### 2.2 Player Management

The `Player` structure acts as a container for all 16 pieces belonging to a user, utilizing dynamic memory allocation to manage the piece arrays.

For the player struct exists a function for initialization and clear at the end of the game, which makes memmory management easier and more robust.

    typedef struct
    {
        const PieceColor color;
        Pawn *pawns;
        Rook *rooks;
        Knight *knights;
        Bishop *bishops;
        Queen *queen;
        King *king;
    } Player;

### 2.3 The Chessboard

The board is the most important aspect of the chess game that is needed for the game in a specific form:

*   **Representation**: The board is represented as a dynamic 2D char array of size 8 x 8.
*   **Visual Styling:** White squares are rendered using "-" and black squares using ".".
*   **Updating**: The `addPiece` function serves as the primary interface for updating the board array based on the current state of the Player structures.
*   **UI/UX**: The `displayBoard` function handles the complex task of centered rendering. It utilizes terminal escape codes to clear the screen, providing a seamless "animation-like" feel. The interface is split into three zones: the Move Log (left), the Active Board (center), and Captured Pieces (right).

Also the board has its own initialization function and free function.

With some functions we needed to work on a copy of the board for safer checks without altering the original board, so a copy and free copy functions were added.

### 2.4 Moves

A move struct was added to store all the move data that is needed to make a move and some extra data that would be helpful when loading the data from the folder:

    typedef struct
    {
        char symbol;      // e.g., 'p', 'r', 'N'
        int colPrev; 
        int rowPrev;
        int colNext; 
        int rowNext;
        char promotedPawn;    // For storing promotion data in the saved moves log
    } Move;

This struct is one of the most important for the game to actually function, so we create for it its own function called `getMove`, which takes a piece symbol previous coordinates and next coordinates without actually seeing if the move is actually valid for this piece it only validates the move to be on board and the correct piece symbol was inputed.

This function also clears the buffer after it takes each input, ensures that every part of the data it stored is in lower case.

The `getMove` is the most tested function in the entire project

3\. The Build System (Makefile)
-------------------------------

The project uses a modular `Makefile` to manage compilation. In addition to the primary game targets, several internal targets exist to test specific logic modules independently during development.

> **Note:** Targets marked as _Internal/Testing_ were used during intermediate development phases. Some may require specific directory structures or mock data to execute correctly and may not be as stable as the primary `run` target.

Target

Command

Category

Purpose

**run**

`make run`

**Primary**

Compiles all modules and launches the full Chess game.

**compile**

`make compile`

**Primary**

Links all source files and generates the `run.o` executable.

**clean**

`make clean`

**Utility**

Wipes all binaries in subdirectories and root, then clears terminal.

**game\_end**

`make game_end`

Internal

Builds the test suite for Checkmate and Stalemate logic.

**moves**

`make moves`

Internal

Compiles move-validation logic for unit testing.

**board**

`make board`

Internal

Compiles board rendering and coordinate mapping modules.

**pieces**

`make pieces`

Internal

Tests individual piece behavior and structure initialization.

**test\_\[module\]**

`make test_board` etc.

Testing

Automated targets that compile and execute test binaries.

4\. Movement Logic
------------------

### 4.1 Pawn Movement & Promotion

The `movePawn` function is the most complex movement module. It validates:

*   Forward Steps: 1-square or 2-square jumps (only on firstMove).
*   Diagonal Captures: Only valid if an opponent occupies the target square.
*   Promotion: Triggered when a pawn reaches the terminal ranks (Row 0 or 7).
*   The `checkPromotedPawn` helper ensures that once a pawn is promoted, it adopts the movement logic of the new piece type.

### 4.2 Rook Movement

The rook moves in a _straight line_ shape so that the difference between two cells should not be equal to _zero_ in one direction, and the difference should be equal to _zero_ in the other direction.

The `moveRook` function starts by finding the required rook that we want to move and then checks if it can be moved (e.g., pinned) and then checks the _straight-shaped_ condition by calculating the difference between the current cell and the required cell in the X and Y directions. If the _straight-shaped_ shape is valid, it checks each cell on the way. If there exists any piece on the way, the move cannot be done until it reaches the destination cell. If there exists a Piece, it captures it in case it's an enemy one; if it's friendly, then the move cannot be done.

### 4.3 The Knight's Leap

The knight moves in an _L_ shape so that the difference between two cells equals 2 in one direction, and the difference equals 1 in the other direction.

The `moveKnight` function starts by finding the required knight that we want to move and then checks if it can be moved (e.g., pinned), then checks the L-shaped condition by calculating the difference between the current cell and the required cell in the X and Y directions. If the _L_ shape is valid, it checks the destination cell; if there is an enemy piece, it captures it, but if it is a friendly piece, then the move cannot be done.

### 4.4 Bishop Movement

The bishop moves in a _diagonal_ shape so that the difference between two cells should be equal in both X and Y directions.

The `moveBishop` function starts by finding the required bishop that we want to move and then checks if it can be moved (e.g., pinned), then checks the _diagonal_ shaped condition by calculating the difference between the current cell and the required cell in X and Y directions. If the diagonal shape is valid, it checks each cell on the way. If there exists any piece on the way, the move cannot be done until it reaches the destination cell. If there exists a piece , it captures it in case it's an enemy one; if it's friendly, then the move cannot be done.

### 4.5 Queen's Sovereign Slide

The queen's move is very similar to the bishop and the rook moves, so the implementation is the same , however for faster and better performance we combined the check for the diagonal and straight line motion in one loop.

### 4.6 King Movement, Safety & Castling

*   The `moveKing` function integrates with `isChecked`.
*   **Castling**: Validates that neither the King nor Rook has moved, and ensures the King does not pass through "check" during transition.
*   **Check Detection**: The `isChecked` function performs an "inverse scan" from the King's position to see if any enemy piece has a line of sight and is repsonsible for piece pinning which is done if an enemy piece is found that can attack this position and if a friendly piece is found the search for an enemy is stopped as there is no way that the current piece can be pinned.

5\. Game End States
-------------------

**Stalemate:** occurs when a player has no legal moves but is not in check. The engine detects this by simulating every possible move using `copyBoard` and `copyPlayer`. If no moves result in a safe state, and the King wasn't in check, a draw is declared.

**Checkmate:** is confirmed using the `legalMove` helper. If the King is currently under attack (`isChecked == true`) and all simulated escape moves or captures still leave the King in check, the game ends and the winner is announced.

**Resignation:** when one of the players types 'r' in his turn, the game consider them resigned and the other player automatically wins.

6\. Persistence: Save, Load, and Undo
-------------------------------------

To ensure games can be resumed, validated moves are appended to a binary file for space efficiency.

*   **Save**: Commits the `Move` struct to disk immediately after a successful turn.
*   **Load**: Reads history sequentially and reconstructs the game state by replaying moves.
*   **Undo**: Implemented by truncating the last `sizeof(Move)` bytes from the binary file and triggering a reload to revert state and storing the data of the undone move in a separate file for redoing.
*   **Redo**: It checks if the redo file isn't empty and then read the last move from the file and then reload the game with the redone move again.

7\. User Manual
---------------

*   **Start**: Enter `p` for a new game or `l` to load a game.
*   **Movement**: Enter moves as `[Piece][Source][Destination]` (e.g., `pe2e4`).
*   **Undo**: Type `u` to revert the last turn.
*   **Redo**: Type `d` to redo the last undone move
*   **Save**: Type `s` to save progress and exit the application.
*   **Resign**: Type `r` to resign and exit the application.
*   **Promotion**: You will be prompted to enter `q`, `r`, `b`, or `n`.

**Note:** Most of the game logic is case-insensitive; inputting upper or lower case characters will not make a difference.

8\. Snapshots of the Game
-------------------------

![Main Menu](image.png)

8.1 Main Menu Interface

![Gameplay UI](image-1.png)

8.2 Active Board Rendering

![Checkmate](image-2.png)

8.3 End of Game State

* * *

9\. References
--------------

*   **C memcpy Documentation:** [GeeksforGeeks](https://www.geeksforgeeks.org/cpp/memcpy-in-cc/)
*   **Clear Console in C:** [GeeksforGeeks](https://www.geeksforgeeks.org/c/clear-console-c-language/)
*   **C File I/O (Binary):** [Programiz](https://www.programiz.com/c-programming/c-file-input-output)

**Note:** Anything that has been used in the implementation of this project was either found in the course material or inquired about in person.