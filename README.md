# algebruuh
A lightweight interpreter for evaluating mathematical expressions and equations.

It includes a custom lexer, Pratt parser, and AST evaluator written in CPP.
The goal of the project is to experiment with language design and interpreter construction.

## Status

🚧 **Early Development**

## Prerequisites

- C/C++ compiler with make support
- Standard build tools (gcc/clang)

## Build

```bash
git clone https://github.com/anondeveg/algebruuh
cd algebruuh
make
```

## Usage

```bash
algebruuh [OPTIONS] <PATH_TO_FILE>
```

### Options

| Flag | Description |
|------|-------------|
| `-v` | **Verbose mode** - Prints tokens as they are generated |
| `-d` | **Dump mode** - Exports token vector to `dump.lex` in the current directory |
| `-i` | **Interactive mode** REPL |

### Example

Create a file with your equation:

```py
# example.bruh
print(5);
PRINT(RAND(5)); #from 0 to 5
PRINT(SIN(90)); # 1
PRINT(2+2*4); #10
PRINT(RAND(SIN(45)) * 10);
PRINT(FACTORIAL(RAND(50)));
```

Run the interpreter:

```bash
# Basic usage
algebruuh example.bruh

# With verbose output
algebruuh -v example.bruh

# Dump tokens 
algebruuh -d example.bruh
```

## Roadmap

- [x] lexer 
- [x] pratt parsing 
- [x] Support multiple equations separated by newlines
- [x] rand random number generation
- [x] factorial
- [ ] sin cos tan etc (currently only supports sin)
- [ ] add equation solver (gauss elemenation)

## Contributing

This project is in active development. Feel free to open issues or submit pull requests!

## License
[Do What The Fuck You Want To Public License](https://choosealicense.com/licenses/wtfpl/)
