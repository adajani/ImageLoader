#include "stdafx.h"
#include "ImageException.h"

struct ImageExceptionDict ImageException::imageExceptionDict[] = {
    { CANNOT_OPEN_IMAGE, "Cannot open image" },
    { NOT_SUPPORTED_IMAGE , "Not supported image format" },

    //BMP
    { UNEXPECTED_COLOR_MASK,       "Unexpected color mask format, pixel data should be in the BGRA format" },
    { UNEXPECTED_COLOR_SPACE_TYPE, "Unexpected color space type, color should be sRGB values" },
    { UNRECOGNIZED_BMP_FORMAT,     "Unrecognized BMP format, does not contain bit mask information" },
    { WRONG_BMP_HEIGHT,            "BMP images is not in the bottom left corner (height<0)" }
};

ImageException::ImageException (ImageExceptionId exceptionId) {
    m_imageExceptionId = exceptionId;
}

const char* ImageException::what() const throw() {
    return imageExceptionDict[m_imageExceptionId].exceptionName;
}
