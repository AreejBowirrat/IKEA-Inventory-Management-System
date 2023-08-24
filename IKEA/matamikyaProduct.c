#define INTEGER_RANGE 0.001
#define MULTIPLY 2
#define POSITIVE_INTEGER 1
#define NEGATIVE_INTEGER -1

#include "matamikyaProduct.h"
#include "matamikya.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>

struct Product_t
{
    unsigned int id;
    char *name;
    double amount;
    MatamikyaAmountType amount_type;
    MtmProductData customData;
    MtmCopyData copyData;
    MtmFreeData freeData;
    MtmGetProductPrice prodPrice;
    double total_income;
    struct Product_t *next;
};


Product productCreate(unsigned int id, const char *name, const double amount,
                      const MatamikyaAmountType amountType, 
                      const MtmProductData customData,
                      MtmCopyData copyData, MtmFreeData freeData, 
                      MtmGetProductPrice prodPrice)
{
    if (name == NULL)
    {
        return NULL;
    }

    if (!checkValidation(amountType, amount))
    {
        return NULL;
    }

    Product product = NULL;
    product = (Product)malloc(sizeof(*product));
    if (product == NULL)
    {
        return NULL;
    }
    //printf("name = %s \nstrlen(name)==%zu\n",name,strlen(name));
    char *product_name = (char *)malloc(strlen(name) + 1);
    if (product_name == NULL)
    {
        free(product);
        return NULL;
    }
    product->id = id;
    strcpy(product_name, name);
    product->name = product_name;
    product->amount = amount;
    product->amount_type = amountType;
    product->copyData = copyData;
    product->customData = copyData(customData);
    product->freeData = freeData;
    product->prodPrice = prodPrice;
    product->total_income = 0;
    product->next = NULL;
    return product;
}

const char *productGetName(Product product)
{
    if (product == NULL)
    {
        return NULL;
    }
    return product->name;
}

const unsigned int productGetId(Product product)
{
    if (product == NULL)
    {
        return -1;
    }
    return product->id;
}

double productGetTotalIncome(Product product)
{
    if (product == NULL)
    {
        return -1;
    }
    return product->total_income;
}

double productGetAmount(Product product)
{
    if (product == NULL)
    {
        return -1;
    }
    return product->amount;
}

MtmProductData productGetData(Product product)
{
    return product->customData;
}

MatamikyaAmountType productGetAmountType(Product product)
{
    return product->amount_type;
}

ProductResult productChangeAmount(Product product, const double amount)
{
    if (product == NULL)
    {
        return MATAMIKYA_PRODUCT_NULL_ARGUMENT;
    }

    double amount_new = product->amount + amount;
    if (checkValidation(product->amount_type, amount_new))
    {
        product->amount = amount_new;
        return MATAMIKYA_PRODUCT_SUCCESS;
    }
    return MATAMIKYA_PRODUCT_FAILED_TO_CHANGE_AMOUNT;
}

bool checkValidation(const MatamikyaAmountType type, const double amount)
{
    if (type == MATAMIKYA_INTEGER_AMOUNT)
    {
        if ((fabs(floor(amount) - amount) <= INTEGER_RANGE) || 
                    (fabs(ceil(amount) - amount) <= INTEGER_RANGE))
        {
            return true;
        }
    }
    if (type == MATAMIKYA_HALF_INTEGER_AMOUNT)
    {

        if ((fabs(floor(MULTIPLY * amount) - MULTIPLY * amount) <= 
                                                (MULTIPLY * INTEGER_RANGE)))
        {
            return true;
        }
    }
    if (type == MATAMIKYA_ANY_AMOUNT)
    {
        return true;
    }
    return false;
}

bool productCheckBeforeChangeAmount(Product product, const double amount)
{
    if (product == NULL)
    {
        return false;
    }
    if (checkValidation(product->amount_type, amount) && 
                                                amount - product->amount >= 0)
    {
        return true;
    }
    return false;
}

MtmCopyData productGetCopyData(Product product)
{
    if (product == NULL)
    {
        return NULL;
    }

    return product->copyData;
}

MtmFreeData productGetFreeData(Product product)
{
    if (product == NULL)
    {
        return NULL;
    }

    return product->freeData;
}

MtmGetProductPrice productGetPrice(Product product)
{
    if (product == NULL)
    {
        return NULL;
    }

    return product->prodPrice;
}

MtmGetProductPrice productGetMtmProductPrice(Product product)
{
    if (product == NULL)
    {
        return NULL;
    }
    return product->prodPrice;
}

void productChangeTotalIncome(Product product, double amount)
{
    if (product == NULL)
    {
        return;
    }
    product->total_income += amount;
}
//**************FUNCTIONS FOR GENERIC SET **************************

SetElement productCopy(SetElement element)
{
    if (element == NULL)
    {
        return NULL;
    }

    Product element_2 = NULL;

    element_2 = productCreate(((Product)element)->id, ((Product)element)->name,
                              ((Product)element)->amount,
                              ((Product)element)->amount_type,
                             ((Product)element)->customData, 
                             ((Product)element)->copyData, 
                             ((Product)element)->freeData, 
                             ((Product)element)->prodPrice);
    if (element_2 == NULL)
    {
        return NULL;
    }
    element_2->total_income = ((Product)element)->total_income;
    return element_2;
}

void productFree(SetElement product)
{
    if (product == NULL)
    {
        return;
    }
    if (((Product)product)->name == NULL)
    {
        free(product);
        return;
    }
    ((Product)product)->freeData(((Product)product)->customData);
    free(((Product)product)->name);
    free(product);
}

int productCompare(SetElement product1, SetElement product2)
{
    if (product1 == NULL || product2 == NULL)
    {
        return 0;
    }
    if (((Product)product2)->id < ((Product)product1)->id)
    {
        return POSITIVE_INTEGER;
    }
    if (((Product)product2)->id > ((Product)product1)->id)
    {
        return NEGATIVE_INTEGER;
    }
    return 0;
}