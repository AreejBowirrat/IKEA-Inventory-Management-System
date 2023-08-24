#ifndef PRODUCT_H_
#define PRODUCT_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "matamikya.h"
#include "set.h"
/** Type for defining the set */

typedef struct Product_t *Product;

typedef enum
{
    MATAMIKYA_PRODUCT_SUCCESS = 0,
    MATAMIKYA_PRODUCT_NULL_ARGUMENT,
    MATAMIKYA_PRODUCT_VALID_AMOUNT,
    MATAMIKYA_PRODUCT_FAILED_TO_CHANGE_AMOUNT,
    MATAMIKYA_PRODUCT_FAILED_TO_SEND_AMOUNT

} ProductResult;
/**
 * productCreate: add a new product
 *
 * @param id - new product id. Must be non-negative, and unique.
 * @param name - name of the product, e.g. "apple". Must be non-empty.
 * @param amount - the initial amount of the product when added to the warehouse.
 * @param amountType - defines what are valid amounts for this product.
 * @param customData - pointer to product's additional data
 * @param copyData - function pointer to be used for copying product's additional 
 *      data.
 * @param freeData - function pointer to be used for free product data.
 * @param prodPrice - function pointer to be used for getting the price of some 
 *      product.
 * @return
 *     NULL - if matamikya/name/customData/copyData/freeData/prodPrice are NULL.
 *          - if name is empty, or doesn't start with a letter (a -z, A -Z) or a digit (0 -9).
 *          - if amount < 0, or is not consistent with amountType
 *          - if a product with the given id already exist.
 *     Product - if product was added successfully.
 */
Product productCreate(unsigned int id, const char *name, const double amount,
                      const MatamikyaAmountType amountType, const MtmProductData customData,
                      MtmCopyData copyData, MtmFreeData freeData, MtmGetProductPrice prodPrice);

/**
 * productGetName: Returns the name of the product.
 *
 *
 * @param product - The product which name is requested.
 * @return
 *     NULL if a NULL pointer was sent.
 *     Otherwise the name of the product.
 */
const char *productGetName(Product product);

/**
 * productGetId: Returns the id of the product.
 *
 *
 * @param product - The product which id is requested.
 * @return
 *     -1 if a NULL pointer was sent.
 *     Otherwise the id of the product.
 */

const unsigned int productGetId(Product product);

/**
 * productGetId: Returns the id of the product.
 *
 *
 * @param product - The product which total income is requested.
 * @return
 *     -1 if a NULL pointer was sent or product dosn't exist.
 *     Otherwise the total income of the product.
 */
double productGetTotalIncome(Product product);

/**
 * productGetAmount: Returns the id of the product.
 *
 *
 * @param product - The product which amount is requested.
 * @return
 *     -1 if a NULL pointer was sent or product dosn't exist.
 *     Otherwise the amount of the product.
 */
double productGetAmount(Product product);

/**
 * productGetData: Returns the Data of the product.
 *
 *
 * @param product - The product which productGetData is requested.
 * @return
 *     NULL if a NULL pointer was sent or product dosn't exist.
 *     Otherwise the productGetData of the product.
 */
MtmProductData productGetData(Product presult);

/**
 * productGetAmountType: Returns the AmountType of the product.
 *
 *
 * @param product - The product which AmountType is requested.
 * @return
 *     NULL if a NULL pointer was sent or product dosn't exist.
 *     Otherwise the AmountType of the product.
 */
MatamikyaAmountType productGetAmountType(Product product);



/**
 * productChangeAmount: add/increase/decrease amount to an existing product.
 * if 'amount' < 0 then this amount should be decreased from the product.
 * if 'amount' > 0 then this amount should be added to the product.
 * if 'amount' = 0 then nothing should be done.
 * @param product - product to modify its amount.
 * @param amount - amount of product to add.
 * @return
 *     MATAMIKYA_PRODUCT_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_PRODUCT_SUCCESS - if amount was increased/decreased to the produvt successfully.
 *     MATAMIKYA_PRODUCT_FAILED_TO_CHANGE_AMOUNT - if amount is not consistent with product's amount type
 *                                                 or the result is negative.
 */
ProductResult productChangeAmount(Product product, const double amount);


/**
 * productCheckBeforeChangeAmount: check if amount is consistent with product's amount type and
 * if the amount still unnegative.
 * @param product - product to modify its amount.
 * @param amount - amount of product to add/decrease.
 * @return
 *     true -if the amount to change sets with matamikya instructions.
 *     false -otherwise.
 */
bool productCheckBeforeChangeAmount(Product product, const double amount);


/**
 * productChangeTotalIncome: increase/decrease income to an existing product.
 * @param product - product to modify its amount.
 * @param amount - amount of product to increase/decrease.
 */
void productChangeTotalIncome(Product product, double amount);

/**
 * productGetMtmProductPrice: Returns the AmountType of the product.
 *
 *
 * @param product - The product which AmountType is requested.
 * @return
 *     NULL if a NULL pointer was sent or product dosn't exist.
 *     Otherwise the MtmProductPrice of the product.
 */
MtmGetProductPrice productGetMtmProductPrice(Product product);


bool checkValidation(const MatamikyaAmountType type, const double amount);


/**
 * productGetMtmProductPrice: Returns the pointer of CopyData of the product.
 *
 *
 * @param product - The product which CopyData is requested.
 * @return
 *     NULL if a NULL pointer was sent or product dosn't exist.
 *     Otherwise the CopyData of the product.
 */
MtmCopyData productGetCopyData(Product product);


/**
 * productGetFreeData: Returns the pointer of FreeData of the product.
 *
 *
 * @param product - The product which FreeData is requested.
 * @return
 *     NULL if a NULL pointer was sent or product dosn't exist.
 *     Otherwise the FreeData of the product.
 */
MtmFreeData productGetFreeData(Product product);


/**
 * productGetPrice: Returns the Price of the product.
 *
 *
 * @param product - The product which AmountType is requested.
 * @return
 *     NULL if a NULL pointer was sent or product dosn't exist.
 *     Otherwise the Price of the product.
 */
MtmGetProductPrice productGetPrice(Product product);

//_________for Generic Functions___________//
SetElement productCopy(SetElement element);

void productFree(SetElement product);

int productCompare(SetElement product1, SetElement product2);

#endif