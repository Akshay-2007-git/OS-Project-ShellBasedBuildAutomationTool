# Shell-Based Build Automation Tool

A mini version of the Unix `make` utility developed as an
Operating Systems and Systems Programming project.

## Project Goal

The goal of this project is to develop a build automation tool
that can analyze source files, determine dependencies, execute
compilation commands, and rebuild only the files that need to
be rebuilt.

## Week 1 Features

- Linux development environment
- C project structure
- Interactive REPL
- Basic command parsing
- Makefile
- Git repository
- GitHub repository

## Build

## Build

```bash
make
```

## Week 2 Features

- Dynamic command input
- Memory allocation using `malloc()`
- Automatic buffer expansion using `realloc()`
- Proper memory cleanup using `free()`
- Modular input handling using `input.c` and `input.h`
- Support for commands of arbitrary length


## Week 3: Command Parsing and Tokenization

Week 3 focuses on implementing the command parser for the Shell-Based Build Automation Tool.

The parser takes the complete command entered by the user and separates it into individual tokens that can later be passed to the process execution module.

### Example

Input:

```text
build> build gcc test.c -o test
```

The parser tokenizes the command into:

```text
build
gcc
test.c
-o
test
```

### Concepts Implemented

* Command parsing
* String tokenization using `strtok()`
* Dynamic memory allocation for the argument array
* `NULL` termination of the argument list
* Preparation of arguments in `argv` format for `execvp()`
* Modular parser implementation

### Implementation Files

```text
include/parser.h
src/parser.c
```

### Example

```text
build> ls -l
```

The parser separates the command and its arguments so that they can be passed to the process execution module.

---

## Week 4: Processes and Process Control

Week 4 focuses on process creation and process control in Linux.

The project uses Unix process-management system calls to execute external commands.

### Concepts Implemented

* `fork()` to create a child process
* `execvp()` to execute external commands
* `waitpid()` to make the parent wait for the child process
* Process exit-status checking
* Error handling for process creation and command execution
* Parent-child process relationship

### Process Execution Flow

```text
User Command
     |
     v
   Parser
     |
     v
   fork()
   /    \
  /      \
Parent   Child
  |        |
waitpid() execvp()
  |        |
  +--------+
```

The parent process waits for the child process to finish before accepting the next command.

### Implementation Files

```text
include/process.h
src/process.c
```

### Example

```text
build> ls
```

The tool:

1. Parses the command.
2. Creates a child process using `fork()`.
3. The child executes `ls` using `execvp()`.
4. The parent waits using `waitpid()`.
5. The process exit status is checked.

Example successful output:

```text
Process exited with status: 0
```

---

## Week 5: Built-in Commands and Environment Variables

Week 5 focuses on implementing built-in shell commands and working with environment variables.

Unlike external commands, built-in commands are handled directly by the Shell-Based Build Automation Tool.

### Implemented Built-in Commands

| Command           | Function                               |
| ----------------- | -------------------------------------- |
| `cd <directory>`  | Changes the current working directory  |
| `pwd`             | Displays the current working directory |
| `help`            | Displays available commands            |
| `clear`           | Clears the terminal                    |
| `env`             | Displays environment variables         |
| `exit`            | Exits the tool                         |
| `build <command>` | Executes a build/compilation command   |

### System Functions Used

* `chdir()` — changes the current working directory
* `getcwd()` — obtains the current working directory
* `getenv()` — accesses environment variables

The `cd` command is handled by the parent process so that the directory change remains effective for subsequent commands.

### Implementation Files

```text
include/builtin.h
src/builtin.c
```

### Built-in Command Examples

#### Help

```text
build> help
```

Displays the available commands.

#### Current Directory

```text
build> pwd
```

Displays the current working directory.

#### Change Directory

```text
build> cd ..
```

Changes the current working directory.

#### Environment Variables

```text
build> env
```

Displays environment variables such as:

```text
HOME
USER
PATH
```

#### Clear Terminal

```text
build> clear
```

Clears the terminal screen.

#### Exit

```text
build> exit
```

Exits the Shell-Based Build Automation Tool.

---

## Integration of Weeks 3, 4 and 5

Weeks 3, 4, and 5 are integrated into the actual Shell-Based Build Automation Tool.

The overall command-processing flow is:

```text
User Input
    |
    v
Command Parser
    |
    v
Tokenization
    |
    v
Built-in Command Check
    |
    +---- Built-in ----> Execute Directly
    |
    +---- External ----> fork()
                              |
                              v
                           execvp()
                              |
                              v
                           waitpid()
                              |
                              v
                         Exit Status
```

### Build Command Integration

Example:

```text
build> build gcc test.c -o test
```

The command is processed as follows:

1. User enters the command.
2. The parser tokenizes the input.
3. The tool identifies the `build` command.
4. The `gcc` command and its arguments are passed to the process execution module.
5. `fork()` creates a child process.
6. `execvp()` executes GCC.
7. `waitpid()` makes the parent wait for compilation.
8. The compilation exit status is checked.

Successful compilation:

```text
Process exited with status: 0
```

This demonstrates the integration of:

```text
Week 3 → Command Parsing and Tokenization
Week 4 → Process Creation and Process Control
Week 5 → Built-in Commands and Environment Variables
```
---

## Week 6: Signals and Process Control

Week 6 focuses on handling Unix signals and controlling child processes in the Shell-Based Build Automation Tool.

The project was extended to handle signals safely so that the build tool continues running when an external command is interrupted.

### Concepts Implemented

* `sigaction()` for signal handling
* `SIGINT` handling for Ctrl+C
* `SIGCHLD` signal handling
* Restoring default `SIGINT` behavior in child processes
* `waitpid()` for child process control
* Handling interrupted `waitpid()` calls
* Process termination status checking
* Preventing the main build tool from terminating when Ctrl+C is pressed

### Signal Handling Behavior

When the user executes an external command, the project creates a child process using `fork()`.

The child process restores the default `SIGINT` behavior so that Ctrl+C can terminate the running command.

The parent build tool installs its own `SIGINT` handler so that the build tool itself continues running.

The process flow is:

    User Command
         |
         v
       Parser
         |
         v
       fork()
      /      \
     /        \
  Parent     Child
    |          |
 waitpid()   SIGINT = default
    |          |
    |        execvp()
    |          |
    +----------+
         |
         v
   Exit Status

### Implementation Files

    include/signals.h
    src/signals.c
    src/main.c
    src/process.c
    Makefile

### Example

Run the build tool:

    make run

Then execute a long-running command:

    build> build sleep 20

Press:

    Ctrl + C

The `sleep` process is terminated while the build tool continues running and displays the prompt again.

### Signals Used

| Signal | Purpose |
| --- | --- |
| `SIGINT` | Handles Ctrl+C |
| `SIGCHLD` | Indicates that a child process has changed state |

### Week 6 Outcome

The Shell-Based Build Automation Tool can now handle signals and child-process termination more safely while maintaining the interactive command prompt.
---

## Week 7: Pipes and Inter-Process Communication

Week 7 extends the Shell-Based Build Automation Tool with anonymous pipe support for communication between two processes.

The project now supports two-command pipelines using the Unix `pipe()` and `dup2()` system calls.

### Concepts Implemented

* Anonymous pipes
* Inter-Process Communication (IPC)
* `pipe()` system call
* `dup2()` for input/output redirection
* Two-stage pipelines
* Multiple child processes
* File descriptor management
* `waitpid()` for child process synchronization

### Pipeline Flow

    User Input
         |
         v
    Command Parser
         |
         v
    Left Command | Right Command
         |
         v
       pipe()
        /  \
       /    \
      v      v
    Child 1  Child 2
      |        |
    stdout   stdin
      |        |
      +--> Pipe <---+
           |
           v
        Output

### Example

The following command sends the output of `ls` to `grep`:

    build> ls | grep .c

The first child executes:

    ls

Its standard output is redirected to the pipe.

The second child executes:

    grep .c

Its standard input is redirected from the pipe.

### System Calls Used

| System Call | Purpose |
| --- | --- |
| `pipe()` | Creates an anonymous communication channel |
| `fork()` | Creates child processes |
| `dup2()` | Redirects standard input/output |
| `execvp()` | Executes commands |
| `close()` | Closes unused file descriptors |
| `waitpid()` | Waits for child processes |

### Supported Pipeline

The Week 7 implementation supports a two-command pipeline:

    command1 | command2

Examples:

    build> ls | grep .c

    build> ls | wc

    build> ps | grep bash

Only one pipe is supported in this Week 7 implementation.

### Implementation Files

    include/pipes.h
    src/pipes.c
    src/main.c
    Makefile

### Week 7 Outcome

The Shell-Based Build Automation Tool can now connect the output of one command directly to the input of another command using Unix pipes and file-descriptor redirection.
