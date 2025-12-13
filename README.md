# Chess

- Since each piece represents its own entity with its own variable, we are going to create a struct for each piece and a struct for a player.

- Each of the structs will have a special initialization function which will be used for each new instance of the struct to be created.

- As we have pieces that are repeated multiple times, we are going to create in the player structure an array for each of these pieces, in which the pointer of the first element will be stored in the player structure.

- The pieces arrays that exist in the player structure will be dynamically allocated in the player's initialization function.

- The board will need to be initialized once at the start of the game, so before printing or doing any moves, we are going to initialize all cells with an empty cell represented as a space.

- The initialization function of the board will only give us a pointer to the first address of the 2D grid, which is done to keep track of the place of the board across the whole runtime.

- Added a function to clear the terminal each time a new move is made (Reference: https://www.geeksforgeeks.org/c/clear-console-c-language/).

- Added a copy function for board and player (Reference: https://www.geeksforgeeks.org/cpp/memcpy-in-cc/)

- File handling in C (Reference: https://www.geeksforgeeks.org/c/basics-file-handling-c/)