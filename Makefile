CC = gcc
CFLAGS = -c -std=c99 -Wall -Wextra -Werror -Wfatal-errors -pedantic -pedantic-errors -pthread 
LDFLAGS = -L. -l
#test
TEST_DIR = test
EXECUTABLE = simple_test_alloc
TEST_SRC = $(TEST_DIR)/test.c main.c  
TEST_OBJ = test.o main.o
#lib
LIB_DIR = lib
LIB_SRC = $(LIB_DIR)/alloc_static.c
LIB_NAME = alloc_static
LIB_OBJ = $(LIB_NAME).o


all: $(TEST_OBJ) $(LIB_NAME)
	$(CC) $(TEST_OBJ) $(LDFLAGS)$(LIB_NAME) -o $(EXECUTABLE)
	
 	
$(TEST_OBJ): $(TEST_SRC)
	$(CC) -c $(TEST_SRC)
	
$(LIB_NAME):  $(LIB_SRC)
	$(CC) -c $(LIB_SRC) -o $(LIB_OBJ)
	ar rcs lib$(LIB_NAME).a $(LIB_OBJ)
	ranlib lib$(LIB_NAME).a
clean_lib:
	rm -rf $(LIB_OBJ) $(EXECUTABLE)
clean:
	rm -f $(TEST_OBJ) *.o *.a $(EXECUTABLE)
