#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() {
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) : kernelHeight(kh),
kernelWidth(kw), stride(stride_val), padding(pad) {
    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; i++) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    // Deallocate memory(kernel variable)
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; i++) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other)
    : kernelHeight(other.kernelHeight), kernelWidth(other.kernelWidth),
      stride(other.stride), padding(other.padding), kernel(nullptr) {
    if (other.kernel) {
        // Allocate memory for the kernel
        kernel = new double*[kernelHeight];
        for (int i = 0; i < kernelHeight; i++) {
            kernel[i] = new double[kernelWidth];
            for (int j = 0; j < kernelWidth; j++) {
                kernel[i][j] = other.kernel[i][j];
            }
        }
    }
}


Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this;  // avoiding self-assignment
    }

    // Deallocate existing resources
    for (int i = 0; i < kernelHeight; i++) {
        delete[] kernel[i];
    }
    delete[] kernel;

    // Copy values from the 'other' object
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;

    // Allocate memory for the kernel and copy values
    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; i++) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = other.kernel[i][j];
        }
    }

    return *this;
}



// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    
    if(padding == true){
        
        //creating a new image matrix to keep the 0 padded matrix
        ImageMatrix paddedImg(input_image.get_height() + 2, input_image.get_width() + 2);
    
        //replacing the middle of the image with the input_image
        for(int i = 1; i < paddedImg.get_height() - 1; i++){
            for(int j = 1; j < paddedImg.get_width() - 1; j++){
                paddedImg.set_data(i, j, input_image.get_data(i-1, j-1));
            }
        }
        
        //stride will change the shape of the output image
        int resultHeight = (paddedImg.get_height() - kernelHeight + stride) / stride;
        int resultWidth = (paddedImg.get_width() - kernelWidth + stride) / stride;
        
        
        ImageMatrix resultImg(resultHeight, resultWidth);

        for(int i = 0; i < resultImg.get_height(); i++) {
            for(int j = 0; j < resultImg.get_width(); j++){
                double sum = 0;
                for (int m = 0; m < kernelHeight; m++){
                    for(int n = 0; n < kernelWidth; n++){
                        int row = i * stride + m;
                        int col = j * stride + n;

                        // checking if the portion of the kernel passes through the image boundaries
                        if (row < paddedImg.get_height() && col < paddedImg.get_width()) {
                            double paddedValue = paddedImg.get_data(row, col);
                            double kernelValue = kernel[m][n];
                            sum += paddedValue * kernelValue;
                        }
                    }
                }
                resultImg.set_data(i, j, sum);
            }
        }
        
        return resultImg;
    }
    else {//no padding (image size will be smaller) 
        
        // stride will change the size of the output image
        int resultHeight = (input_image.get_height() - kernelHeight) / stride + 1;
        int resultWidth = (input_image.get_width() - kernelWidth) / stride + 1;

        ImageMatrix resultImg(resultHeight, resultWidth);// an image with 0's is generated

        for (int i = 0; i <= input_image.get_height() - kernelHeight; i++) {
            for (int j = 0; j <= input_image.get_width() - kernelWidth; j++) {
                double sum = 0;
                for (int m = 0; m < kernelHeight; m++) {
                    for (int n = 0; n < kernelWidth; n++) {
                        int row = i * stride + m;
                        int col = j * stride + n;
                        
                        // checking if the portion of the kernel passes through the image boundaries
                    if (row >= 0 && row < input_image.get_height() && col >= 0 && col < input_image.get_width()) {
                        double inputValue = input_image.get_data(row, col);
                        double kernelValue = kernel[m][n];
                        sum += inputValue * kernelValue;
                    }
                }
                resultImg.set_data(i, j, sum);
                
            }
            }
        }
        
        return resultImg;
    }
}   


