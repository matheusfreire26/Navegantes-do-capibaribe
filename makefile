CC      = gcc
CFLAGS  = -Wall -Wextra -O2
LIBS    = -lraylib -lm -lpthread -ldl -lrt -lX11

TARGET  = navegantes
SRC     = src/main.c src/lista.c src/render.c src/jogo.c \
          src/fase_marco_zero.c src/fase_ponte_giratoria.c \
          src/fase_torre_cristal.c src/fase_rio_capibaribe.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)