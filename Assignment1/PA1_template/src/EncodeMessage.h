#ifndef ENCODE_MESSAGE_H
#define ENCODE_MESSAGE_H

#include <string>
#include <vector>
#include <bitset>
#include "ImageMatrix.h"


class EncodeMessage {
public:
    EncodeMessage();
    ~EncodeMessage();

    ImageMatrix encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions);


private:
    // Any private helper functions or variables if necessary
    bool isPrime(int a); // to determine if the argument is a prime or not
    int fib(int a); // to find the fibonacci number of the corresponding argument
    std::string char_transformation(const std::string& message); // to transform(manipulate) the given message
    std::string right_shifting(const std::string& transformed_message); // to shift the message to the right by len(message)/2 characters
    std::string asciiToBinary(const std::string& asciiString); // to convert the ascii string into binary
};

#endif // ENCODE_MESSAGE_H
