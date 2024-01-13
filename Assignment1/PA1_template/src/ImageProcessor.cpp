#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {

    // sharpen the input image (img)
    ImageSharpening imageSharpener;
    ImageMatrix sharpenedImg = imageSharpener.sharpen(img, 2);

    // detect the edges in the sharpened image (sharpenedImg)
    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edgePixels = edgeDetector.detectEdges(sharpenedImg);

    // decode the message in the sharpened image
    DecodeMessage decoder;
    std::string hiddenMessage = decoder.decodeFromImage(sharpenedImg, edgePixels);
    
    return hiddenMessage;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    
    // sharpen the image
    ImageSharpening imageSharpener;
    ImageMatrix sharpenedImg = imageSharpener.sharpen(img, 2);

    // detect the edges in the sharpened image (sharpenedImg)
    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edgePixels = edgeDetector.detectEdges(sharpenedImg);

    // encode the message in the image
    EncodeMessage encoder;
    ImageMatrix resultImg = encoder.encodeMessageToImage(img, message, edgePixels);

    return resultImg;

}
