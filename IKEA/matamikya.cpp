#define FIRST_SMALL_LETER 'a'
#define LAST_SMALL_LETER 'z'
#define FIRST_BIG_LETER 'A'
#define LAST_BIG_LETER 'Z'
#define FIRST_DIGIT 0
#define LAST_DIGIT 9
#define PER_ONE_PRODUCT 1

#include "matamikya.h"
#include "matamikya_print.h"
#include "set.h"
#include "matamikyaOrder.h"
#include "matamikyaProduct.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>

struct Matamikya_t
{
    Set products;
    Set orders;
};
//_____ static function decleration ______
static bool checkFirstLetterValidity(const char *name);
static Product productById(Set set_products, const unsigned int id);
Order orderById(Set order_set, unsigned int id);
static bool checkProductsBeforeShiping(Set products_per_order, 
                                                    Set products_in_store);
static double getMaxIncome(Set set);
static void printProductWithMaxIncome(FILE *stream, Set products,
                                                            double max_income);

Matamikya matamikyaCreate()
{
    Matamikya matam_ikya = (Matamikya)malloc(sizeof(*matam_ikya));
    if (matam_ikya == NULL)
    {
        return NULL;
    }

    Set new_set_product = setCreate(productCopy, productFree, productCompare);
    if (new_set_product == NULL)
    {
        free(matam_ikya);
        return NULL;
    }
    Set new_set_order = setCreate(orderCopy, orderFree, orderCompare);
    if (new_set_order == NULL)
    {
        setDestroy(new_set_product);
        free(matam_ikya);
        return NULL;
    }
    matam_ikya->products = new_set_product;
    matam_ikya->orders = new_set_order;
    return matam_ikya;
}

void matamikyaDestroy(Matamikya matamikya)
{
    if (matamikya == NULL)
    {
        return;
    }

    setDestroy(matamikya->products);
    setDestroy(matamikya->orders);
    free(matamikya);
}

MatamikyaResult mtmNewProduct(Matamikya matamikya,
                              const unsigned int id, const char *name,
                               const double amount,
                              const MatamikyaAmountType amountType,
                               const MtmProductData customData,
                              MtmCopyData copyData, MtmFreeData freeData,
                               MtmGetProductPrice prodPrice)
{
    if (matamikya == NULL || name == NULL || customData == NULL || 
                    copyData == NULL || freeData == NULL || prodPrice == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    if ((strcmp("", name) == 0) || !checkFirstLetterValidity(name))
    {
        return MATAMIKYA_INVALID_NAME;
    }

    if (amount < 0 || !checkValidation(amountType, amount))
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }

    Product new_product = NULL;
    new_product = productById(matamikya->products, id);
    if (new_product != NULL)
    {
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    new_product = productCreate(id, name, amount, amountType, customData, 
                                            copyData, freeData, prodPrice);
    if (new_product == NULL)
    {
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    SetResult result = setAdd(matamikya->products, new_product);

    if (result == SET_OUT_OF_MEMORY)
    {
        productFree(new_product);
        return MATAMIKYA_OUT_OF_MEMORY;
    }

    productFree(new_product);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya,
                                    const unsigned int id, const double amount)
{
    if (matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Product product = productById(matamikya->products, id);
    if (product == NULL)
    {
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    if (checkValidation(productGetAmountType(product), amount) == false)
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }

    if (amount < 0 && productGetAmount(product) < fabs(amount))
    {
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }

    ProductResult result = productChangeAmount(product, amount);
    if (result == MATAMIKYA_PRODUCT_SUCCESS)
    {
        return MATAMIKYA_SUCCESS;
    }
    return MATAMIKYA_INSUFFICIENT_AMOUNT;
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
    if (matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Product product = productById(matamikya->products, id);
    if (product == NULL)
    {
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }

    SET_FOREACH(Order, order_iterator, matamikya->orders)
    {
        SET_FOREACH(Product, product_iterator, orderGetSet(order_iterator))
        {
            if (productGetId(product_iterator) == id)
            {
                MtmGetProductPrice func = 
                                  productGetMtmProductPrice(product_iterator);
                


                
                orderDecreseAmount(order_iterator,func(productGetData(product_iterator), productGetAmount(product_iterator)));
                setRemove(orderGetSet(order_iterator), product_iterator);
                continue;
            }
        }
    }
    setRemove(matamikya->products, product);
    return MATAMIKYA_SUCCESS;
}

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    if (matamikya == NULL)
    {
        return 0;
    }
    Order new_order = orderCreate(setGetSize(matamikya->orders) + 1);

    if (new_order == NULL)
    {
        return 0;
    }

    SetResult result = setAdd(matamikya->orders, new_order);
    if (result == SET_OUT_OF_MEMORY)
    {
        orderFree(new_order);
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    orderFree(new_order);
    return setGetSize(matamikya->orders);
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, 
                                                const unsigned int orderId,
                                              const unsigned int productId, 
                                              const double amount)

{
    if (matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    Order order = orderById(matamikya->orders, orderId);
    if (order == NULL)
    {

        return MATAMIKYA_ORDER_NOT_EXIST;
    }

    Product product = productById(matamikya->products, productId);
    if (product == NULL)
    {

        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }

    if (!checkValidation(productGetAmountType(product), amount))
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }

    Product product_in_order = productById(orderGetSet(order), productId);
    if (product_in_order == NULL && amount > 0)
    {
        Product new_product = productCreate(productGetId(product), 
                                            productGetName(product),
                                            amount,productGetAmountType(product)
                                            , productGetData(product),
                                            productGetCopyData(product), 
                                            productGetFreeData(product),
                                            productGetPrice(product));
        if (new_product == NULL)
        {

            return MATAMIKYA_OUT_OF_MEMORY;
        }
        SetResult result = setAdd(orderGetSet(order), new_product);
        if (result == SET_OUT_OF_MEMORY)
        {
            return MATAMIKYA_OUT_OF_MEMORY;
        }

        MtmGetProductPrice function = productGetPrice(new_product);
   
                
        orderIncreaseAmount(order, function(productGetData(new_product),amount));
        productFree(new_product);
   
        return MATAMIKYA_SUCCESS;
    }


    if (product_in_order != NULL && amount < 0 && 
                         fabs(amount) > productGetAmount(product_in_order))
    {
        double old_product_amount = productGetAmount(product_in_order);
        productChangeAmount(product_in_order,amount);
        MtmGetProductPrice function = productGetPrice(product_in_order);
 
                orderDecreseAmount(order, function(productGetData(product_in_order), old_product_amount));

        setRemove(orderGetSet(order), product_in_order);
        return MATAMIKYA_SUCCESS;
    }

if(product_in_order != NULL)
{
    double old_product_amount = productGetAmount(product_in_order);
    productChangeAmount(product_in_order, amount);
    MtmGetProductPrice function2 = productGetPrice(product_in_order);
 
    orderIncreaseAmount(order, function2(productGetData(product_in_order),
                    productGetAmount(product_in_order) - old_product_amount));

    if(productGetAmount(product_in_order) == 0)
    {
        setRemove(orderGetSet(order), product_in_order);
    }
         return MATAMIKYA_SUCCESS;
}
return MATAMIKYA_SUCCESS;

}

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId)
{
    if (matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    Order order = orderById(matamikya->orders, orderId);
    if (order == NULL)
    {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    if (orderGetProductsSize(order) == 0)
    {
   
        return MATAMIKYA_SUCCESS;
    }

    if (checkProductsBeforeShiping(orderGetSet(order), matamikya->products))
    {


        SET_FOREACH(Product, product_in_order, orderGetSet(order))
        {
            SET_FOREACH(Product, product_in_mtmikya, (matamikya->products))
            {
                if(productGetId(product_in_order)==
                                            productGetId(product_in_mtmikya))
                {
                    if(productGetAmount(product_in_order)>0)
                    {
                        
                    productDecreseAmount(product_in_mtmikya,productGetAmount(product_in_order));
                    MtmGetProductPrice func= productGetPrice(product_in_order);
                    productChangeTotalIncome(product_in_mtmikya, func(productGetData(product_in_order), productGetAmount(product_in_order)));  
                    }
                    
                }
            }
        }
        setRemove(matamikya->orders, order);
        return MATAMIKYA_SUCCESS;
    }

  //  return MATAMIKYA_SUCCESS;
   return MATAMIKYA_INSUFFICIENT_AMOUNT;
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId)
{
    if (matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order order = orderById(matamikya->orders, orderId);
    if (order == NULL)
    {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    SetResult result = setRemove(matamikya->orders, order);
    if (result == SET_SUCCESS)
    {
        return MATAMIKYA_SUCCESS;
    }

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output)
{
    if (output == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    if (matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    fprintf(output, "Inventory Status:\n");
    if (setGetSize(matamikya->products) != 0)
    {
        MtmGetProductPrice function;

        SET_FOREACH(Product, iterator, matamikya->products)
        {
            function = productGetMtmProductPrice(iterator);
            mtmPrintProductDetails(productGetName(iterator),
                 productGetId(iterator), productGetAmount(iterator), 
                 function(productGetData(iterator), PER_ONE_PRODUCT), output);
        }
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, 
                                FILE *output)
{
    if (output == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    if (matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    Order order = orderById(matamikya->orders, orderId);
    if (order == NULL)
    {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    mtmPrintOrderHeading(orderId, output);
    MtmGetProductPrice function;

    SET_FOREACH(Product, product_iterator, orderGetSet(order))
    {
        function = productGetMtmProductPrice(product_iterator);
        mtmPrintProductDetails(productGetName(product_iterator),
            productGetId(product_iterator),productGetAmount(product_iterator), 
                           function(productGetData(product_iterator), 
                            productGetAmount(product_iterator)), output);
    }
    mtmPrintOrderSummary(orderGetTotalPrice(order), output);
    return MATAMIKYA_SUCCESS;
}
MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{
    if (output == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    if (matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    fprintf(output, "Best Selling Product:\n");

    double max_income = getMaxIncome(matamikya->products);
    if (max_income == 0)
    {
        fprintf(output, "none\n");
    }
    else
    {
        printProductWithMaxIncome(output, matamikya->products, max_income);
    }
    return MATAMIKYA_SUCCESS;
}

//****************************STATIC FUNCTION ******************************

static bool checkFirstLetterValidity(const char *name)
{
    if ((name[0] >= FIRST_SMALL_LETER && name[0] <= LAST_SMALL_LETER) ||
        (name[0] >= FIRST_BIG_LETER && name[0] <= LAST_BIG_LETER) ||
        (name[0] >= FIRST_DIGIT && name[0] <= LAST_DIGIT))
    {
        return true;
    }

    return false;
}

static Product productById(Set set_products, const unsigned int id)
{
    SET_FOREACH(Product, iterator, set_products)
    {
        if (productGetId(iterator) == id)
        {
            return iterator;
        }
    }
    return NULL;
}

Order orderById(Set order_set, const unsigned int id)
{
    SET_FOREACH(Order, iterator, order_set)
    {
        if (orderGetId(iterator) == id)
        {
            return iterator;
        }
    }
    return NULL;
}
//checking the validation of the amounts in each order of the set
static bool checkProductsBeforeShiping(Set products_per_order, 
                                            Set products_in_store)
{
    SET_FOREACH(Product, iterator, products_per_order)
    {
        SET_FOREACH(Product, iterator2, products_in_store)
        {
            if ((productGetId(iterator2) == productGetId(iterator)) &&
                (!productCheckBeforeChangeAmount(iterator, 
                                            productGetAmount(iterator2))))
            {
                return false;
            }
        }
    }
    return true;
}

static double getMaxIncome(Set set)
{

    if (set == NULL)
    {
        return 0;
    }
    Product iterator = setGetFirst(set);
    double max_income = productGetTotalIncome(iterator);

    SET_FOREACH(Product, iterator, set)
    {
        if (productGetTotalIncome(iterator) > max_income)
        {
            max_income = productGetTotalIncome(iterator);
        }
    }

    return max_income;
}

static void printProductWithMaxIncome(FILE *stream, Set products,
                                           double max_income)
{
    SET_FOREACH(Product, iterator, products)
    {
        if (productGetTotalIncome(iterator) == max_income)
        {
            mtmPrintIncomeLine(productGetName(iterator), productGetId(iterator),
                                  productGetTotalIncome(iterator), stream);
            return;
        }
    }
}

Set mtamikyaGetProductsSet(Matamikya matamikya)
{
    if (matamikya == NULL)
    {
        return NULL;
    }

    return matamikya->products;
}