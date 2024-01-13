#include "ImageMatrix.h"
#include <iostream>


// Default constructor
ImageMatrix::ImageMatrix(){
    
}

// Parameterized constructor for creating a blank image of given size
ImageMatrix::ImageMatrix(int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth) {
    data = new double*[height];//creating a blank image with values of 0
    for (int i = 0; i < height; i++) {
        data[i] = new double[width];
        for (int j = 0; j < width; j++) {
            data[i][j] = 0.0;
        }   
    }
}

// Parameterized constructor for loading image from file. PROVIDED FOR YOUR CONVENIENCE
ImageMatrix::ImageMatrix(const std::string &filepath) {
    // Create an ImageLoader object and load the image
    ImageLoader imageLoader(filepath);

    // Get the dimensions of the loaded image
    height = imageLoader.getHeight();
    width = imageLoader.getWidth();

    // Allocate memory for the matrix
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
    }

    // Copy data from imageLoader to data
    double** imageData = imageLoader.getImageData();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imageData[i][j];
        }
    }
}

// Destructor
ImageMatrix::~ImageMatrix() {
    // Deallocate memory
    if (data != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
}

// Parameterized constructor - direct initialization with 2D matrix
ImageMatrix::ImageMatrix(const double** inputMatrix, int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth) {
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = inputMatrix[i][j];
        }
    }
}

// Copy constructor
ImageMatrix::ImageMatrix(const ImageMatrix &other) : height(other.height), width(other.width) {
    // Allocate memory for the matrix and copy data
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Copy assignment operator
ImageMatrix& ImageMatrix::operator=(const ImageMatrix &other) {
     if (this == &other) {
            return *this; // self-assignment check
        }

    // Deallocate old memory("data" variable)
    if (data != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }

    // Copy from "other" object
    height = other.height;
    width = other.width;
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = other.data[i][j];
        }
    }

    return *this;
}

// Overloaded operators

// Overloaded operator + to add two matrices
ImageMatrix ImageMatrix::operator+(const ImageMatrix &other) const {
    if(this->height != other.height || this->width != other.width)
    {
        std::cerr << "Error: Incompatible matrices!" << std::endl;
        return ImageMatrix(0, 0);
    }

    ImageMatrix resultImg(this->height, this->width);//new ImageMatrix object to keep the result

    for(int i = 0; i < this->height; ++i)//adding other.data and this.data
    {
        for(int j = 0; j < this->width; ++j)
        {
            resultImg.data[i][j] = this->data[i][j] + other.data[i][j];
        }
    }
    return resultImg;
}

// Overloaded operator - to subtract two matrices
ImageMatrix ImageMatrix::operator-(const ImageMatrix &other) const {
    if(this->height != other.height || this->width != other.width)
    {
        std::cerr << "Error: Incompatible matrices!" << std::endl;
        return ImageMatrix(0, 0);
    }

    ImageMatrix resultImg(this->height, this->width);//new ImageMatrix object to keep the result

    for(int i = 0; i < this->height; ++i)//subtracting other.data from this.data
    {
        for(int j = 0; j < this->width; ++j)
        {
            resultImg.data[i][j] = this->data[i][j] - other.data[i][j];
        }
    }
    return resultImg;
}

// Overloaded operator * to multiply a matrix with a scalar
ImageMatrix ImageMatrix::operator*(const double &scalar) const {

    ImageMatrix resultImg(this->height, this->width);//new ImageMatrix object to keep the result

    for(int i = 0; i < this->height; ++i)//multiplying the "image matrix" with a scalar
    {
        for(int j = 0; j < this->width; ++j)
        {
            resultImg.data[i][j] = this->data[i][j] * scalar;
        }
    }
    return resultImg;
}

// Getter function to access the data in the matrix
double** ImageMatrix::get_data() const {
    return data;
}

// Getter function to access the data at the index (i, j)
double ImageMatrix::get_data(int i, int j) const {
    return data[i][j];
} 

// Getter function to access the height of the matrix
int ImageMatrix::get_height() const {
    return height;
} 

// Getter function to access the width of the matrix
int ImageMatrix::get_width() const {
    return width;
} 

//Setter function of a pixel in the image matrix
void ImageMatrix::set_data(int i, int j, double value){
    data[i][j] = value;
}
