# STM32F767ZI Blink Project

Bare-metal LED blink project for the STM32 Nucleo F767ZI board.
Written without HAL drivers — direct register manipulation.

## Project Structure

- `Src/main.c` — main application code
- `stm32f767_startup.c` — custom startup file (vector table, Reset_Handler)
- `stm32_ls.ld` — custom linker script (memory layout)
- `Inc/` — header files
- `Startup/` — CubeIDE generated startup files

## Memory Map

| Region | Address | Size |
|--------|---------|------|
| Flash  | 0x08000000 | 2MB |
| SRAM   | 0x20000000 | 512KB |

## Building

```bash
arm-none-eabi-gcc -c -mcpu=cortex-m7 -mthumb -std=gnu11 Src/main.c -o main.o
arm-none-eabi-gcc -c -mcpu=cortex-m7 -mthumb -std=gnu11 stm32f767_startup.c -o stm32f767_startup.o
arm-none-eabi-gcc -nostdlib -T stm32_ls.ld *.o -o firmware.elf
```

## Flashing

```bash
openocd -f interface/stlink.cfg -f target/stm32f7x.cfg
```

Then in GDB:
```bash
target remote localhost:3333
monitor reset init
monitor flash write_image erase firmware.elf
monitor reset init
monitor resume
```

## What I Learned

- How to write a linker script from scratch
- How to write a startup file from scratch
- Memory mapped I/O and peripheral registers
- Boot sequence from power on to main()
