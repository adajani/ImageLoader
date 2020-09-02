#pragma once
#include <cstdint>
#include <string>

enum EImageType { BMP, PCX, MAX_DEFINED };

class IMetaImage {
public:
    virtual int32_t GetWidth() = 0;
    virtual int32_t GetHeight() = 0;
    virtual EImageType GetType() const = 0;
    virtual void Destroy() = 0;
    virtual void SetImageName(std::string imagePath) = 0;
    virtual std::string GetImageName() = 0;
    virtual void Load() = 0;
    virtual uint32_t GetPixel(uint32_t x, uint32_t y) = 0;
    virtual uint8_t *GetPixelPtr() = 0;
    virtual uint32_t GetRedMask() = 0;
    virtual uint32_t GetGreenMask() = 0;
    virtual uint32_t GetBlueMask() = 0;
    virtual uint32_t GetAlphaMask() = 0;
    virtual uint16_t GetPitch() = 0;
};
