# CRISP8

A CHIP-8 emulator written in C, using [raylib](https://www.raylib.com/) for windowing, input, and rendering.

## Features

- Full CHIP-8 instruction set (arithmetic, flow control, display, input, memory/timers, BCD, etc.), organized by category under `src/cpu/opcodes/`
- Fetch/decode/execute CPU core (`src/cpu/`)
- Raylib-based display and keyboard input
- Built-in disassembler and per-instruction tracer for debugging (`disasm.h`), enabled via a debug build
- Unit tests for each opcode category (`tests/`)
- Ships with the classic `c8games` ROM pack plus an extra demo ROM, so you can try it out immediately
- Nix flake for a reproducible dev shell

## Requirements

- A C compiler (`gcc`)
- [raylib](https://www.raylib.com/) installed and discoverable by the linker (`-lraylib`)
- `make`

If you use [Nix](https://nixos.org/), a flake is provided with everything set up:

```sh
nix develop
```

## Building

```sh
make
```

This builds `build/bin/crisp8` and also copies the binary to `crisp8` in the project root.

For a debug build with per-instruction tracing enabled:

```sh
make debug
```

Other useful targets:

```sh
make clean          # remove build artifacts
make fclean         # same as clean
make re              # fclean + all
make tests_run       # build and run the unit tests
```

## Usage

```sh
./crisp8 <path-to-rom>
```

For example, using one of the bundled ROMs:

```sh
./crisp8 chip8_roms/c8games/PONG
```

## Controls

CHIP-8 used a 16-key hexadecimal keypad. CRISP8 maps it to your keyboard as follows:

| CHIP-8 Key | Keyboard |
|:---:|:---:|
| 1 2 3 C | 1 2 3 4 |
| 4 5 6 D | Q W E R |
| 7 8 9 E | A S D F |
| A 0 B F | Z X C V |

## Project structure

```
include/           Public headers (cpu, opcodes, disasm, window, font, utils)
src/
  cpu/              Fetch/decode core and per-opcode implementations
  cpu/opcodes/      Opcodes grouped by category: arithmetic, display, flow, input, load
  window/           Raylib window, rendering, and input handling
  utils/            ROM loading and misc helpers
  main.c            Entry point
  run_cpu.c         Main emulation loop
tests/              Unit tests, split by opcode category
chip8_roms/         Bundled ROMs (classic c8games pack + a demo ROM)
Makefile
flake.nix           Nix dev shell
```

## Testing

Unit tests cover arithmetic, display, flow control, input, and load/store opcodes:

```sh
make tests_run
```

## License

No license has been specified for this project yet.
