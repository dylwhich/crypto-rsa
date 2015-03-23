CFLAGS := -ggdb -Wall
OBJS := off.o mesh.o main.o

Model: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
