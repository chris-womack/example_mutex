INC_DIR=/home/redbeard/dev/example_mutex

mutex: mutex_example.c $(INC_DIR)/util/util.c
	gcc mutex_example.c $(INC_DIR)/util/util.c -I$(INC_DIR) -lpthread -o mutex_example.o

all: mutex

clean:
	rm -rf *.o	
