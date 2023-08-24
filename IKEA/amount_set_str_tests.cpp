#include "amount_set_str_tests.h"
#include "tests/test_utilities.h"
#include "amount_set_str_product.h"
#include "amount_set_str.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
#define INVENTORY_OUT_FILE "tests/printed_inventory.txt"
#define INVENTORY_TEST_FILE "tests/expected_inventory.txt"
#define ORDER_OUT_FILE "tests/printed_order.txt"
#define ORDER_TEST_FILE "tests/expected_order.txt"
#define BEST_SELLING_OUT_FILE "tests/printed_best_selling.txt"
#define BEST_SELLING_TEST_FILE "tests/expected_best_selling.txt"
#define NO_SELLING_OUT_FILE "tests/printed_no_selling.txt"
#define NO_SELLING_TEST_FILE "tests/expected_no_selling.txt"
*/
#define ASSERT_OR_DESTROY(expr) ASSERT_TEST_WITH_FREE((expr), asDestroy(set))

bool testAsCreate()
{
    AmountSet new_set = asCreate();
    ASSERT_TEST(new_set != NULL);
    asDestroy(new_set);
    return true;
}

bool testAsDestroy()
{
    AmountSet set1 = asCreate();
    asDestroy(set1);
    asDestroy(NULL);
    AmountSet set2 = asCreate();
    ASSERT_TEST(set2 != NULL);
    asDestroy(set2);
    return true;
}
bool testAsCopy()
{
   ASSERT_TEST(asCopy(NULL) == NULL);
  AmountSet empty = asCreate();
  AmountSet emptyCopy = asCopy(empty);
  ASSERT_TEST(empty != emptyCopy);
  asDestroy(emptyCopy);
  asDestroy(empty);
  AmountSet set = asCreate();
  asRegister(set, "hi");
  asRegister(set, "how");
  asRegister(set, "are");
  AmountSet copy = asCopy(set);
  asRegister(set, "you");
  asDestroy(copy);
  asDestroy(set);
  return true;
}
bool testAsGetSize()
{
  AmountSet empty = asCreate();
  AmountSet names = asCreate();
  asRegister(names, "areej");
  asRegister(names, "haya");
  asRegister(names, "bowirrat");
  ASSERT_TEST(asGetSize(empty) == 0);
  ASSERT_TEST(asGetSize(names) == 3);
  AmountSet namesCopy = asCopy(names);
  asDelete(names, "bowirrat");
  ASSERT_TEST(asGetSize(names) == 2);
  ASSERT_TEST(asGetSize(namesCopy) == 3);
  asDestroy(namesCopy);
  asDestroy(names);
  asDestroy(empty);
  return true;
}
bool testAsContain()
{
    AmountSet set = asCreate();
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "Haya"));
    assert(asContains(set,"Haya"));
    asDestroy(set);
    return true;
}

bool testAsRegister()
{
    AmountSet set = asCreate();
   ASSERT_TEST(AS_NULL_ARGUMENT == asRegister(NULL,"areej"));
   asRegister(set,"areej");
     ASSERT_TEST(asGetSize(set) == 1);
     asDestroy(set);
     return true;
}
bool testasChangeAmount()
{
    AmountSet set = asCreate();
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "Haya"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "ZAHRA"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asChangeAmount(set, "ZAHRA", 90));
    ASSERT_OR_DESTROY(AS_SUCCESS == asChangeAmount(set, "Haya", 50));
    if(AS_INSUFFICIENT_AMOUNT!=asChangeAmount(set, "Haya", -100))
    {
        asDestroy(set);
        return false;
    }

    asDestroy(set);
    return true;
}
bool testAsGetFirst()
{
    AmountSet set = asCreate();
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "Haya"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "ZAHRA"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "Amal"));
    if(strcmp("Amal",asGetFirst(set))!=0)
    {
        asDestroy(set);
        return false;
    }
    asDelete(set,"Amal");
    if(strcmp("Amal",asGetFirst(set))==0)
    {
        asDestroy(set);
        return false;
    }
    asDestroy(set);
    return true;
}
bool testAsGetNext()
{
    AmountSet set = asCreate();
    ASSERT_OR_DESTROY(NULL == asGetNext(NULL));
    asDestroy(set);
    return true;
}

bool testAsClear()
{
     asClear(NULL);
    AmountSet set = asCreate();
    asRegister(set, "hi");
  asRegister(set, "how");
  asRegister(set, "are");
     asClear(set);
           ASSERT_TEST(0 == asGetSize(set));
           asDestroy(set);
           return true;

}
bool testModifyProduct()
{
    AmountSet set = asCreate();
    AmountSet set_2 = asCopy(set);
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "AREEJ"));
        AmountSet set_3 = asCopy(set);
    if (AS_ITEM_DOES_NOT_EXIST != asDelete(set_2, "AREEJ"))
    {

        printf("failed in delete or copy set function");
        return false;
    }
        ASSERT_OR_DESTROY(AS_SUCCESS == asDelete(set, "AREEJ"));

    if (!asContains(set_3, "AREEJ"))
    {
        printf("apperantely your copy function dosn't make a new copy!");
        return false;
    }
    asDestroy(set_2);
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "A"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "AREEJ"));
    if (asGetSize(set_3) != 1)
    {
        return false;
    }
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "H"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "HAYA"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "  "));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "ABD"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, ""));
    ASSERT_OR_DESTROY(AS_ITEM_ALREADY_EXISTS == asRegister(set, "ABD"));
    ASSERT_OR_DESTROY(AS_ITEM_DOES_NOT_EXIST == asDelete(set, "__"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asChangeAmount(set, "ABD", 90));
    ASSERT_OR_DESTROY(AS_SUCCESS == asChangeAmount(set, "AREEJ", 7));
    ASSERT_OR_DESTROY(AS_SUCCESS == asChangeAmount(set, "AREEJ", -7));
    ASSERT_OR_DESTROY(AS_NULL_ARGUMENT == asChangeAmount(set, NULL, 30));
    ASSERT_OR_DESTROY(AS_ITEM_DOES_NOT_EXIST == 
                                        asChangeAmount(set, "bowirrat", -7));
    ASSERT_OR_DESTROY(AS_INSUFFICIENT_AMOUNT == asChangeAmount(set, "H", -7));
    asClear(set);

    if (set == NULL)
    {
        printf("clear delete the set instead of just deleting its elements");
        return false;
    }
    ASSERT_OR_DESTROY(AS_ITEM_DOES_NOT_EXIST == asChangeAmount(set, 
                                                            "bowirrat", -7));
    ASSERT_OR_DESTROY(AS_ITEM_DOES_NOT_EXIST == asDelete(set, "__"));
    asDestroy(set_3);
    asDestroy(set);
    return true;
}

bool testGetProductDetails()
{
    AmountSet set = asCreate();
    if (NULL != asGetFirst(set) || NULL != asGetNext(set))
    {
        printf("failed,check your getfirst and getnext functions");
        return false;
    }
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "ZAHRA"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asDelete(set, "ZAHRA"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asRegister(set, "ZAHRA"));
    ASSERT_OR_DESTROY(AS_SUCCESS == asChangeAmount(set, "ZAHRA", 90));
    ASSERT_OR_DESTROY(1 == asGetSize(set));
    asDestroy(set);
    //double x = 9.0;
  //  double *outAmount = &x;
    //ASSERT_OR_DESTROY(90 == asGetAmount(set, "ZAHRA", outAmount));
    return true;
}
