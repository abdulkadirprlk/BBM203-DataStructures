#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:

    int shape_height;//initialize in read_blocks (I added)
    int shape_width;//initialize in read_blocks (I added)  
    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game

    // constructor creates a block with zeros
    Block(int height, int width) : shape_height(height), shape_width(width) {
        // create a 2d bool vector(shape)
        shape = std::vector<std::vector<bool>>(shape_height, std::vector<bool>(shape_width, false));
    }

    // constructor creates a block based on a string vector
    Block(std::vector<std::string>& block_vector) {
        // find the height and weight of the shape of the block
        int block_height = block_vector.size();
        int block_width = block_vector[0].size();

        shape_height = block_height;
        shape_width = block_width;

        shape = std::vector<std::vector<bool>>(block_height, std::vector<bool>(block_width));

        // fill the block's shape with corresponding 1's and 0's
        for(int j = 0; j < block_height; j++) {
            for (int k = 0; k < block_width; k++) {
                if(block_vector[j][k] == '1') {
                    shape[j][k] = true;
                } else {
                    shape[j][k] = false;
                }
            }
        }
    }

    bool operator==(const Block& other) const {
        // shape sizes don't match
        if (shape_height != other.shape_height || shape_width != other.shape_width) {
            return false;
        }

        for (int i = 0; i < shape_height; i++) {
            for (int j = 0; j < shape_width; j++) {
                if (shape[i][j] != other.shape[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Block& other) const {
        if(shape_height != other.shape_height || shape_width != other.shape_width){
            return true;
        }
        for(int i = 0; i < shape_height; i++){
            for(int j = 0; j < shape_width; j++){
                if(shape[i][j] != other.shape[i][j]){
                    return true;
                }
            }
        }
        return false;
    }
};


#endif //PA2_BLOCK_H
