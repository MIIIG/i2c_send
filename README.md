# NuttX I2C Send Application

Simple I2C send application for NuttX that sends a hex value to an I2C device.

## Configuration Requirements

1. `CONFIG_I2C=y` - Enable I2C support
2. `CONFIG_I2C_CHAR=y` - Enable I2C character device support
3. `CONFIG_I2C_SEND=y` - Enable this specific example application

## How to Build
```Bash
make
```

## How to Run

The application works on the NuttX shell:

```bash
./nuttx
```

# Test commands

This will send the value 0x1A (26 in decimal) to the I2C device at address 0x38.

```Bash
nsh> i2c_send 0x00
nsh> i2c_send 0x1A
nsh> i2c_send 0xFF
nsh> i2c_send
nsh> i2c_send 0x100
nsh> i2c_send ZZ
```

# Check address
```Bash
nsh> i2c dev 00 38
```



## Troubleshooting

If you encounter `sim_i2cbus_initialize failed` errors:

1. Verify that I2C support is enabled in your configuration
2. Check that you're using a proper I2C bus configuration for your target platform
3. For simulation environments, ensure the I2C simulator is properly initialized
4. Confirm that the device at address 0x38 exists on the I2C bus

## Hardware Notes

This application uses:
- Device path: `/dev/i2c0`
- I2C address: `0x38` (56 in decimal)
- Frequency: 100 kHz
