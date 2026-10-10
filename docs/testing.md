# Testing — Shell-Based Build Automation Tool

## 1. Clean build

Run `make clean` followed by `make`. The project should compile without errors.

## 2. Basic command tests

Test `pwd`, `ls`, `echo`, and `whoami` in the shell.

## 3. Redirection tests

Test output redirection using `>`, input redirection using `<`, and append redirection using `>>`, if supported by the existing implementation.

## 4. Pipeline tests

Test commands connected by `|`. Confirm that output from one command is passed to the next command.

## 5. Threading tests

Run the existing concurrency feature. Confirm that all expected worker threads complete and synchronization behaves correctly.

## 6. Deadlock demonstration

Compile `src/deadlock.c` separately. Run it with `timeout 5s ./bin/deadlock`. A timeout exit code of 124 indicates the demonstration was stopped after the time limit.

## 7. Job-control tests

After job control has been integrated into the existing shell, test:

- Launching a command with `&`.
- Listing jobs with `jobs`.
- Resuming a stopped job with `bg`.
- Bringing a job to the foreground with `fg`.
- Interrupting a foreground process with Ctrl+C.
- Stopping a foreground process with Ctrl+Z.

## 8. Regression tests

Re-run the project's previously working commands, pipelines, redirection, signal handling, and concurrency tests after integration.

## Result recording

Record the command, expected result, actual result, and pass/fail status for each test. Capture terminal screenshots for the final project review.
