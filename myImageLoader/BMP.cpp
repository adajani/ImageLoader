#include "stdafx.h"
#include "BMP.h"

EImageType BMP::GetType() const { 
    return EImageType::BMP; 
}

int32_t BMP::GetWidth() { return bmp_info_header.width; }

int32_t BMP::GetHeight() { return bmp_info_header.height; }

void BMP::SetImageName(std::string imagePath) { m_imagePath = imagePath; }

std::string BMP::GetImageName() { return m_imagePath; }

IMetaImage *BMP::Create() { return new BMP(); }

void BMP::Destroy() {
        data.clear();
        delete this;
}
uint32_t BMP::GetPixel(uint32_t x, uint32_t y) {
    return reinterpret_cast<uint32_t>(&data[y * bmp_info_header.width + x]);
}
uint8_t *BMP::GetPixelPtr() {
    return reinterpret_cast<uint8_t*> (&data.front());
}

uint32_t BMP::GetRedMask() {
    return bmp_color_header.red_mask;
}
uint32_t BMP::GetGreenMask() {
    return bmp_color_header.green_mask;
}
uint32_t BMP::GetBlueMask() {
    return bmp_color_header.blue_mask;
}
uint32_t BMP::GetAlphaMask() {
    return bmp_color_header.alpha_mask;
}
uint16_t BMP::GetPitch() {
    return bmp_info_header.bit_count;
}

void BMP::Load() {
    std::ifstream inp{ m_imagePath, std::ios_base::binary };

    inp.read((char*)&file_header, sizeof(file_header));
    inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

    // The BMPColorHeader is used only for transparent images
    if (bmp_info_header.bit_count == 32) {
        // Check if the file has bit mask color information
        if (bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
            inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
            // Check if the pixel data is stored as BGRA and if the color space type is sRGB
            CheckHeaderColor(bmp_color_header);
        }
        else {
            throw ImageException(UNRECOGNIZED_BMP_FORMAT);
        }
    }

    // Jump to the pixel data location
    inp.seekg(file_header.offset_data, inp.beg);

    // Adjust the header fields for output.
    // Some editors will put extra info in the image file, we only save the headers and the data.
    if (bmp_info_header.bit_count == 32) {
        bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
    }
    else {
        bmp_info_header.size = sizeof(BMPInfoHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    }
    file_header.file_size = file_header.offset_data;

    if (bmp_info_header.height < 0) {
        throw ImageException(WRONG_BMP_HEIGHT);
    }

    data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

    // Here we check if we need to take into account row padding
    if (bmp_info_header.width % 4 == 0) {
        inp.read((char*)data.data(), data.size());
        file_header.file_size += static_cast<uint32_t>(data.size());
    }
    else {
        row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
        uint32_t new_stride = make_stride_aligned(4);
        std::vector<uint8_t> padding_row(new_stride - row_stride);

        for (int y = 0; y < bmp_info_header.height; ++y) {
            inp.read((char*)(data.data() + row_stride * y), row_stride);
            inp.read((char*)padding_row.data(), padding_row.size());
        }
        file_header.file_size += static_cast<uint32_t>(data.size()) + bmp_info_header.height * static_cast<uint32_t>(padding_row.size());
    }

    this->flip();
}

void BMP::flip() {
        std::vector<uint8_t> data2(data.size());
        int skip = this->GetPitch() / 8;
        int tail = data.size() - skip;
        for (int i = 0; i < data.size(); i += skip) {
            for (int j = 0; j < skip; j++) {
                data2[i + j] = data[tail + j];
            }
            tail -= skip;
        }
        data.assign(data2.begin(), data2.end());
        data2.clear();
}

// Add 1 to the row_stride until it is divisible with align_stride
uint32_t BMP::make_stride_aligned(uint32_t align_stride) {
    uint32_t new_stride = row_stride;
    while (new_stride % align_stride != 0) {
        new_stride++;
    }
    return new_stride;
}

// Check if the pixel data is stored as BGRA and if the color space type is sRGB
void BMP::CheckHeaderColor(BMPColorHeader &bmpHeaderColor) {
    BMPColorHeader colorHeader;
    if (colorHeader.red_mask != bmpHeaderColor.red_mask ||
        colorHeader.blue_mask != bmpHeaderColor.blue_mask ||
        colorHeader.green_mask != bmpHeaderColor.green_mask ||
        colorHeader.alpha_mask != bmpHeaderColor.alpha_mask) {
        throw ImageException(UNEXPECTED_COLOR_MASK);
    }

    if (colorHeader.color_space_type != bmpHeaderColor.color_space_type) {
        throw ImageException(UNEXPECTED_COLOR_SPACE_TYPE);
    }
}

