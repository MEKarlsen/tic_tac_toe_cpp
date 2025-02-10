#include "TicTacToeWindow.h"

void play_tick_tack_toe() {
    int x = 100;
    int y = 50;
    int width = 620;
    int height = 660;
    std::string title = "Tic Tac Toe";

    TicTacToeWindow game(x, y, width, height, title);
    game.play();
}

int main() {
    play_tick_tack_toe();
    return 0;
}
