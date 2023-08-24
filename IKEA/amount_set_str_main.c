#include "amount_set_str_tests.h"
#include "tests/test_utilities.h"

int main()
{
    RUN_TEST(testAsCreate);
    RUN_TEST(testAsDestroy);
    RUN_TEST(testAsCopy);
    RUN_TEST(testAsGetSize);
    RUN_TEST(testAsContain);
    RUN_TEST(testAsRegister);
    RUN_TEST(testasChangeAmount);
    RUN_TEST(testAsGetFirst);
    RUN_TEST(testAsGetNext);
    RUN_TEST(testAsClear);
    RUN_TEST(testModifyProduct);
    RUN_TEST(testGetProductDetails);
    return 0;
}
