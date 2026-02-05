
#include "move_wrappers.hpp"

PYBIND11_MODULE(Move_Wrappers, m)
{
    m.doc() = "Wrappers for all moving function implemented in C and moddified by C++";

    m.def("move_piece", &move_piece, "A Wrapper Function that wraps the pieces move functions all at once",
        "board_string"_a, "move_string"_a);

    m.def("check_mate", &check_mate, "Checks whether the game reached a state of checkmate."
        , "board_string"_a);
 
    m.def("check_stalemate", &check_stalemate, "Checks whether the game reached a state of checkmate."
        , "board_string"_a);
}
