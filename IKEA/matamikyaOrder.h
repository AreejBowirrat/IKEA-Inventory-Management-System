#ifndef OEDER_H_
#define ORDER_H_
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "matamikyaProduct.h"
#include "set.h"

/** Type for defining the order */
typedef struct oreder_t *Order;

/**
 * orderCreate: add a new order
 *
 * @param id - new order id. Must be non-negative, and unique.
 * @return
 *     NULL - if is already exist or negative.
 *     order - if order was added successfully.
 */
Order orderCreate(unsigned int id);

/**
 * orderGetId: Returns the id of the order.
 *
 *
 * @param order - The order which id is requested.
 * @return
 *     -1 if a NULL pointer was sent.
 *     Otherwise the id of the order.
 */
unsigned int orderGetId(Order order);
/**
 * OrderGetTotalPrice: Returns the TotalPrice of the order.
 *
 *
 * @param order - The order which TotalPrice is requested.
 * @return
 *     -1 if a NULL pointer was sent.
 *     Otherwise the TotalPrice of the order.
 */
double orderGetTotalPrice(Order order);

/**
 * orderGetProductsSize: Returns the size of the order.
 *
 *
 * @param order - The order which size is requested.
 * @return
 *     -1 if a NULL pointer was sent or order dosn't exist.
 *     Otherwise the size of the product in the order.
 */
int orderGetProductsSize(Order order);

/**
 * orderGetSet: Returns the set of products in the order.
 *
 *
 * @param product - The order which set of products is requested.
 * @return
 *     NULL pointer was sent or order dosn't exist.
 *     Otherwise the set of the product in the order.
 */
Set orderGetSet(Order order );



//typedef void* ELlement;
/* functions that will be used by the amount set */
/**
 * asCopy: Creates a copy of target set.
 *
 * Iterator values for both sets are undefined after this operation.
 *
 * @param set - Target set.
 * @return
 *     NULL if a NULL was sent or a memory allocation failed.
 *     An amount set containing the same elements (and amounts) as set,
 *  otherwise.
 */
SetElement orderCopy(SetElement element);
/**
 * freeOrder: frees an existing order. deallocates all the products in
 *  the Amount set.
 *
 * @param element - Target element to be freed. If element is NULL nothing will
 *  be done.
 */
void orderFree(SetElement element);
/**
* Type of function used by the set to identify equal elements.
* This function will be used to deciding the iteration order of the set the
 order is an element in it.
* This function should return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
*		A negative integer if the second element is greater.
*/
 int orderCompare(SetElement element_1, SetElement element_2);

/**
 * orderChangeAmount: increase/decrease amount to an existing product.
 * @param order - product to modify its amount.
 * @param amount - amount to increase/decrease.
*/
void orderChangeAmount(Order order, double amount);


#endif