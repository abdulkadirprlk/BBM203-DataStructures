#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"
#include "Leaderboard.h"
#include "LeaderboardEntry.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay

    std::vector<std::string> read_commands(const string& commands_file);// function to read commands file
    // function to check if a move(move_right, move_left, rotate_right, rotate_left) is valid or not
    bool is_valid_move(const std::vector<std::vector<int>>& grid, const Block* block, int row, int col);
    // function to place a block into the grid
    void place_block(std::vector<std::vector<int>>& grid, const Block* block, int row, int col);
    // function to calculate the points
    long calculate_points(const Block* block, int moving_distance);
    // function to check whether power-up is recognizable
    bool is_power_up(const vector<vector<int>>& grid, const vector<vector<bool>>& power_up_shape);
    // function to clear rows of filled and calculate the score 
    long clear_filled_rows_and_calculate(std::vector<std::vector<int>>& grid);
    //function to clear the grid after power-up
    long clear_grid(std::vector<std::vector<int>>& grid);
    // function to drop the cells if gravity switches to true
    void drop_cells_gravity(std::vector<std::vector<int>>& grid);
    void print_game_over(BlockFall& game, Block* active_rotation, Leaderboard& game_leaderboard);
    void print_block(const Block* block);
    void print_game_end(BlockFall& game, Block* iter, Leaderboard& game_leaderboard);
    bool is_grid_full(const std::vector<std::vector<int>>& grid, const Block* block);
    // function to move the blocks according to the gravity on
    long move_down_with_gravity(std::vector<std::vector<int>>& grid, Block* active_block, int active_block_row, int active_block_col);
    void print_grid(BlockFall& game, const Block* active_block, int active_block_row, int active_block_col);// function to print the game grid
    void print_grid_without_block(std::vector<std::vector<int>>& grid);
    std::vector<std::string> commands_vector;// vector will contain all the commands


};


#endif //PA2_GAMECONTROLLER_H
