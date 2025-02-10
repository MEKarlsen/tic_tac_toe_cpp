#pragma once

#include "AnimationWindow.h"
#include "widgets/Button.h"
#include <vector>
#include <fstream>
#include <stdexcept>

using namespace TDT4102;

class TicTacToeWindow : public TDT4102::AnimationWindow {
public:
    TicTacToeWindow(int x, int y, int width, int height, const std::string& title);
    void play(); 

private:
    void draw_board();
    void draw_marks();
    void draw_winning_line();
    void handle_click(int x, int y);
    bool check_winner();
    bool check_draw();
    void reset_game();
    void write_result_to_file(const std::string& result);
    void cb_reset();
    void cb_quit();

    std::vector<std::vector<char>> board;
    char current_player;
    char last_player;
    Button reset_button;
    Button quit_button;
    bool game_over;
    std::string winner_text;
    std::pair<Point, Point> winning_line;
};