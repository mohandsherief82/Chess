# Terminal Chess Engine

- Added a copy function for board and player (Reference: https://www.geeksforgeeks.org/cpp/memcpy-in-cc/)

## Overview

* 

## Features

### Pieces

* Since Each piece represents its own entity, so for each type of piece has its own structure that follow a general piece st
    *promotionSymbol = pawn->symbol;ructure.

~~~
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

* The idea behind this desgin: as pieces have different positions during the game so using the rowPosition and colPosition members we stored its position on the chess board.
* Also we need an indicator for the state of the piece as it can be captured by the opponent or is pinned, for this reason two flags has been add for each piece.
* All pieces follow this design schema. However, there are some pieces that needed extra data.
* Each piece has its own initializor function for creating its initial position and data.
* This general piece structure will be useful later as it will be used for the addition of the pieces to the board.

#### Pawn Piece

* The pawn piece have some special memebers that makes it different from the other pieces as it can get promoted or for its first move it can move two cells forward, so we have two extra special members that are indicators for promotion and first move.

~~~
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

* The king piece also have some special members, where it can castle on its first move and it needs an indicator for its check state. As a result, a member for its first move for castling and a check flag.

~~~
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

* To store the player data, we choose to create a struct with 7 members, where 2 of them are pointers to a dynamically allocated memory block, 4 are dynamically allocated arries, which made the manipulation of piece data easier later on, and a member that act as a color indicator.
* The player also has its own initializor function, which assign and initialize each piece and store there addresses in their respective members and correct positions.
* For memroy leakage, we created a function that clears the dynamically allocated members.

~~~
typedef struct
{
    const PieceColor color;
    Pawn *pawns;
    Rock *rocks;
    Knight *knights;
    Bishop *bishops;
    Queen *queen;
    King *king;
} Player;
~~~

### Board

* The board is created as a normal 2D array that stores the symbols of the pieces, a white square represented by **"-"** or a black square represented by **"."**.
* White pieces are represented by lowercase letters, while black pieces are represented by uppercase letters.
* The board array has its own initializor function, which will dynamically allocate the board, and a freeing function to empty the board at the end of the game.
* The board will be initialized with empty squares/ spaces that will be used later to determine if the cell is white or black.
* The *addPiece* function has been created to add the pieces to the board, using the base piece structure which acts as an adaptable data type for different pieces.
* The *displayBoard* function prints the board in the center of the terminal with the move log on its left and the captured piece list on the right. It also shows the white captures in the right lower corner of the display, while the black captures on top of the white captures.
* A clear screen function was added to the clear the screen after each move (Reference: https://www.geeksforgeeks.org/c/clear-console-c-language/).

### Moves

* Each move has its own important data that is unique to it, so a structure has been created to store the piece symbol to move, its previous position and where to move the piece.

~~~
typedef struct
{
    char symbol;
    int colPrev;
    int colNext;
    int rowPrev;
    int rowNext;
} Move;
~~~

* As most pieces have their own special way of motion, each type of piece will need its own function for motion.

#### Pawn Movement

* The pawn has a couple of different moves, as it can either step one cell ahead with nothing in front of it, or in case of that it is the pawns first move, it can move two cells ahead without anything in its path.
* The *movePawn* function starts with a linear search in the player pawns array to find the pawn at the specified position.
* After finding the correct pawn, it checks the pawn's color to determine in which direction it actually can move and starts to check if the move can be performed.
* As pawn can only capture on its diagonals, capture logic is different from its main movement pattern. Therefore, it needed an extra check which involves a check for empty cells and if the piece is of the oppisite color.
* Also the pawn has the funcionality for promotion, which involves a check for it reaching either the row 1 or 8 and checking the color to check.
* After the promotion, the player will be directly asked on which type of piece he wants to promote his pawn to and change the pawn symbol to the respective symbol based on the color of the player.
* The pawn promotion might cause a problem later on as the promoted pawn won't be moving as a pawn anymore, so the function *checkPromotedPawn* was created to allow with other piece calls to check whether the piece the playe wants to move is a promoted pawn first then continue the logic to move the piece whether its is a promoted pawn or the actual piece itself.

#### Knight Movement

* The knight moves in an *L* shape so that the difference between two cells equals 2 in one direction, and the difference equals 1 in the other direction.

* The *moveKnight* function starts by finding the required knight that we want to move and then checks if it can be moved (e.g., pinned), then checks the L-shaped condition by calculating the difference between the current cell and the required cell in the X and Y directions. If the *L* shape is valid, it checks the destination cell; if there is an enemy piece, it captures it, but if it is a friendly piece, then the move cannot be done.

#### Rock Movement

* The rock moves in a *straight line* shape so that the difference between two cells should not be equal to *zero* in one direction, and the difference should be equal to *zero* in the other direction.

* The *moveRock* function starts by finding the required rock that we want to move and then checks if it can be moved (e.g., pinned) and then checks the *straight-shaped* condition by calculating the difference between the current cell and the required cell in the X and Y directions. If the diagonal shape is valid, it checks each cell on the way. If there exists any piece on the way, the move cannot be done until it reaches the destination cell. If there exists a piece, it captures it in case it's an enemy one; if it's friendly, then the move cannot be done.

#### Bishop Movement

* The bishop moves in a *diagonal* shape so that the difference between two cells should be equal in both X and Y directions.

* The *moveBishop* function starts by finding the required bishop that we want to move and then checks if it can be moved (e.g., pinned), then checks the *diagonally* shaped condition by calculating the difference between the current cell and the required cell in X and Y directions. If the diagonal shape is valid, it checks each cell on the way. If there exists any piece on the way, the move cannot be done until it reaches the destination cell. If there exists a piece, it captures it in case it's an enemy one; if it's friendly, then the move cannot be done.

#### Queen Movement

* As the queen is basically a combination between the rock and the bishop, it inherited the logic of their movements.

#### King Movement

* The king moves in the 8 cells around him, so his movement pattern can be done using a direction matrices one for the x axis and one for the y axis, the logic was implemented by moveing the king then checking whether he is checked or not.
* Also the king can castle, if it is his first move and there is no enemy piece cutting it of, the logic for castling has been implemented by mutliple king moves then checking if the king is checked in each position and if he is checked the castling act is stopped.
* The most important piece for the king motion is the isChecked function, which checks whether the king is checked by looking out for enemy pieces on their move patterns for example the rock on the horizontal and vertical axis around it, this function also is responisble for piece pinning, and it works by finding a friendly piece then continue traversing in the direction to see if an enemy piece that can be blocked is in the way and if it finds a friendly piece it stops checking.

#### Captures

### Game end states

* The game could end by *Stalemate* or *Checkmate*, each of these has its own function and logic in implementation.

#### Stalemate

* When the king is not in check, and the player whose turn it is has no legal moves available, the game ends with a draw.

* The *checkStalemate* function checks if there exists at least one legal move and the king is not in check; this happens with helper functions.

* The copy board/player functions, which are auxiliary functions helping us to copy all the content of the player/board.

* The *legalMove* function, which is an essential helping function in stalemate, checks every possible move that can happen by iterating through every piece and trying to move it around all the cells of the board. This happens on the copied board/player, which happens in the background of the game while it's being played without affecting it, as we are working on a deep copy of the board and player. If the function finds a legal move, it stops immediately; if not, it continues searching until it completes the iteration. The true/false value returning from this function is helping us check for stalemate, which happens in the *checkStalemate* function.


#### Checkmate

* It depends on the isChecked function and legalMove function that is implemented alone and with the stalemate logic respectively, and the logic flags a checkmate if the king is in check and there is no legal moves that can be done.

* The legal move function covers all types of piece moves including blocks, which make this logic possible.
