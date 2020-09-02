// myImageLoaderTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "..\myImageLoader\ImageException.h"
#include "..\myImageLoader\ImageFactory.h"
#include "..\myImageLoader\IMetaImage.h"
#include "..\myImageLoader\BMP.h"

using std::string;

const char *actualValTrue = "hello gtest";
const char *actualValFalse = "hello world";
const char *expectVal = "hello gtest";


TEST(a, b) {
    IMetaImage *image = ImageFactory::Get()->Detector("d:\\image_test\\test.bmp");

    EXPECT_EQ(image->GetType(), EImageType::BMP);
}

TEST(StrCompare, CStrEqual) {
    //EXPECT_STREQ(expectVal, actualValTrue);
}

TEST(StrCompare, CStrNotEqual) {
    //EXPECT_STREQ(expectVal, actualValFalse);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}