# Arm-v8

This is `Group 5`'s repository for the Arm-v8 Emulator/Assembler/Extended project.

## Guide to blinking the RPi
1. `cd` into `src`:
```bash
cd src
```
2. Compile the assembler:
```bash
make assemble
```
3. Assemble `led_blink.s`:
```bash
./assemble rpi/led_blink.s kernel8.img
```
4. Transfer `kernel8.img` onto the RPi's SD card and power it!


