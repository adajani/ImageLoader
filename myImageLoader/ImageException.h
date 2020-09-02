#pragma once
#include <iostream>

enum ImageExceptionId {
    //Factory
    CANNOT_OPEN_IMAGE,
    NOT_SUPPORTED_IMAGE,

    //BMP
    UNEXPECTED_COLOR_MASK,
    UNEXPECTED_COLOR_SPACE_TYPE,
    UNRECOGNIZED_BMP_FORMAT,
    WRONG_BMP_HEIGHT
};

struct ImageExceptionDict {
    unsigned int exceptionId;
    char *exceptionName;
};

class ImageException : public std::exception {
public:
    ImageException(ImageExceptionId exceptionId);
    const char* what() const throw();
private:
    ImageExceptionId m_imageExceptionId;
    static struct ImageExceptionDict imageExceptionDict[];
};
