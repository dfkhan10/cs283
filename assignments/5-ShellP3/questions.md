1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

The shell ensures that all forked child processes complete before accepting new user input by using waitpid() in a loop for each child process ID (PID) after executing the pipeline.

How It Works:

Each command in the pipeline is forked into a new child process.
The parent process does not immediately return to the command prompt.
Instead, it calls waitpid() for each child to ensure that they all finish before proceeding.
What Happens If We Forget waitpid()?

Zombie Processes: The child processes will complete, but their exit statuses remain in the process table, leading to zombie processes.
Resource Leakage: Since the shell never collects exit statuses, the system wastes process table entries over time.
Race Conditions: The shell might start accepting new input before all children finish executing, leading to unpredictable behavior.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

When redirecting input/output using dup2(), it's critical to close any unused pipe ends to avoid resource leaks and deadlocks.

Why Is This Necessary?

Pipes have two ends (read & write). If a process does not close unused ends, the pipe remains open indefinitely.
For example, if a process is reading from a pipe, but the writer process fails to close its write end, the reader never detects EOF and waits forever.
What Could Go Wrong If We Leave Pipes Open?

Deadlocks: If a process keeps waiting for input that will never arrive, it gets stuck.
File Descriptor Leaks: Since file descriptors are limited, leaving pipes open exhausts system resources.
Unexpected Behavior: The shell might hang indefinitely waiting on input from a pipe that’s not properly closed.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command must be built-in because it affects the current shell process. Unlike external commands (which run in child processes), cd needs to change the working directory of the shell itself.

Why Can't cd Be an External Command?

If cd were an external command:

It Would Only Change the Directory of a Child Process
External commands execute in a separate process.
When that process exits, the parent shell remains in the same directory.

No Persistent Effect on the Shell
Since each process has its own copy of the environment, a child process cannot modify the shell’s directory.
Alternative Solutions

The only way to make cd work as an external command would be to modify the shell's environment after execution.
This is not possible in a standard process-based model.

Making cd a built-in ensures it modifies the shell's working directory, persisting after execution.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

Currently, the shell supports a fixed number (CMD_MAX) of piped commands. To support arbitrary pipelines, we need dynamic memory allocation.

How to Modify It

Dynamically Allocate Memory for Command Buffers
Instead of using a fixed-size array (command_list_t commands[CMD_MAX]), allocate memory dynamically
Use realloc() to grow the array if needed

Use a Linked List Instead of an Array
Instead of an array, store commands in a linked list
Dynamically allocate nodes as needed.

Adjust Pipe Creation
Since we don't know how many pipes we need ahead of time, we should dynamically allocate pipe file descriptors

Trade-off->Explanation
More Memory Overhead-> Requires memory allocation & deallocation.
More Complexity Managing-> memory increases implementation complexity.
Increased Flexibility-> Allows handling any number of piped commands.
Better Scalability-> No fixed limit (CMD_MAX) on pipeline length.
