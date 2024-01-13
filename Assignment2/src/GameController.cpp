#include <iostream>
#include <fstream>
#include <string>

#include "GameController.h"


std::vector<std::string> GameController::read_commands(const string& commands_file){
    // open the file
    std::ifstream inputFile(commands_file);

    std::vector<std::string> c_vector;// declare a vector to keep commands

    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return c_vector;
    }

    // read the file command by command
    std::string line;
    while (std::getline(inputFile, line)) {
        if(!line.empty()){
            c_vector.push_back(line); // these lines do not contain '\n'
        }
    }
    // Close the file
    inputFile.close();
    return c_vector;
}


void GameController::print_grid(BlockFall& game, const Block* active_block, int active_block_row, int active_block_col){
    //printing the grid

    // copy the grid to visualize the overview without modifying the actual game grid(deep copy)
    std::vector<std::vector<int>> visual_grid;

    for (const auto& row : game.grid) {
        std::vector<int> newRow(row.begin(), row.end());
        visual_grid.push_back(newRow);
    }

    // modifying the visual grid
    place_block(visual_grid, active_block, active_block_row, active_block_col);

    for (const auto& row : visual_grid) {
        for (const auto& cell : row) {
            if(cell == 1){
                std::cout << occupiedCellChar << " ";
            }
            else{
                std::cout << unoccupiedCellChar << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void GameController::print_grid_without_block(std::vector<std::vector<int>>& grid){
    //printing the grid

    for (const auto& row : grid) {
        for (const auto& cell : row) {
            if(cell == 1){
                std::cout << occupiedCellChar << " ";
            }
            else{
                std::cout << unoccupiedCellChar << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n" <<std::endl;
}


// Function to check if placing a block at a specific position is valid
bool GameController::is_valid_move(const std::vector<std::vector<int>>& grid, const Block* block, int row, int col) {
    // Check if the block can be placed within the grid boundaries
    if (row < 0 || col < 0 || row + block->shape_height > grid.size() || col + block->shape_width > grid[0].size()) {
        return false;
    }

    // Check if the cells in the grid are available for the block
    for (int i = 0; i < block->shape_height; i++) {
        for (int j = 0; j < block->shape_width; j++) {
            if (block->shape[i][j] == true) {
                if (row + i >= grid.size() || //out of row
                 col + j >= grid[0].size() || // out of column
                  (row + i > 0 && grid[row + i][col + j] != 0) || //  
                  (row + i == 0 && grid[row + i][col + j] != 0)) {
                    return false; // Block is blocked by existing block or boundary
                }
            }
        }
    }
    return true;
}


// Function to place a block on the grid
void GameController::place_block(std::vector<std::vector<int>>& grid, const Block* block, int row, int col) {
    // row = active_rotation's top leftmost row
    // col = active_rotation's top leftmost column
    for (int i = 0; i < block->shape_height; i++) {
        for (int j = 0; j < block->shape_width; j++) {
            if (block->shape[i][j] == true) {//bool vector
                grid[row + i][col + j] = 1;
            }
        }
    }
}

long GameController::clear_filled_rows_and_calculate(std::vector<std::vector<int>>& grid) {
    long score = 0;
    int numRows = grid.size();
    int numCols = grid[0].size();

    // Check each row for being filled
    for (int i = numRows - 1; i >= 0; i--) {
        bool is_row_filled = true;

        for (int j = 0; j < numCols; j++) {
            if (grid[i][j] == 0) {
                is_row_filled = false;
                break;
            }
        }

        // If the row is filled, clear it and shift down the above rows
        if (is_row_filled) {
            std::cout << "Before Clearing: " << std::endl;
            print_grid_without_block(grid);

            grid.erase(grid.begin() + i);
            grid.insert(grid.begin(), std::vector<int>(numCols, 0));
            score += numCols;

            // Decrement i to ensure the next row is not skipped, unless i is 0
            if (i > 0) {
                i--;
            }
        }
    }

    return score;
}


void GameController::drop_cells_gravity(std::vector<std::vector<int>>& grid) {
    // iterate through each column
    for (int col = 0; col < grid[0].size(); ++col) {
        // Move full cells down
        int dest_row = grid.size() - 1;

        for (int row = grid.size() - 1; row >= 0; --row) {
            if (grid[row][col] != 0) {
                std::swap(grid[dest_row][col], grid[row][col]);
                --dest_row;
            }
        }
    }
}


// Function to calculate points of the gravity off mode
long GameController::calculate_points(const Block* block, int moving_distance) {
    
    long full_cells_in_shape = 0;

    for (const auto& row : block->shape) {
        for (int cell : row) {
            full_cells_in_shape += cell;
        }
    }
    return full_cells_in_shape * moving_distance;
    
    
}


bool GameController::is_power_up(const vector<vector<int>>& grid, const vector<vector<bool>>& power_up_shape) {

    // Like kernel striding, iterate over the grid to check if the power-up is recognizable
    int rows = grid.size();
    int cols = grid[0].size();
    int power_up_rows = power_up_shape.size();
    int power_up_cols = power_up_shape[0].size();

    // Traversing the grid
    for (int i = 0; i <= rows - power_up_rows; i++) {
        for (int j = 0; j <= cols - power_up_cols; j++) {
            bool is_matching = true;

            // Traversing the power-up to check if it is recognizable
            for (int pi = 0; pi < power_up_rows && is_matching; pi++) {
                for (int pj = 0; pj < power_up_cols && is_matching; pj++) {
                    bool power_up_cell = power_up_shape[pi][pj];
                    int grid_cell = grid[i + pi][j + pj];

                    if ((power_up_cell && grid_cell == 0) || (!power_up_cell && grid_cell != 0)) {
                        // power-up cell is true (1) and grid cell is 0, or
                        // power-up cell is false (0) and grid cell is not 0
                        // does not match, so power-up is not recognizable yet
                        is_matching = false;
                    }
                }
            }

            if (is_matching) {
                // After all iterations, if it is matching, then the power-up is recognizable
                return true;
            }
        }
    }

    return false;
}


long GameController::clear_grid(std::vector<std::vector<int>>& grid) {
    long count_of_full_cells = 0;

    // Loop through the grid to count full cells and clear them
    for (auto& row : grid) {
        for (int& cell : row) {
            if (cell != 0) {
                count_of_full_cells++;
            }
            cell = 0;  // Set the cell to zero
        }
    }

    return count_of_full_cells;
}


// returns the score that is gained after the drop
long GameController::move_down_with_gravity(std::vector<std::vector<int>>& grid, Block* active_block, int active_block_row, int active_block_col) {
    long score = 0;

    for (int i = active_block->shape_height - 1; i >= 0; i--) {
        for (int j = 0; j < active_block->shape_width; j++) {
            if (active_block->shape[i][j] != 0) { // Check if the current cell is part of the block
                int cell_row = active_block_row + i;
                int cell_col = active_block_col + j;

                // Check if the cell can move down
                while (cell_row + 1 < grid.size() && grid[cell_row + 1][cell_col] == 0) {
                    // Move the entire block down, not just individual cells
                    std::swap(grid[cell_row][cell_col], grid[cell_row + 1][cell_col]);
                    cell_row++;
                    score += 1; // Increment score for moving down by one row
                }
                grid[cell_row][cell_col] = 1; // Set the cell to full
            }
        }
    }
    return score;
}


bool GameController::is_grid_full(const std::vector<std::vector<int>>& grid, const Block* block) {
    for (size_t i = 0; i < block->shape_height; i++) {
        for (size_t j = 0; j < block->shape[i].size(); j++) {
            if (block->shape[i][j] && grid[i][j]) {
                return true; // Found an overlap, so the block can't be placed
            }
        }
    }
    return false; // No overlaps found, so the block can be placed
}


void GameController::print_game_over(BlockFall& game, Block* active_rotation, Leaderboard& game_leaderboard) {
    std::cout << "GAME FINISHED!\nNext block that couldn't fit:\n";
    print_block(active_rotation);
    std::cout << "Score: " << game.current_score << std::endl;
    if (game.leaderboard.head_leaderboard_entry != nullptr) {
        std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
    } else {
        std::cout << "High Score: " << 0 << std::endl;
    }
    print_grid(game, active_rotation, 0, 0);
    game_leaderboard.print_leaderboard();
}

void GameController::print_game_end(BlockFall& game, Block* iter, Leaderboard& game_leaderboard) {
    if (iter == nullptr) {
        std::cout << "YOU WIN!\nNo more blocks.\nFinal grid and score:\n";
    } else {
        std::cout << "GAME FINISHED!\nNo more commands.\nFinal grid and score:\n\n";
    }
    std::cout << "Score: " << game.current_score << std::endl;
    if (game.leaderboard.head_leaderboard_entry != nullptr) {
        std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
    } else {
        std::cout << "High Score: " << 0 << std::endl;
    }
    print_grid_without_block(game.grid);
    game_leaderboard.print_leaderboard();
}


void GameController::print_block(const Block* block) {
    for (const auto& row : block->shape) {
        for (bool value : row) {
            if(value == 1) {
                std::cout << occupiedCellChar << " ";
            }
            else{
                std::cout << unoccupiedCellChar << " ";
            }
        }
        std::cout << std::endl;
    }
}


bool GameController::play(BlockFall& game, const std::string& commands_file) {
    Leaderboard game_leaderboard;
    std::vector<std::string> commands_vector;

    time_t current_time;
    time(&current_time);

    commands_vector = read_commands(commands_file);

    Block* initial_block = game.initial_block;
    Block* active_rotation = game.active_rotation;

    int commandIndex = 0;

    while (active_rotation != nullptr && commandIndex < commands_vector.size()) {
        int active_block_row = 0;
        int active_block_col = 0;

        if (is_grid_full(game.grid, active_rotation)) {
            std::cout << "GAME FINISHED!\nNext block that couldn't fit:\n";
            print_block(active_rotation);
            std::cout << "Score: " << game.current_score << std::endl;
            if (game.leaderboard.head_leaderboard_entry != nullptr) {
                std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
            } else {
                std::cout << "High Score: " << 0 << std::endl;
            }
            print_grid(game, active_rotation, active_block_row, active_block_col);
            game_leaderboard.print_leaderboard();
            return false; // Block can't be placed, game over
        }

        while (commandIndex < commands_vector.size()) {
            std::string command = commands_vector[commandIndex];
            command.erase(command.find_last_not_of("\r\n") + 1);

            if (command == "PRINT_GRID") {
                std::cout << "Score: " << game.current_score << std::endl;
                if (game.leaderboard.head_leaderboard_entry != nullptr) {
                    std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
                } else {
                    std::cout << "High Score: " << 0 << std::endl;
                }
                print_grid(game, active_rotation, active_block_row, active_block_col);
                std::cout << std::endl;

            } else if (command == "ROTATE_RIGHT") {
                if (is_valid_move(game.grid, active_rotation->right_rotation, active_block_row, active_block_col)) {
                    active_rotation = active_rotation->right_rotation;
                }
            } else if (command == "ROTATE_LEFT") {
                if (is_valid_move(game.grid, active_rotation->left_rotation, active_block_row, active_block_col)) {
                    active_rotation = active_rotation->left_rotation;
                }
            } else if (command == "MOVE_RIGHT") {
                if (is_valid_move(game.grid, active_rotation, active_block_row, active_block_col + 1)) {
                    active_block_col++;
                }
            } else if (command == "MOVE_LEFT") {
                if (is_valid_move(game.grid, active_rotation, active_block_row, active_block_col - 1)) {
                    active_block_col--;
                }
            } else if (command == "DROP") {
                if (game.gravity_mode_on) {
                    game.current_score += move_down_with_gravity(game.grid, active_rotation,
                                                                active_block_row, active_block_col);

                    if (is_power_up(game.grid, game.power_up)) {
                        game.current_score += 1000;
                        game.current_score += clear_grid(game.grid);
                    }
                    game.current_score += clear_filled_rows_and_calculate(game.grid);
                    
                } else { //gravity off
                    int drop_row = 0;
                    for (int i = 0; i < game.grid.size() - active_rotation->shape_height + 1; i++) {
                        if (is_valid_move(game.grid, active_rotation, i + 1, active_block_col)) {
                            drop_row = i + 1;
                        } else {
                            break;
                        }
                    }
                    place_block(game.grid, active_rotation, drop_row, active_block_col);
                    game.current_score += calculate_points(active_rotation, drop_row);

                    if (is_power_up(game.grid, game.power_up)) {
                        game.current_score += 1000;
                        game.current_score += clear_grid(game.grid);
                    }
                    game.current_score += clear_filled_rows_and_calculate(game.grid);
                    
                }
                active_rotation = active_rotation->next_block;
                commandIndex++;//drop is the last command for the block
                break;//break out of the while loop in order to move to the next block

            }
            else if (command == "GRAVITY_SWITCH") {
                if (game.gravity_mode_on) {
                    game.gravity_mode_on = false;
                } else {
                    game.gravity_mode_on = true;
                    drop_cells_gravity(game.grid);
                    if (is_power_up(game.grid, game.power_up)) {
                        game.current_score += 1000;
                        game.current_score += clear_grid(game.grid);
                    }
                    game.current_score += clear_filled_rows_and_calculate(game.grid);
                }
            } else {
                std::cout << "Unknown command: " << command << std::endl;
            }

            commandIndex++;
        }
    }

    LeaderboardEntry *leaderboard_entry = new LeaderboardEntry(game.current_score, current_time, game.player_name);
    game_leaderboard.insert_new_entry(leaderboard_entry);

    print_game_end(game, active_rotation, game_leaderboard);

    return true;
}




