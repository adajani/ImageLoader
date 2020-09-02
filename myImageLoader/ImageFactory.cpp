#include "stdafx.h"
#include <memory>
#include <fstream>
#include <sstream>
#include "ImageException.h"
#include "ImageFactory.h"
ImageFactory::~ImageFactory() { m_imageMap.clear(); }

ImageFactory *ImageFactory::Get() {
    static ImageFactory instance;
    return &instance;
}

IMetaImage *ImageFactory::Detector(const char *imagePath) {
        std::unique_ptr<char[]> signature(new char[SIGNATURE_BYTES_COUNT]());

        std::ifstream image(imagePath, std::ios::in | std::ios::binary);
        if (!image) {
            throw ImageException(CANNOT_OPEN_IMAGE);
        }

        auto singPtr = const_cast<char*>(signature.get());

        image.read(singPtr, SIGNATURE_BYTES_COUNT);
        image.close();

        for (register int index = 1; index < SIGNATURE_BYTES_COUNT; index++) {
            std::string detect(singPtr);
            auto it = m_imageMap.find(detect.substr(0, index));
            if (it != m_imageMap.end()) {
                auto imageObject = it->second();
                imageObject->SetImageName(imagePath);
                return imageObject;
            }
        }

        throw ImageException(NOT_SUPPORTED_IMAGE);
}


void ImageFactory::Register(const std::string &signiture, ImageCreatePtr imageCreatePtr) {
    m_imageMap[signiture] = imageCreatePtr;
}

ImageFactory::ImageFactory() {
        Register("\x42\x4d", &BMP::Create);

        //TODO: add more format
        //Register("\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", &PNG::Create);
}
