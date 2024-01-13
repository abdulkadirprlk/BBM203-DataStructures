#ifndef BLOCKFALL_H
#define BLOCKFALL_H

#define occupiedCellChar "⬛"
#define unoccupiedCellChar "⬜"
//#define occupiedCellChar "██"
//#define unoccupiedCellChar "▒▒"

#include <vector>
#include <string>


#include "Block.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"

using namespace std;

class BlockFall {
public:

    BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name,
              const string &player_name);
    virtual ~BlockFall();

    int rows;  // Number of rows in the grid
    int cols;  // Number of columns in the grid
    vector<vector<int>> grid;  // 2D game grid
    vector<vector<bool>> power_up; // 2D matrix of the power-up shape
    Block * initial_block = nullptr; // Head of the list of game blocks. Must be filled up and initialized after a call to read_blocks()
    Block * active_rotation = nullptr; // Currently active rotation of the active block. Must start with the initial_block
    int active_block_row = 0;// coordinates of the active block in the grid(before dropping)
    int active_block_col = 0;
    bool gravity_mode_on = false; // Gravity mode of the game
    unsigned long current_score = 0; // Current score of the game
    string leaderboard_file_name; // Leaderboard file name, taken from the command-line argument 5 in main
    string player_name; // Player name, taken from the command-line argument 6 in main
    Leaderboard leaderboard;

    // creates a block, takes a block string ie "101\n010" as argument
    Block* make_block(const std::string& input_string);

    //generates the rotations of the original block
    Block* generate_left_rotation(Block* original_block);
    Block* generate_right_rotation(Block* original_block);

    // Function to create doubly linked lists of blocks
    Block* create_rotation_list(Block*& original_block, Block*& next_original_block);

    std::vector<std::string> splitString(const std::string& input, char delimiter); // Splits a string into a vector
    
    void initialize_grid(const string & input_file); // Initializes the grid using the command-line argument 1 in main
    void read_blocks(const string & input_file); // Reads the input file and calls the read_block() function for each block;
};



#endif // BLOCKFALL_H
