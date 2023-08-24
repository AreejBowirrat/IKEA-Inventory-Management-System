#include <string.h>
#include "amount_set_str_product.h"

struct product_t
{
    char *name;
    double amount;
    struct product_t *next;
};

Product createProduct(const char *name)
{
    if (name == NULL)
    {
        return NULL;
    }
    Product new_product = (Product)malloc(sizeof(*new_product));
    if (new_product == NULL)
    {
        return NULL;
    }
    char *str_name = (char *)malloc(strlen(name) + 1);
    if (str_name == NULL)
    {
        free(new_product);
        return NULL;
    }
    strcpy(str_name, name);
    new_product->name = str_name;
    new_product->amount = 0;
    new_product->next = NULL;
    return new_product;
}

Product copyProduct(Product src)
{
    if (src == NULL )
    {
        return NULL;
    }
    Product new_product = createProduct(src->name);
    if(new_product == NULL)
    {
        return NULL;
    }
    new_product->amount = src->amount;
    return new_product;
}

char *getProductName(Product product)
{
    if (product == NULL)
    {
        return NULL;
    }
    return product->name;
}

void freeProduct(Product product)
{
    if (product == NULL)
    {
        return;
    }
    free(product->name);
    free(product);
}

double getProductAmount(Product product)
{
    if (product == NULL)
    {
        return -1;
    }
    return product->amount;
}

ProductResult checkProductAmount(Product product, double amount)
{
    if (product == NULL)
    {
        return PRODUCT_NULL_ARGUMENT;
    }
    if (product->amount + amount >= 0)
    {
        return PRODUCT_CAN_CHANGE;
    }

    return PRODUCT_CANT_CHANGE;
}
Product getProductNext(Product product)
{
    if (product == NULL)
    {
        return NULL;
    }
    return product->next;
}

void changeProductNext(Product product, Product new_next)
{
    if (product == NULL)
    {
        return;
    }

    product->next = new_next;
}

ProductResult changeProductAmount(Product product, double amount)
{
    if (checkProductAmount(product, amount) == PRODUCT_CAN_CHANGE)
    {
        product->amount += amount;
        return PRODUCT_SUCCESS;
    }
    return PRODUCT_FAILED_TO_UPDATE_AMOUNT;
}