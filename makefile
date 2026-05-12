CC      = gcc
CFLAGS  = -Wall -Wextra -O2
LIBS    = -lraylib -lm -lpthread -ldl -lrt -lX11

TARGET  = navegantes
SRC     = src/main.c src/lista.c src/render.c src/jogo.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)