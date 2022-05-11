CFLAGS = -c -MD -g -Wall
THREADFLAG = -pthread
INCLUDES = -I./ -I./client -I./server -I./integration
SRC = main.c ./client/client.c shared_net.c ./server/server.c ./integration/thread_opt.c ./integration/int_count.c
OBJ = $(SRC:.c=.o)
EXEC = net

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(THREADFLAG) $(INCLUDES) -o $@ $^ -lm

%.o: %.c
	$(CC) $(THREADFLAG) $(CFLAGS) $(INCLUDES) -o $@ $< -lm

.PHONY: clean
clean:
	rm -rf $(OBJ) $(OBJ:.o=.d)

-include $(OBJ:.o=.d)