#pragma once
#include <map>
#include "IMetaImage.h"
#include "BMP.h"

#define SIGNATURE_BYTES_COUNT 8
typedef IMetaImage* (*ImageCreatePtr)(void);

class ImageFactory {
public:
    ~ImageFactory();
    static ImageFactory *Get();
    IMetaImage *Detector(const char *imagePath);
private:
    std::map<std::string, ImageCreatePtr> m_imageMap;
    void Register(const std::string &signiture, ImageCreatePtr imageCreatePtr);
    ImageFactory();
};