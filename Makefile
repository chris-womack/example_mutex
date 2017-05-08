INC_DIR=/home/redbeard/dev/example_mutex

mutex: mutex_example.c $(INC_DIR)/util/util.c
	gcc -Wall -Wextra mutex_example.c $(INC_DIR)/util/util.c -I$(INC_DIR) -lpthread -o mutex_example.o
	gcc -Wall -Wextra unprotected_example.c $(INC_DIR)/util/util.c -I$(INC_DIR) -lpthread -o unprotected_example.o

all: mutex

clean:
	rm -rf *.o	
