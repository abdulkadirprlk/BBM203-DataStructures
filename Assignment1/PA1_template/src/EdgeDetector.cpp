// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() : vs_height(3), vs_width(3), hs_height(3), hs_width(3) {

    // allocate the vertical sobel operator
    vertical_sobel = new double*[vs_height];
    for (int i = 0; i < vs_height; i++) {
        vertical_sobel[i] = new double[vs_width];
    }
    vertical_sobel[0][0] = -1;
    vertical_sobel[0][1] = 0;
    vertical_sobel[0][2] = 1;
    vertical_sobel[1][0] = -2;
    vertical_sobel[1][1] = 0;
    vertical_sobel[1][2] = 2;
    vertical_sobel[2][0] = -1;
    vertical_sobel[2][1] = 0;
    vertical_sobel[2][2] = 1;

    // allocate the horizontal sobel operator
    horizontal_sobel = new double*[hs_height];
    for (int i = 0; i < hs_height; i++) {
        horizontal_sobel[i] = new double[hs_width];
    }
    horizontal_sobel[0][0] = -1;
    horizontal_sobel[0][1] = -2;
    horizontal_sobel[0][2] = -1;
    horizontal_sobel[1][0] = 0;
    horizontal_sobel[1][1] = 0;
    horizontal_sobel[1][2] = 0;
    horizontal_sobel[2][0] = 1;
    horizontal_sobel[2][1] = 2;
    horizontal_sobel[2][2] = 1;
}

// Destructor
EdgeDetector::~EdgeDetector() {

    // deallocate the vertical sobel operator
    if (horizontal_sobel != nullptr) {
        for (int i = 0; i < hs_height; i++) {
            delete[] horizontal_sobel[i];
        }
        delete[] horizontal_sobel;
    }

    // deallocate the horizontal sobel operator
    if (horizontal_sobel != nullptr) {
        for (int i = 0; i < hs_height; i++) {
            delete[] horizontal_sobel[i];
        }
    }
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {

    Convolution vert_conv(vertical_sobel, vs_height, vs_width, 1, true);// two different convolution operations
    Convolution hor_conv(horizontal_sobel, hs_height, hs_width, 1, true);// vertical and horizontal

    ImageMatrix Ix = vert_conv.convolve(input_image);// Ix represents the horizontal edge response
    ImageMatrix Iy = hor_conv.convolve(input_image);// Iy represents the vertical edge response

    ImageMatrix gradients(input_image.get_height(), input_image.get_width()); // the gradients 2d array will store the gradient magnitudes of all pixels
    double total_gradient = 0;
    double gradient_count = input_image.get_height() * input_image.get_width();
    for(int i = 0; i < input_image.get_height(); i++){
        for(int j = 0; j < input_image.get_width(); j++){
            double gradient = sqrt( pow(Ix.get_data(i, j), 2) + pow(Iy.get_data(i, j), 2));
            gradients.set_data(i, j, gradient);
            total_gradient += gradient;
        }
    }

    double avg_gradient = total_gradient / gradient_count;
    // in the gradients matrix, find the edge pixels (which are greater than average gradient of all pixels)
    for(int i = 0; i < gradients.get_height(); i++){
        for(int j = 0; j < gradients.get_width(); j++){
            if(gradients.get_data(i, j) > avg_gradient){
                edgePixels.push_back(std::make_pair(i, j));
            }
        }
    }
    return edgePixels;
}

