#include <nuttx/config.h>
#include <nuttx/i2c/i2c_master.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define I2C_ADDRESS 0x38
#define I2C_PATH "/dev/i2c0"

/* Возвращает:
 *  0  — успешно
 * -1  — неверный формат
 * -2  — значение вне диапазона 0x00..0xFF
 */
static int parse_hex_byte(const char *s, uint8_t *out)
{
    if (s == NULL || *s == '\0') {
        return -1;
    }

    /* Необязательный префикс 0x / 0X */
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        s += 2;
    }

    if (*s == '\0') {
        return -1;
    }

    unsigned int value = 0;
    int digits = 0;

    while (*s != '\0') {
        int c = (unsigned char)*s;
        int d;

        if (c >= '0' && c <= '9') {
            d = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            d = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            d = c - 'A' + 10;
        } else {
            return -1;
        }

        value = (value << 4) | (unsigned int)d;
        if (value > 0xFF) {
            return -2;
        }

        digits++;
        s++;
    }

    if (digits == 0) {
        return -1;
    }

    *out = (uint8_t)value;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_value 0x00..0xFF>\n", argv[0]);
        fprintf(stderr, "Example: %s 0x1A\n", argv[0]);
        return 1;
    }

    uint8_t byte;
    int ret = parse_hex_byte(argv[1], &byte);

    if (ret == -1) {
        fprintf(stderr, "Invalid hex value: %s\n", argv[1]);
        return 1;
    }

    if (ret == -2) {
        fprintf(stderr, "Hex value out of range 0x00..0xFF: %s\n", argv[1]);
        return 1;
    }

    int fd = open(I2C_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open I2C device " I2C_PATH);
        return 1;
    }

    struct i2c_msg_s msg;
    struct i2c_transfer_s xfer;

    memset(&msg, 0, sizeof(msg));
    memset(&xfer, 0, sizeof(xfer));

    msg.frequency = 100000; /* 100 kHz */
    msg.addr      = I2C_ADDRESS;
    msg.flags     = 0;      /* запись */
    msg.buffer    = &byte;
    msg.length    = 1;

    xfer.msgv = &msg;
    xfer.msgc = 1;

    if (ioctl(fd, I2CIOC_TRANSFER, (unsigned long)&xfer) != 0) {
        perror("Failed to transfer I2C data");
        close(fd);
        return 1;
    }

    printf("Data 0x%02X sent successfully to I2C address 0x%02X on %s\n",
           byte, I2C_ADDRESS, I2C_PATH);

    close(fd);
    return 0;
}
