CC = gcc
CFLAGS = -Wall -g
LDFLAGS =

TARGET = nuttx_i2c_app

SRCS = nuttx_i2c_app.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
