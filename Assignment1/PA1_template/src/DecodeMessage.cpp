// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string binaryString = ""; // A string to store the least significant bits of the edge pixels
    std::string decodedMessage = ""; // A string to store the decoded message

    // Extract LSBs from the edge pixels
    for (std::pair<int, int> pixel : edgePixels) {
        int lsb = static_cast<int>(image.get_data(pixel.first, pixel.second)) & 1; // Extract the LSB
        binaryString += std::to_string(lsb);
    }

    // Pad the binary string with leading zeros for consistent 7-bit segments
    while (binaryString.length() % 7 != 0) {
        binaryString = "0" + binaryString;
    }

    // Convert binary to ASCII and ensure printable characters
    for (int i = 0; i < binaryString.length(); i += 7) {
        std::string binaryGroup = binaryString.substr(i, 7); // Extract a 7-bit segment
        int asciiCode = std::stoi(binaryGroup, nullptr, 2); // Convert binary to integer

        // Adjust ASCII values to ensure printable characters
        if (asciiCode <= 32) {
            asciiCode += 33;
        } else if (asciiCode >= 127) {
            asciiCode = 126;
        }

        decodedMessage += static_cast<char>(asciiCode); // Convert integer to ASCII character
    }

    return decodedMessage;
}


