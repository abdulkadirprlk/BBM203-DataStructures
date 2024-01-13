    #ifndef EDGE_DETECTOR_H
    #define EDGE_DETECTOR_H

    #include "ImageMatrix.h"
    #include "Convolution.h"
    #include <vector>

    class EdgeDetector {
        public:
            EdgeDetector(); 
            ~EdgeDetector(); 
            
            std::vector<std::pair<int, int>> detectEdges(const ImageMatrix& input_image);

        private:
            // add your private member variables and functions 
            std::vector<std::pair<int, int>> edgePixels;
            double** vertical_sobel;
            int vs_height;
            int vs_width;
            double** horizontal_sobel;
            int hs_height;
            int hs_width;
    };

    #endif // EDGE_DETECTOR_H


