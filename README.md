# AM Shell

[![progress-banner](https://backend.codecrafters.io/progress/shell/bfd39c0a-e772-4d75-8e3c-4f8f4d089418)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

A minimal POSIX-style shell written in C as part of the CodeCrafters **Build Your Own Shell** challenge.

The shell implements a REPL loop, builtin commands, external program execution and a custom tokenizer capable of handling quoting, escaping and shell operators.

---

## Current Features

### Builtin Commands

* [x] `echo`
* [x] `pwd`
* [x] `cd`
* [x] `type`
* [x] `exit`

### Command Execution

* [x] PATH lookup for executables
* [x] `fork` + `execve` execution
* [x] process waiting with `waitpid`

### Input Handling

* [x] multi-line input
* [x] quote continuation
* [x] escape characters

### Tokenization

* [x] whitespace splitting
* [x] quote handling (`'` `"`)
* [x] escape handling (`\`)
* [x] operator detection

### Operators (planned)

* [ ] pipeline `|`
* [ ] background execution `&`

### Redirection (planned)

* [ ] `>`
* [ ] `>>`
* [ ] `<`

---

## Architecture

```src/
 ├── main.c
 ├── ambuilt.c
 ├── ambuilt.h
 ├── amio.c
 ├── amio.h
```

---

## Core Components

### REPL Loop

The shell runs a simple read–execute loop:

```prompt
↓
read_line()
↓
tokenize()
↓
check builtin
↓
execute external command
```

---

### Input System (`amio`)

Handles reading and tokenizing command input.

Capabilities:

* dynamic input buffers
* multi-line continuation
* quote tracking
* escape handling

---

### Tokenizer

The tokenizer converts command strings into tokens while respecting shell rules.

Features:

* whitespace splitting
* quote preservation
* escape character handling
* operator detection

---

### Builtin System

Builtins are registered using a function pointer table.

```struct builtin
{
    char* key;
    built_func func;
};
```

Example:

```{"echo", builtin_echo}
{"cd", builtin_cd}
{"pwd", builtin_pwd}
```

---

## External Command Execution

External programs are executed through:

1. PATH lookup
2. `fork()`
3. `execve()`
4. `waitpid()`

PATH resolution checks:

* file existence
* regular file
* execute permission

---

## Build

Requirements:

* gcc
* cmake

Build:

```cmake -B build
cmake --build build
```

Run:
./your_program.sh

---

## Learning Goals

This project explores low-level UNIX concepts:

* process creation (`fork`)
* program execution (`execve`)
* file system inspection (`stat`)
* PATH resolution
* command parsing
* REPL programs

---

## Status

The shell currently supports builtin commands and execution of external programs. Operator execution (pipes, redirection, background processes) is under development.
