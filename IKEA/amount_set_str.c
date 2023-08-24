#include "amount_set_str.h"
#include "amount_set_str_product.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

struct AmountSet_t
{
    Product head;
    Product iterator;
    int size;
};
//functions declarations
static AmountSetResult copySetToSet(AmountSet dest_set, AmountSet origin_set);
static AmountSetResult addProductOrDestroy(AmountSet set, char *str);
static Product getProductByName(AmountSet set, const char *name);
//functions implementations
AmountSet asCreate()
{
    AmountSet set = (AmountSet)malloc(sizeof(*set));
    if (set == NULL)
    {
        return NULL;
    }

    set->head = NULL;
    set->iterator = NULL;
    set->size = 0;
    return set;
}
void asDestroy(AmountSet set)
{
    if (set == NULL)
    {
        return;
    }
    while (asGetSize(set) != 0)
    {
        asDelete(set, asGetFirst(set));
    }
    free(set);
}

AmountSet asCopy(AmountSet set)
{
    if (set == NULL)
    {
        return NULL;
    }
    AmountSet new_set = asCreate();
    if (!new_set)
    {
        return NULL;
    }
    if (copySetToSet(new_set, set) == AS_OUT_OF_MEMORY)
    {
        return NULL;
    }
 
    return new_set;
}

int asGetSize(AmountSet set)
{
    if (set == NULL)
    {
        return -1;
    }
    return set->size;
}

bool asContains(AmountSet set, const char *element)
{
    if (set == NULL)
    {
        return false;
    }
    Product pointer = set->head;
    
    for (int i = 0; i < set->size; i++)
    {
        if (pointer != NULL && strcmp(element, getProductName(pointer)) == 0)
        {
            return true;
        }
        pointer = getProductNext(pointer);
    }

    return false;
}

AmountSetResult asGetAmount(AmountSet set, const char *element,
                                                            double *outAmount)
{
    if (set == NULL || element == NULL || outAmount == NULL)
    {
        return AS_NULL_ARGUMENT;
    }

    if (asContains(set, element))
    {
        Product get_product = getProductByName(set, element);
        assert(strcmp(getProductName(get_product), element) == 0);
        *outAmount = getProductAmount(get_product);
        return AS_SUCCESS;
    }
    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asRegister(AmountSet set, const char *element)
{
    if (set == NULL || element == NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    if (asContains(set, element))
    {
        return AS_ITEM_ALREADY_EXISTS;
    }
    Product new_product = createProduct(element);
    if (new_product == NULL)
    {
        return AS_OUT_OF_MEMORY;
    }
    if (set->head == NULL)
    {
        set->head = new_product;
        set->iterator = new_product;
        set->size++;
        return AS_SUCCESS;
    }
    Product iterator = set->head;
    //if the new product should be added in the begging of the set
    if (strcmp(getProductName(iterator), element) > 0)
    {
        changeProductNext(new_product, set->head);
        set->head = new_product;
        set->size++;
        return AS_SUCCESS;
    }
    while (iterator != NULL)
    {
        if (getProductNext(iterator) == NULL)
        {
            changeProductNext(iterator, new_product);
            set->size++;
            return AS_SUCCESS;
        }

        if (strcmp(getProductName(iterator), element) < 0 &&
                 strcmp(getProductName(getProductNext(iterator)), element) > 0)
        {
            changeProductNext(new_product, getProductNext(iterator));
            changeProductNext(iterator, new_product);
            set->size++;
            return AS_SUCCESS;
        }
        iterator = getProductNext(iterator);
    }
    return AS_SUCCESS;
}

AmountSetResult asChangeAmount(AmountSet set,const char *element,double amount)
{
    if (set == NULL || element == NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    if (!asContains(set, element))
    {
        return AS_ITEM_DOES_NOT_EXIST;
    }

    Product product = getProductByName(set, element);
    if (checkProductAmount(product, amount) == PRODUCT_CAN_CHANGE)
    {
        if (changeProductAmount(product, amount) == PRODUCT_SUCCESS)
        {
            return AS_SUCCESS;
        }
    }
    return AS_INSUFFICIENT_AMOUNT;
}

AmountSetResult asDelete(AmountSet set, const char *element)
{
    if (set == NULL || element == NULL)
    {
        return AS_NULL_ARGUMENT;
    }

    Product iterator = set->head;
    if (!asContains(set, element))
    {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    Product previous = iterator;
    while (iterator != NULL)
    {
        if (strcmp(getProductName(iterator), element) == 0)
        {
            if (iterator == set->head)
            {
                Product tmp1 = getProductNext(set->head);
                freeProduct(iterator);
                set->head = tmp1;
                set->size--;
                return AS_SUCCESS;
            }
            Product tmp = getProductNext(iterator);
            freeProduct(iterator);
            changeProductNext(previous, tmp);
            set->size--;
            return AS_SUCCESS;
        }
        previous = iterator;
        iterator = getProductNext(iterator);
    }
    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asClear(AmountSet set)
{
    Product tmp = NULL;
    if (set == NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    Product iterator = set->head;
    while (iterator != NULL)
    {
        tmp = getProductNext(iterator);
        freeProduct(iterator);
        iterator = tmp;
    }
    set->size = 0;
    return AS_SUCCESS;
}

char *asGetFirst(AmountSet set)
{
    if (set == NULL || asGetSize(set) == 0)
    {
        return NULL;
    }
    set->iterator = set->head;
    return getProductName(set->head);
}

char *asGetNext(AmountSet set)
{
    if (set == NULL || set->iterator == NULL || 
                                    getProductNext(set->iterator) == NULL)
    {
        return NULL;
    }
    set->iterator = getProductNext(set->iterator);
    return getProductName(set->iterator);
}

//___________static function_________

static AmountSetResult copySetToSet(AmountSet dest_set, AmountSet origin_set)
{

    if (origin_set == NULL || dest_set == NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    origin_set->iterator = origin_set->head;
    char *iterator = getProductName(origin_set->iterator);
    for (int i = 0; i < asGetSize(origin_set); ++i)
    {
        if (addProductOrDestroy(dest_set, iterator) == AS_OUT_OF_MEMORY)
        {
            return AS_OUT_OF_MEMORY;
        }
        iterator = asGetNext(origin_set);
    }

    return AS_SUCCESS;
}

static AmountSetResult addProductOrDestroy(AmountSet set, char *str)
{
    AmountSetResult result = asRegister(set, str);

    if (result != AS_SUCCESS)
    {
        asDestroy(set);
    }
    return result;
}

static Product getProductByName(AmountSet set, const char *name)
{
    Product iterator = set->head;
    while (iterator != NULL)
    {
        if (strcmp(name, getProductName(iterator)) == 0)
        {
            return iterator;
        }
        iterator = getProductNext(iterator);
    }
    return NULL;
}