#include <nuttx/config.h>
#include <nuttx/i2c/i2c.h>
#include <nuttx/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define I2C_ADDRESS 0x38
#define I2C_PATH "/dev/i2c0"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_value>\n", argv[0]);
        fprintf(stderr, "Example: %s 0x1A\n", argv[0]);
        return 1;
    }

    char *end;
    unsigned long hex_value = strtoul(argv[1], &end, 16);
    if (*end != '\0') {
        fprintf(stderr, "Invalid hex value: %s\n", argv[1]);
        return 1;
    }

    if (hex_value > 0xFF) {
        fprintf(stderr, "Hex value out of range: %lu\n", hex_value);
        return 1;
    }

    uint8_t byte = (uint8_t)hex_value;

    int fd = open(I2C_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open I2C device");
        return 1;
    }

    struct i2c_msg_s msg;
    struct i2c_transfer_s xfer;

    msg.frequency = 100000; /* 100 kHz */
    msg.addr = I2C_ADDRESS;
    msg.flags = 0;
    msg.buffer = &byte;
    msg.length = 1;

    xfer.msgv = &msg;
    xfer.msgc = 1;

    if (ioctl(fd, I2CIOC_TRANSFER, (unsigned long)&xfer) != 0) {
        perror("Failed to transfer I2C data");
        close(fd);
        return 1;
    }

    printf("Data sent successfully to I2C address 0x%02X\n", I2C_ADDRESS);

    close(fd);
    return 0;
}
