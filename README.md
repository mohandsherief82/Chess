# Terminal Chess Engine

- Added a copy function for board and player (Reference: https://www.geeksforgeeks.org/cpp/memcpy-in-cc/)

## Overview

* ## Features

### Pieces

* Since each piece represents its own entity, each type of piece has its own structure that follows a general piece structure.

~~~c
typedef struct
{
    char symbol;
    PieceColor color;
    int rowPosition; 
    int colPosition;
    bool isActive;
    bool isPinned;
} Piece;
~~~

* The idea behind this design: as pieces have different positions during the game, we use the `rowPosition` and `colPosition` members to store their position on the chess board.
* Also, we need an indicator for the state of the piece as it can be captured by the opponent or pinned; for this reason, two flags have been added for each piece.
* All pieces follow this design schema. However, there are some pieces that require extra data.
* Each piece has its own initializer function for creating its initial position and data.
* This general piece structure will be useful later as it will be used for adding pieces to the board.

#### Pawn Piece

* The pawn piece has some special members that make it different from other pieces; it can be promoted, and for its first move, it can move two cells forward. Therefore, we have two extra special members that serve as indicators for promotion and the first move.

~~~c
typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition;
    int colPosition;
    bool isActive;
    bool isPinned;
    bool promoted;
    bool firstMove;
} Pawn;
~~~

#### King Piece

* The king piece also has some special members, as it can castle on its first move and needs an indicator for its check state. As a result, it includes a member for its first move (for castling) and a check flag.

~~~c
typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition;
    int colPosition;
    bool isActive;
    bool isChecked;
    bool firstMove;
} King;
~~~

### Player

* To store the player data, we chose to create a struct with 7 members. Two of them are pointers to a dynamically allocated memory block and four are dynamically allocated arrays, which makes the manipulation of piece data easier later on. There is also a member that acts as a color indicator.
* The player also has its own initializer function, which assigns and initializes each piece and stores their addresses in their respective members and correct positions.
* To prevent memory leaks, we created a function that clears the dynamically allocated members.

~~~c
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
~~~

### Board

* The board is created as a standard 2D array that stores the symbols of the pieces; a white square is represented by **"-"** and a black square is represented by **"."**.
* White pieces are represented by lowercase letters, while black pieces are represented by uppercase letters.
* The board array has its own initializer function, which dynamically allocates the board, and a freeing function to empty the board at the end of the game.
* The board will be initialized with empty squares/spaces that will be used later to determine if a cell is white or black.
* The `addPiece` function has been created to add pieces to the board using the base piece structure, which acts as an adaptable data type for different pieces.
* The `displayBoard` function prints the board in the center of the terminal with the move log on its left and the captured piece list on the right. It also shows the white captures in the bottom-right corner of the display, while the black captures are shown above the white captures.
* A clear screen function was added to clear the screen after each move (Reference: https://www.geeksforgeeks.org/c/clear-console-c-language/).

### Moves

* Each move has unique data, so a structure has been created to store the piece symbol being moved, its previous position, and its destination.

~~~c
typedef struct
{
    char symbol;
    int colPrev;
    int colNext;
    int rowPrev;
    int rowNext;
} Move;
~~~

* As most pieces have a unique way of moving, each type of piece requires its own function for motion.

#### Pawn Movement

* The pawn has a couple of different moves: it can either step one cell ahead if nothing is in front of it, or, if it is the pawn's first move, it can move two cells ahead provided nothing is in its path.
* The `movePawn` function starts with a linear search in the player's pawns array to find the pawn at the specified position.
* After finding the correct pawn, it checks the pawn's color to determine its allowed direction and verifies if the move can be performed.
* As pawns can only capture on their diagonals, the capture logic is different from the main movement pattern. Therefore, it requires an extra check for empty cells and to see if the target piece is of the opposite color.
* Additionally, the pawn has promotion functionality, which involves checking if it has reached row 1 or 8.
* After promotion, the player is prompted to choose the piece type for promotion, and the pawn symbol is changed to the respective symbol based on the player's color.
* Pawn promotion might cause a problem later on as the promoted pawn will no longer move like a pawn; thus, the function `checkPromotedPawn` was created to determine if a piece is a promoted pawn before executing movement logic.

#### Knight Movement

* The knight moves in an *L* shape where the difference between two cells equals 2 in one direction and 1 in the other direction.
* The `moveKnight` function starts by finding the required knight and checks if it can be moved (e.g., if it is pinned). It then validates the L-shaped condition by calculating the difference between the current cell and the target cell in the X and Y directions. If the *L* shape is valid, it checks the destination cell; if there is an enemy piece, it is captured, but if it is a friendly piece, the move is invalid.

#### Rook Movement

* The rook moves in a *straight line* where the difference between two cells is non-zero in one direction and zero in the other.
* The `moveRock` function finds the required rook and checks if it can be moved (e.g., if it is pinned). It then validates the *straight-line* condition. If the path is valid, it checks each cell along the way. If any piece is in the path, the move cannot be performed. At the destination, it captures the piece if it is an enemy; if it is friendly, the move is invalid.

#### Bishop Movement

* The bishop moves in a *diagonal* shape where the difference between two cells is equal in both X and Y directions.
* The `moveBishop` function finds the required bishop and checks if it can be moved. It validates the *diagonal* condition by calculating the difference between the current and target cells. If the path is clear, it checks the destination. It captures enemy pieces at the destination cell but prevents moving onto a friendly piece.

#### Queen Movement

* As the queen is a combination of the rook and the bishop, it inherits the logic of their movements.

#### King Movement

* The king moves to the 8 adjacent cells. This is implemented using direction matrices for the X and Y axes. The logic moves the king and then checks whether he is in check.
* The king can also castle if it is his first move and no enemy piece is attacking the path. Castling logic involves checking if the king is in check at each step of the process.
* The most important part of king movement is the `isChecked` function, which determines if the king is under attack by looking for enemy pieces in their respective move patterns. This function is also responsible for identifying pinned pieces; it works by finding a friendly piece and continuing in that direction to see if an enemy piece is targeting the king.

#### Captures

### Game End States

* The game can end in *Stalemate* or *Checkmate*. Each has its own implementation logic.

#### Stalemate

* When the king is not in check and the player whose turn it is has no legal moves available, the game ends in a draw.
* The `checkStalemate` function checks if at least one legal move exists while the king is not in check using helper functions.
* Copy functions for the board and player are used to create deep copies of the game state for background validation.
* The `legalMove` function is an essential helper for stalemate; it iterates through every piece and attempts every possible move on the board using a copied state. If it finds a legal move, it stops immediately.

#### Checkmate

* Checkmate logic flags a game over if the king is in check and no legal moves are available (using the `legalMove` and `isChecked` functions).
* The legal move function covers all types of piece moves, including blocks, which makes this logic possible.

### Save, Load, and Undo

#### Save

* The save function writes each validated move into a binary file (Reference: https://www.programiz.com/c-programming/c-file-input-output).

#### Load

* The load function reads moves from the game file. It determines whose turn it is by counting the moves read; if an odd number of moves is read, it is the second player's turn, whereas a completed pair of moves returns the turn to the first player(Reference: https://www.programiz.com/c-programming/c-file-input-output).

#### Undo