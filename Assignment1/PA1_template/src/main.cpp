#include <iostream>
#include <fstream>
#include <iomanip>
#include "ImageProcessor.h"

using std::cout;
using std::cerr;
using std::endl;

int main() {

    double score = 0.0;
    int total_number = 0;  

    // Disable out
    std::ofstream nul("NUL");
    std::streambuf* old_stdout_buf = std::cout.rdbuf(nul.rdbuf());
    std::streambuf* old_stderr_buf = std::cerr.rdbuf(nul.rdbuf());       

    // Create an ImageMatrix for testing (you can replace this with your own image data)
    const double* matrixData[5] = {
        new double[5]{1, 2, 3, 4, 5},
        new double[5]{6, 7, 8, 9, 10},
        new double[5]{11, 12, 13, 14, 15},
        new double[5]{16, 17, 18, 19, 20},
        new double[5]{21, 22, 23, 24, 25}
    };
    
    ImageMatrix testImage(matrixData, 5, 5);
    
    // Create an ImageProcessor object
    ImageProcessor imageProcessor;

    // TEST 1 
    // decodeHiddenMessage from testImage
    std::string decodedMessage1 = imageProcessor.decodeHiddenMessage(testImage);

    std::string expectedOutput1 = "\"#";

    // compare the decoded message with the expected message
    if (!decodedMessage1.compare(expectedOutput1)) {
        score += 1.0;
    } 
    total_number++;




    const double* large_im_data[28] = {
    new double[28] {189, 93, 174, 181, 25, 206, 48, 119, 167, 249, 119, 219, 239, 160, 119, 48, 97, 248, 203, 164, 47, 129, 166, 175, 237, 0, 189, 148},
    new double[28] {216, 150, 160, 157, 22, 97, 105, 181, 28, 67, 28, 123, 223, 179, 222, 1, 85, 114, 239, 55, 188, 213, 101, 208, 252, 143, 119, 127},
    new double[28] {87, 33, 72, 235, 174, 44, 107, 172, 115, 205, 247, 104, 216, 195, 176, 156, 140, 217, 49, 174, 110, 180, 65, 85, 91, 39, 137, 52},
    new double[28] {89, 15, 116, 3, 28, 97, 92, 26, 55, 158, 202, 197, 98, 17, 203, 56, 229, 35, 12, 4, 222, 207, 200, 193, 169, 254, 73, 131},
    new double[28] {135, 213, 44, 238, 117, 17, 15, 159, 217, 162, 124, 228, 134, 9, 70, 225, 8, 9, 6, 46, 39, 34, 83, 56, 121, 191, 156, 52},
    new double[28] {15, 0, 166, 9, 72, 250, 175, 69, 212, 192, 255, 92, 216, 31, 218, 55, 84, 27, 141, 254, 19, 104, 236, 86, 165, 236, 240, 168},
    new double[28] {24, 157, 56, 3, 49, 28, 129, 87, 87, 108, 198, 143, 207, 92, 118, 71, 223, 162, 2, 132, 22, 57, 89, 112, 120, 110, 239, 199},
    new double[28] {69, 206, 85, 234, 222, 139, 75, 229, 100, 203, 59, 185, 240, 140, 19, 175, 206, 210, 72, 1, 51, 116, 28, 101, 39, 127, 80, 235},
    new double[28] {125, 11, 205, 121, 108, 84, 46, 105, 74, 41, 128, 170, 83, 146, 169, 65, 175, 85, 239, 151, 21, 79, 200, 21, 132, 240, 161, 236},
    new double[28] {19, 239, 10, 161, 119, 198, 236, 243, 69, 30, 14, 180, 139, 210, 158, 70, 163, 190, 138, 248, 3, 97, 238, 198, 91, 144, 118, 179},
    new double[28] {107, 22, 89, 165, 159, 43, 22, 228, 204, 181, 220, 29, 100, 28, 40, 49, 30, 210, 214, 214, 198, 194, 204, 105, 157, 249, 40, 146},
    new double[28] {34, 239, 224, 109, 165, 85, 89, 6, 39, 220, 49, 96, 155, 200, 237, 99, 188, 252, 122, 184, 81, 27, 90, 117, 93, 8, 185, 164},
    new double[28] {115, 246, 169, 173, 34, 211, 37, 42, 244, 130, 23, 17, 157, 143, 233, 2, 242, 231, 34, 185, 180, 249, 36, 37, 227, 241, 16, 172},
    new double[28] {38, 182, 85, 205, 14, 222, 127, 15, 116, 210, 15, 201, 243, 86, 157, 31, 216, 0, 74, 236, 43, 199, 50, 3, 97, 143, 181, 132},
    new double[28] {24, 149, 141, 210, 88, 13, 210, 45, 225, 9, 172, 95, 198, 83, 99, 89, 101, 133, 113, 52, 35, 50, 128, 251, 161, 161, 117, 203},
    new double[28] {223, 219, 7, 231, 239, 171, 10, 240, 227, 203, 196, 40, 28, 242, 67, 226, 67, 208, 173, 211, 224, 201, 140, 193, 60, 27, 80, 102},
    new double[28] {206, 214, 238, 208, 162, 203, 182, 127, 85, 87, 49, 55, 78, 229, 244, 195, 112, 90, 237, 205, 181, 12, 175, 242, 44, 47, 92, 72},
    new double[28] {249, 33, 39, 95, 220, 220, 227, 246, 135, 91, 52, 135, 98, 82, 216, 226, 23, 244, 16, 134, 255, 1, 153, 134, 189, 60, 136, 231},
    new double[28] {129, 118, 84, 71, 13, 92, 241, 215, 189, 170, 159, 61, 115, 91, 21, 184, 45, 62, 193, 67, 214, 39, 139, 55, 183, 19, 255, 110},
    new double[28] {101, 232, 55, 219, 152, 67, 212, 78, 3, 152, 219, 201, 70, 142, 226, 12, 185, 64, 248, 206, 134, 126, 236, 128, 239, 108, 65, 16},
    new double[28] {205, 245, 77, 77, 43, 123, 230, 226, 239, 126, 157, 71, 94, 117, 45, 54, 176, 252, 174, 101, 142, 65, 252, 27, 45, 149, 122, 163},
    new double[28] {214, 26, 98, 92, 229, 8, 154, 119, 31, 76, 85, 60, 235, 13, 143, 130, 69, 166, 224, 199, 215, 150, 236, 243, 242, 85, 25, 163},
    new double[28] {74, 38, 43, 240, 63, 110, 44, 174, 158, 152, 65, 76, 243, 230, 164, 17, 219, 90, 235, 245, 178, 79, 242, 45, 246, 85, 148, 65},
    new double[28] {111, 66, 238, 248, 206, 76, 70, 12, 198, 17, 209, 38, 119, 176, 18, 165, 147, 238, 21, 180, 14, 67, 77, 181, 31, 19, 199, 48},
    new double[28] {28, 205, 124, 117, 126, 226, 195, 181, 226, 164, 124, 226, 108, 25, 42, 188, 16, 67, 150, 227, 194, 12, 232, 119, 67, 153, 173, 232},
    new double[28] {2, 240, 36, 187, 19, 146, 209, 72, 58, 170, 156, 231, 48, 127, 0, 132, 214, 7, 75, 199, 192, 64, 183, 207, 4, 127, 69, 211},
    new double[28] {214, 14, 90, 46, 124, 87, 246, 226, 161, 207, 165, 29, 70, 254, 109, 5, 184, 148, 20, 155, 37, 95, 17, 105, 81, 221, 31, 169},
    new double[28] {238, 139, 255, 23, 22, 27, 56, 88, 101, 220, 220, 30, 29, 31, 178, 231, 44, 212, 178, 195, 93, 3, 168, 232, 38, 67, 209, 177}
    };    

    ImageMatrix large_inputImage(large_im_data, 28, 28);

    // Test decodeHiddenMessage
    std::string large_decodedMessage = imageProcessor.decodeHiddenMessage(large_inputImage);

    std::string expectedOutput2 = "&]{oW?<pK.P!Lbi|,L9'g4YlyWT#E~?=LmO&6N;6&DU<uc\"NZiul22_";

    // print out the decoded message
    // cout << "Decoded message: " << large_decodedMessage << endl;

    // compare the decoded message with the expected message
    if (!large_decodedMessage.compare(expectedOutput2)) {
        score += 1.0;
    }
    total_number++;

    // Enable out
    std::cout.rdbuf(old_stdout_buf);
    std::cerr.rdbuf(old_stderr_buf);

    std::cout << std::fixed << std::setprecision(6) << score / (double)total_number << std::endl;

    return 0;

}

// g++ -std=c++11 TestImageProcessorDecodeHiddenMessage.cpp -o test ImageMatrix.cpp ImageSharpening.cpp EdgeDetector.cpp DecodeMessage.cpp EncodeMessage.cpp ImageProcessor.cpp ImageLoader.cpp Convolution.cpp