# Arm-v8

This is **Group 5**'s repository for the Arm-v8 Emulator/Assembler/Extended project.

## Quick-Start

These guides assume you're working in the `src` directory:
```shell
$ cd src
```

### 1 - Emulator
1. Build the emulator:
```shell
$ make emulate
```
2. Run the emulator:
```shell
$ ./emulate <file_in> <file_out>
```
where
- `<file_in>` is the binary file to emulate
- `<file_out>` (optional) is the output file. If not specified, output will be printed to`stdout`

Example:
```shell
$ ./emulate add01.bin add01.out
```

### 2 - Assembler
1. Build the assembler:
```shell
$ make assemble
```
2. Run the emulator:
```shell
$ ./assemble <file_in> <file_out>
```
where
- `<file_in>` is the AArch64 source file to assemble
- `<file_out>` is the output AArch64 binary code file

Example:
```shell
$ ./assemble add01.s add01.bin
```

### 3 - Blinking the RPi
1. Compile the assembler:
```shell
$ make assemble
```
2. Assemble `led_blink.s`:
```shell
$ ./assemble rpi/led_blink.s kernel8.img
```
3. Transfer `kernel8.img` to the RPi
