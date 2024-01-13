#include "EncodeMessage.h"
#include <cmath>


// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {

    // transform the message
    std::string transformedMessage = char_transformation(message); // transformedMessage is an ASCII string
    std::string shiftedMessage = right_shifting(transformedMessage); // resultMessage is an ASCII string too

    // convert the ASCII string into binary 
    std::string binaryString = asciiToBinary(shiftedMessage);

    // truncate from ending if necessary
    if(binaryString.length() > positions.size()){
        binaryString.resize(positions.size());
    }

    // Create a copy of the original image to modify
    ImageMatrix modifiedImage = img;

    // embed the resulting binary message(binaryString)
    int binaryStringIndex = 0;
    for (const auto& position : positions) {
        if (binaryStringIndex >= message.length()) {
            break;  // Stop if we used all bits in the message
        }

        char bit = message[binaryStringIndex];
        double pixelValue = modifiedImage.get_data(position.first, position.second);
        int intPixelValue = static_cast<int>(pixelValue);

        // Convert the integer to its binary representation
        std::bitset<7> binaryPixel(intPixelValue);

        // Replace the LSB with the provided bit
        binaryPixel[6] = bit;

        // Convert the modified binary pixel back to an integer
        int modifiedPixelValue = static_cast<int>(binaryPixel.to_ulong());

        // Convert the modified integer back to a double
        pixelValue = static_cast<double>(modifiedPixelValue);

        modifiedImage.set_data(position.first, position.second, pixelValue);

        ++binaryStringIndex;
    }

    return modifiedImage;
}

// Function to determine whether the number is prime or not
bool EncodeMessage::isPrime(int number){
    if (number == 0 || number == 1) { // 0 and 1 aren't prime
        return false;
    }

    //  check if number has any dividors
    for (int i = 2; i <= number/2; i++) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

// Function to return the corresponding Fibonacci number
int EncodeMessage::fib(int n){
    if (n <= 1) {
        return 0;
    } else if (n == 2) {
        return 1;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

// Function to transform(manipulate) the given message
std::string EncodeMessage::char_transformation(const std::string& message) {

    std::string transformedMessage = "";
    for(int i = 0; i < message.length(); i++){
        if(isPrime(i)){
            int asciiValue = message[i]; // get the ascii value of the char in 'message' string
            asciiValue += fib(i);
            if(asciiValue <= 32){
                asciiValue += 33;
            }
            else if(asciiValue >= 127){
                asciiValue = 126;
            }
            transformedMessage += static_cast<char>(asciiValue); // Convert integer to ASCII character and store in the new string
        }
        else{
            transformedMessage += message[i]; // no conversion at all
        }
    }
    return transformedMessage;
}

// Function to shift the message to the right by len(message)/2 characters
std::string EncodeMessage::right_shifting(const std::string& transformedMessage) {
    std::string shiftedMessage = transformedMessage; // Make a copy of the input string
    int shiftAmount = transformedMessage.length() / 2; // Calculate the shift amount

    if (shiftAmount > 0) {
        // Perform the right circular shift
        shiftedMessage = transformedMessage.substr(transformedMessage.length() - shiftAmount)
            + transformedMessage.substr(0, transformedMessage.length() - shiftAmount);
    }

    return shiftedMessage; // Return the shifted message
}

std::string EncodeMessage::asciiToBinary(const std::string& asciiString) {
    std::string binaryString = "";

    for (char c : asciiString) {
        std::bitset<7> binaryChar(c); 
        binaryString += binaryChar.to_string();
    }

    return binaryString;
}
