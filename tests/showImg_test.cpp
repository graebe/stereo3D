#include <gtest/gtest.h>
#include "../src/showImg.hpp"

TEST(ShowImgTest, CanOpenValidImage)
{
    ASSERT_TRUE(ShowImg::canOpenImage("./res/img_test_1.jpg"));
}

TEST(ShowImgTest, CannotOpenInvalidImage)
{
    ASSERT_FALSE(ShowImg::canOpenImage("invalid_path.jpg"));
}
