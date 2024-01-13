#include "ImageSharpening.h"
#include <iostream>

// Default constructor
ImageSharpening::ImageSharpening() : kernel_height(3), kernel_width(3) {
    //allocate a blurring kernel(predefined)
    blurring_kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; i++) {
        blurring_kernel[i] = new double[kernel_width];
        for (int j = 0; j < kernel_width; j++) {
            blurring_kernel[i][j] = 1.0/9.0;
        }
    }
}

//Destructor
ImageSharpening::~ImageSharpening(){
    //deallocate the blurring_kernel
    if (blurring_kernel != nullptr) {
        for (int i = 0; i < kernel_height; i++) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
    }
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {

    //creating a Convolution object to use the 'convolve' function
    Convolution conv(blurring_kernel, kernel_height, kernel_width, 1, true);
    
    //kernel, padding, stride_val are all defined
    ImageMatrix blurredImg = conv.convolve(input_image);// this convolve method will blur the image

    ImageMatrix sharpImg = input_image + (input_image - blurredImg) * k;

    return sharpImg;
}
