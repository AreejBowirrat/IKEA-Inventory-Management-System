#include "matamikyaOrder.h"
#include "set.h"
#define POSITIVE_INTEGER 1
#define NEGATIVE_INTEGER -1
struct oreder_t
{
    unsigned int id;
    Set products_per_order;
    double total_price;
    struct oreder_t *next;
};

unsigned int orderGetId(Order order)
{
    if (order == NULL)
    {
        return -1;
    }
    return order->id;
}

double orderGetTotalPrice(Order order)
{
    if (order == NULL)
    {
        return -1;
    }
    return order->total_price;
}

SetElement orderCopy(SetElement element)
{
    if (element == NULL)
    {
        return NULL;
    }
    Order new_order = (Order)malloc(sizeof(*new_order));
    if (new_order == NULL)
    {
        return NULL;
    }
    new_order->id = ((Order)element)->id;
    Set new_set =setCopy(((Order)element)->products_per_order);
    if(new_set==NULL){
        return NULL;
    }
    new_order->products_per_order =new_set;
    new_order->total_price=((Order)element)->total_price;
    return new_order;
}
void orderFree(SetElement element)
{
    if (element == NULL)
    {
        return;
    }
    setDestroy(((Order)element)->products_per_order);
    free((Order)element);
}

int orderCompare(SetElement element_1, SetElement element_2)
{
    if (((Order)element_2)->id < ((Order)element_1)->id)
    {
        return POSITIVE_INTEGER;
    }
    if (((Order)element_2)->id > ((Order)element_1)->id)
    {
        return NEGATIVE_INTEGER;
    }
    //doen't reach because order id is unique
    return 0;
}


Order orderCreate(unsigned int id)
{
  Order new_order = (Order)malloc(sizeof(*new_order));
  if(new_order == NULL)
  {
	  return NULL;
  }

  new_order -> id = id;
  Set new_set = setCreate(productCopy,productFree,productCompare);
  if(new_set == NULL)
  {
	  free(new_order);
	  return NULL;
  }
  new_order -> products_per_order = new_set;
  new_order -> total_price = 0;
  new_order -> next = NULL;
  return new_order;
}

int orderGetProductsSize(Order order)
{
    if (order == NULL)
    {
        return -1;
    }

    return setGetSize(order->products_per_order);
}

Set orderGetSet(Order order)
{
    if (order == NULL)
    {
        return NULL;
    }
    return order->products_per_order;
}

void orderDecreseAmount(Order order, double amount)

{
  if (order == NULL)
    {
        return;
    }
    
    order->total_price -= amount;
}

void orderIncreaseAmount(Order order, double amount)
{
  if (order == NULL)
    {
        return;
    }
    
    order->total_price += amount;
}

