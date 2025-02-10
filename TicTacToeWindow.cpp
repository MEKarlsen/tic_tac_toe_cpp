#include "TicTacToeWindow.h"
#include <iostream>

constexpr int BUTTON_WIDTH = 100;
constexpr int BUTTON_HEIGHT = 30;
constexpr int RESET_BUTTON_X = 10;
constexpr int RESET_BUTTON_Y = 10;
constexpr int QUIT_BUTTON_X = 120;
constexpr int QUIT_BUTTON_Y = 10;
constexpr int WINNER_TEXT_X = 240;
constexpr int WINNER_TEXT_Y = 10;

constexpr int BOARD_SIZE = 600;
constexpr int CELL_SIZE = BOARD_SIZE / 3;
constexpr int BOARD_X = 20;
constexpr int BOARD_Y = 60;
constexpr int FONT_SIZE = 100;

TicTacToeWindow::TicTacToeWindow(int x, int y, int width, int height, const std::string& title)
    : AnimationWindow(x, y, width, height, title), current_player('X'), reset_button({RESET_BUTTON_X, RESET_BUTTON_Y}, BUTTON_WIDTH, BUTTON_HEIGHT, "Reset"), quit_button({QUIT_BUTTON_X, QUIT_BUTTON_Y}, BUTTON_WIDTH, BUTTON_HEIGHT, "Quit"), game_over(false) { 
    reset_game();
    add(reset_button);
    add(quit_button);
    reset_button.setCallback(std::bind(&TicTacToeWindow::cb_reset, this));
    quit_button.setCallback(std::bind(&TicTacToeWindow::cb_quit, this));
}

void TicTacToeWindow::play() {
    while (!should_close()) {
        if (is_left_mouse_button_down() && !game_over) {
            auto [x, y] = get_mouse_coordinates();
            if (x >= RESET_BUTTON_X && x <= RESET_BUTTON_X + BUTTON_WIDTH && y >= RESET_BUTTON_Y && y <= RESET_BUTTON_Y + BUTTON_HEIGHT) {
                cb_reset();
            } else if (x >= QUIT_BUTTON_X && x <= QUIT_BUTTON_X + BUTTON_WIDTH && y >= QUIT_BUTTON_Y && y <= QUIT_BUTTON_Y + BUTTON_HEIGHT) {
                cb_quit();
            } else {
                try {
                    handle_click(x, y);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }
        }

        if (check_winner() && !game_over) {
            game_over = true;
            winner_text = "Player " + std::string(1, last_player) + " wins!";
            write_result_to_file(winner_text);
        } else if (check_draw() && !game_over) {
            game_over = true;
            winner_text = "The game is a draw!";
            write_result_to_file(winner_text);
        }

        draw_board();
        draw_marks();

        if (game_over) {
            draw_winning_line();
        }
        draw_text({WINNER_TEXT_X, WINNER_TEXT_Y}, winner_text, TDT4102::Color::black, 30);

        next_frame();
    }
}

void TicTacToeWindow::draw_board() {
    for (int i = 1; i < 3; ++i) {
        draw_line({BOARD_X + i * CELL_SIZE, BOARD_Y}, {BOARD_X + i * CELL_SIZE, BOARD_Y + BOARD_SIZE}, TDT4102::Color::black);
        draw_line({BOARD_X, BOARD_Y + i * CELL_SIZE}, {BOARD_X + BOARD_SIZE, BOARD_Y + i * CELL_SIZE}, TDT4102::Color::black);
    }
}

void TicTacToeWindow::draw_marks() {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board[row][col] == 'X') {
                draw_text({BOARD_X * 3 / 2 + col * CELL_SIZE + FONT_SIZE / 2, BOARD_Y + row * CELL_SIZE + FONT_SIZE / 2}, "X", TDT4102::Color::red, FONT_SIZE);
            } else if (board[row][col] == 'O') {
                draw_text({BOARD_X * 3 / 2 + col * CELL_SIZE + FONT_SIZE / 2, BOARD_Y + row * CELL_SIZE + FONT_SIZE / 2}, "O", TDT4102::Color::blue, FONT_SIZE);
            }
        }
    }
}

void TicTacToeWindow::draw_winning_line() {
    draw_line(winning_line.first, winning_line.second, TDT4102::Color::yellow);
}

void TicTacToeWindow::handle_click(int x, int y) {
    int row = (y - BOARD_Y) / CELL_SIZE;
    int col = (x - BOARD_X) / CELL_SIZE;

    if (row < 0 || row >= 3 || col < 0 || col >= 3) {
        throw std::out_of_range("Invalid board position");
    }

    if (board[row][col] == ' ') {
        board[row][col] = current_player;
        last_player = current_player; 
        if (current_player == 'X') {
            current_player = 'O';
        } else {
            current_player = 'X';
        }
    }
}

bool TicTacToeWindow::check_winner() {
    // Check rows and columns
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            winning_line = {{BOARD_X, BOARD_Y + i * CELL_SIZE + CELL_SIZE / 2}, {BOARD_X + BOARD_SIZE, BOARD_Y + i * CELL_SIZE + CELL_SIZE / 2}};
            return true;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            winning_line = {{BOARD_X + i * CELL_SIZE + CELL_SIZE / 2, BOARD_Y}, {BOARD_X + i * CELL_SIZE + CELL_SIZE / 2, BOARD_Y + BOARD_SIZE}};
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        winning_line = {{BOARD_X, BOARD_Y}, {BOARD_X + BOARD_SIZE, BOARD_Y + BOARD_SIZE}};
        return true;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        winning_line = {{BOARD_X + BOARD_SIZE, BOARD_Y}, {BOARD_X, BOARD_Y + BOARD_SIZE}};
        return true;
    }

    return false;
}

bool TicTacToeWindow::check_draw() {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == ' ') {
                return false;
            }
        }
    }
    return true;
}

void TicTacToeWindow::reset_game() {
    std::cout << "Resetting game..." << std::endl; 
    board = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));
    current_player = 'X';
    game_over = false;
    winner_text.clear();
    std::cout << "Game reset complete." << std::endl;
}

void TicTacToeWindow::write_result_to_file(const std::string& result) {
    try {
        std::ofstream file("game_results.txt", std::ios::app);
        if (!file) {
            throw std::ios_base::failure("Failed to open file");
        }
        file << result << std::endl;
    } catch (const std::ios_base::failure& e) {
        show_error_dialog("Error writing to file: " + std::string(e.what()));
    }
}

void TicTacToeWindow::cb_reset() {
    std::cout << "Reset button pressed." << std::endl;
    reset_game();
}

void TicTacToeWindow::cb_quit() {
    std::cout << "Quit button pressed." << std::endl; 
    close();
}
