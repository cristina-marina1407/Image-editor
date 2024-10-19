CC = gcc
CFLAGS = -Wall -Wextra --std=c99

TARGET = sfl

SRCS = image_editor.c helper.c functions.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)