CC = gcc
OBJS = matamikya.o matamikyaProduct.o matamikyaOrder.o matamikya_print.o matamikya_main.o matamikya_tests.o amount_set_str.o amount_set_str_product.o amount_set_str_tests.o
EXEC = matamikya
CFLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L. -lmtm -lm -las

matamikya.o : matamikya.c matamikya.h matamikyaOrder.h matamikyaProduct.h tests/matamikya_print.h
	$(CC) $(CFLAGS) -c matamikya.c -o matamikya.o

matamikyaProduct.o: matamikyaProduct.c matamikyaProduct.h matamikya.h
	$(CC) -c $(CFLAGS) $*.c -o matamikyaProduct.o

matamikyaOrder.o: matamikyaOrder.c matamikyaOrder.h
	$(CC) -c $(CFLAGS) $*.c -o matamikyaOrder.o

matamikya_print.o: tests/matamikya_print.c tests/matamikya_print.h
	$(CC) -c $(CFLAGS) $*.c -o matamikya_print.o

matamikya_main.o: tests/matamikya_main.c tests/matamikya_tests.h tests/test_utilities.h
	$(CC) -c $(CFLAGS) $*.c -o matamikya_main.o

matamikya_tests.o: tests/matamikya_tests.c tests/matamikya_tests.h matamikya.h tests/test_utilities.h
	$(CC) -c $(CFLAGS) $*.c -o matamikya_tests.o

amount_set_str.o: amount_set_str.c amount_set_str.h amount_set_str_product.h
	$(CC) -c $(CFLAGS) $*.c -o amount_set_str.o

amount_set_str_product.o: amount_set_str_product.c amount_set_str_product.h
	$(CC) -c $(CFLAGS) $*.c -o amount_set_str_product.o

amount_set_str_main.o: amount_set_str_main.c amount_set_str_tests.h tests/test_utilities.h
	$(CC) -c $(CFLAGS) $*.c -o amount_set_str_main.o

amount_set_str_tests.o: amount_set_str_tests.c amount_set_str_tests.h amount_set_str_product.h  amount_set_str.h tests/test_utilities.h
	$(CC) -c $(CFLAGS) $*.c -o amount_set_str_tests.o

clean:
	rm -f $(OBJS) $(EXEC)