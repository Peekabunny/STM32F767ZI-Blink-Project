# STM32F767ZI Blink Project

Bare-metal LED blink project for the STM32 Nucleo F767ZI board.
Written without HAL drivers — direct register manipulation.

## Project Structure

- `Src/main.c` — main application code
- `stm32f767_startup.c` — custom startup file (vector table, Reset_Handler)
- `stm32_ls.ld` — custom linker script (memory layout)
- `Makefiles/` — Makefile build system
- `Inc/` — header files
- `Startup/` — CubeIDE generated startup files

## Memory Map

| Region | Address    | Size  |
|--------|------------|-------|
| Flash  | 0x08000000 | 2MB   |
| SRAM   | 0x20000000 | 512KB |

## Understanding the Build Process

The build process converts C source code into a binary that runs directly on the microcontroller with no operating system involved.

Each step:
- **Preprocessing** — expands macros and includes headers
- **Compilation** — translates C into ARM assembly
- **Assembly** — converts assembly into machine code object files
- **Linking** — combines object files using the linker script into one executable
- **Flashing** — writes the binary into Flash memory via OpenOCD

## GNU Toolchain

| Tool | Command | Purpose |
|------|---------|---------|
| Compiler | `arm-none-eabi-gcc` | Compiles C/C++ to object files |
| Linker | `arm-none-eabi-ld` | Links object files into executable |
| Debugger | `arm-none-eabi-gdb` | Debugs and flashes firmware |
| Object copy | `arm-none-eabi-objcopy` | Converts ELF to binary/hex |
| Size tool | `arm-none-eabi-size` | Shows memory usage |

### Key Compiler Flags

| Flag | Meaning |
|------|---------|
| `-mcpu=cortex-m7` | Target the Cortex-M7 CPU |
| `-mthumb` | Use Thumb instruction set |
| `-std=gnu11` | Use C11 standard |
| `-c` | Compile only, do not link |
| `-nostdlib` | Do not use standard C library |
| `-T stm32_ls.ld` | Use custom linker script |

## Linker Script

The linker script tells the linker exactly where to place each section in memory.

| Section | Location | Purpose |
|---------|----------|---------|
| `.isr_vector_tbl` | Flash (start) | Interrupt vector table |
| `.text` | Flash | Executable code |
| `.rodata` | Flash | Constants and string literals |
| `.data` | Flash → SRAM | Initialized variables |
| `.bss` | SRAM | Uninitialized variables (zeroed at boot) |

## Startup File

The startup file runs before `main()` and prepares the system:

1. Defines the interrupt vector table
2. Sets the stack pointer to the top of SRAM (`0x20080000`)
3. Copies `.data` section from Flash to SRAM
4. Zeros out the `.bss` section
5. Calls `main()`

## Makefile Build System

Instead of running compiler commands manually, the project uses GNU Make to automate the build process.

### Makefile Variables

| Variable | Value | Purpose |
|----------|-------|---------|
| `CC` | `arm-none-eabi-gcc` | The compiler |
| `CFLAGS` | `-c -mcpu=cortex-m7 -mthumb -std=gnu11` | Compiler flags |
| `LDFLAGS` | `-nostdlib -T stm32_ls.ld -Wl,-Map=...` | Linker flags |

### Special Variables

| Variable | Meaning |
|----------|---------|
| `$@` | The target file (left side of `:`) |
| `$^` | All prerequisites (right side of `:`) |

### Makefile Rules

| Rule | Command | Purpose |
|------|---------|---------|
| `make final` | Builds everything | Compiles and links into `.elf` |
| `make load` | Launches OpenOCD | Prepares board for flashing |
| `make clean` | `rm -f *.o *.elf *.map` | Removes all generated files |

### Makefile

```makefile
CC = arm-none-eabi-gcc
CFLAGS = -c -mcpu=cortex-m7 -mthumb -std=gnu11
LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=5_makefile_project_v2.map

final : 5_makefile_project_v2.elf

main.o : main.c
	$(CC) $(CFLAGS) $^ -o $@

stm32f767_startup.o : stm32f767_startup.c
	$(CC) $(CFLAGS) $^ -o $@

5_makefile_project_v2.elf : main.o stm32f767_startup.o
	$(CC) $(LDFLAGS) $^ -o $@

load :
	openocd -f board/st_nucleo_f7.cfg

clean:
	rm -f *.o *.elf *.map
```

## Building with Make

```bash
make final   # compile and link
make load    # launch OpenOCD
make clean   # clean build directory
```

## Flashing with GDB

Open a second terminal after running `make load`:

```bash
arm-none-eabi-gdb
target remote localhost:3333
monitor reset init
monitor flash write_image erase 5_makefile_project_v2.elf
monitor reset init
monitor resume
quit
```

## What I Learned

- How the GNU ARM toolchain works end to end
- How to write a linker script from scratch
- How to write a startup file from scratch
- The full build pipeline from C source to running firmware
- How to use GNU Make to automate the build process
- Makefile variables (`CC`, `CFLAGS`, `LDFLAGS`) and special variables (`$@`, `$^`)
- Memory mapped I/O and peripheral registers
- Boot sequence from power on to `main()`
