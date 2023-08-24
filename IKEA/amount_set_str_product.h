#ifndef PRODUCT_H_
#define PRODUCT_H_
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//
typedef struct product_t *Product;

/** Type used for returning error codes from amount set functions */
typedef enum AmountProductResult_t
{
    PRODUCT_CAN_CHANGE = 0,
    PRODUCT_CANT_CHANGE,
    PRODUCT_NULL_ARGUMENT,
    PRODUCT_FAILED_TO_UPDATE_AMOUNT,
    PRODUCT_SUCCESS
    
} ProductResult;

/**
 * createProduct: Allocates a new product.
 *
 * @return
 *     NULL - if the parameter is NULL or allocations failed.
 *     A new amount set in case of success.
 */
Product createProduct(const char *name);
/**
 * freeProduct: Deallocates an existing product. 
 *
 * @param product - Target set to be deallocated. If product is NULL nothing will be done.
 */
void freeProduct(Product product);
/**
 * copyProduct: Creates a copy of target product.
 *
 *
 * @param src - * @return
 *     NULL if a NULL was sent or a memory allocation failed.
 *     An product containing the same name as the origianl procudt
 */
 Product copyProduct(Product src);

/**
 * GetProductAmount: Returns the amount of productt.
 *
 * @paru product - The product which amount is requested.
 * @ret-1rn
 *     0 if a NULL pointer was sent.
 */
double getProductAmount(Product product);
/**
 * getProductName: Returns the name of productt.
 *
 * @param product - The product which name is requested.
 * @return
 *     Otherwise the number of elements in the set.
 */
char *getProductName(Product product);
/**
 * changeProductAmount: changes product's amount.
 *
 *
 * @param product - The product which amount will change.
 * @param amount - The number which will change to.

 * @return
 *     PRODUCT_CAN_CHANGE if the changed amount is >=0.
 *     PRODUCT_NULL_ARGUMENT if product is NULL.
 *     Otherwise PRODUCT_CANT_CHANGE.
 */
ProductResult checkProductAmount(Product product, double amount);
/* getNextProduct: Returns the next  product.
 *
 * @paru product - The product which his next is requested.
 * @retrn
 *      NULL pointer if the argument was NULL
 *  the next product otherwise.
 */
Product getProductNext(Product product);
/**
 * changeProductNext: changes product's next ptr.
 *
 *
 * @param product - The product which we want to change his next ptr pionter.
 * @param new_next - The new next ptr.
 *    void 
 */
void changeProductNext(Product product, Product new_next);

/**
 * setProductAmount: changes product's amount.
 *
 *
 * @param product - The product which we want to change his amount.
 * @param amount - the amount we want to change to.
 * @return 
 * 
 * PRODUCT_SUCCESS : if the amount changed otherwise
 * PRODUCT_FAILED_TO_UPDATE_AMOUNT
 */
ProductResult changeProductAmount(Product product, double amount);
#endif