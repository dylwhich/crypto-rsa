CFLAGS := -ggdb -Wall -I/usr/include -L/usr/lib64
OBJS := mesh.o main.o draw.o trackball.o bezier.o
EXECUTABLE := Animate
LDLIBS := -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm

.PHONY: clean

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDLIBS)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJS)
