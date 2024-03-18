#include <gtest/gtest.h>



int main(
        int argc,
        char *argv[])
{
    std::cout << "Dad";
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
