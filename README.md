# SAFSH - Simple As Fuck Shell

**SAFSH** is a minimalistic Unix-like command-line shell written in C, built for fun, experimentation, and learning how shells work under the hood.

## Features

- Prompt displays the **current directory name**
- Supports execution of external commands via `execvp`
- Basic built-in commands:
  - `cd` — Change the current working directory
  - `help` — Display help message for built-in commands
  - `exit` — Exit the shell
- Command line editing and history support via GNU Readline
- Handles `SIGINT` (`Ctrl+C`) gracefully
- Dynamic memory management for token parsing
- Uses `fork` and `waitpid` to manage child processes

## Installation

### Prerequisites

- GCC (or any modern C compiler)
- GNU Readline development headers (e.g., `libreadline-dev` on Debian/Ubuntu)

### Build

```bash
make
```

If `make` throws unused-variable warnings, the code is still functional. You can alternatively compile manually:

```bash
gcc safsh.c -o safsh -lreadline
```

## Usage

```bash
./safsh
```

Once running, you'll see the shell prompt display like:

```bash
foldername =>
```

You can enter any valid system command or one of the built-in commands.

## Example

```bash
~/Documents => cd ..
~/ => ls
Documents  Downloads  safsh
~/ => exit
Exiting SAFSH...
```

## Project Structure

```
.
├── safsh.c         # Main shell source code
├── Makefile        # Build instructions
└── README.md       # Project description
```

## TODO

- Add support for command piping (`|`)
- Implement input/output redirection (`<`, `>`)
- Add support for environment variables
- Tab completion (via readline)
- Script file execution

## License

This project is provided for educational purposes under the MIT License.

## Author

Created by **selfAnnihilator**
