```doxygen
/**
 * Increase the capacity of the aliases by \param step
 * and realloc them with the new capacity.
 *
 * @param [in] step how much to increase the aliases capacity by
 */
static void realloc_aliases(const ssize_t step);
```

```doxygen
/**
 * Grow the aliases by 1 and realloc if over aliases_capacity.
 */
static void grow_aliases();
```

```doxygen
/**
 * Shrink the aliases (by reallocing)
 * if aliases_capacity is more than aliases_capacity_step greater than num_aliases.
 */
static void shrink_aliases();
```

```doxygen
/**
 * Set the name and value of the Alias at index \param index
 * to \param name and \param value.
 *
 * @param [in] index the index of the Alias
 * @param [in] name the new name of the Alias
 * @param [in] value the new value of the Alias
 */
void set_Alias(const size_t index, const char *const name, const char *const value);
```

```doxygen
/**
 * Free the fields of an Alias.
 *
 * @param [in, out] alias the Alias to free
 */
static void free_Alias(const Alias *const alias);
```

```doxygen
/**
 * Get the index of an Alias by \param name.
 *
 * @param [in] name the name of the Alias
 * @return the index of the Alias, or -1 if it doesn't exist
 */
static ssize_t get_Alias_index_by_name(const char *const name);
```

```doxygen
/**
 * Get the index of an Alias by \param value.
 *
 * @param [in] value the value of the Alias
 * @return the index of the Alias, or -1 if it doesn't exist
 */
static ssize_t get_Alias_index_by_value(const char *const value);
```

```doxygen
/**
 * Remove the Alias at index \param i.
 *
 * @param [in] i the index of the Alias to remove
 */
static void remove_Alias_at_index(const size_t i);
```

```doxygen
/**
 * Free all the aliases.
 */
void free_aliases();
```

```doxygen
/**
 * Add an alias with the name \param name and the value \param value.
 *
 * @param [in] name the name of the new alias
 * @param [in] value the value of the new alias
 */
void add_alias(const char *name, const char *value);
```

```doxygen
/**
 * Remove the alias with the name \param name.
 *
 * @param [in] name the name of the alias to remove
 * @return true if the alias was removed (false if it didn't exist)
 */
bool remove_alias_by_name(const char *name);
```

```doxygen
/**
 * Remove the alias with the value \param value.
 *
 * @param [in] value the value of the alias to remove
 * @return true if the alias was removed (false if it didn't exist)
 */
bool remove_alias_by_value(const char *value);
```

```doxygen
/**
 * Get the value of an alias with name \param name.
 *
 * @param [in] name the name of the alias
 * @return the value of the alias or NULL if that alias doesn't exist
 */
const char *get_alias_value(const char *name);
```

```doxygen
/**
 * Get the name of an alias with value \param value.
 *
 * @param [in] value the value of the alias
 * @return the name of the alias or NULL if that alias doesn't exist
 */
const char *get_alias_name(const char *value);
```

```doxygen
/**
 * Print the name and value of an Alias with correct formatting
 * (as per the alias Builtin command).
 *
 * @param [in] name the name of the alias
 * @param [in] value the value of the alias
 */
void print_alias(const char *name, const char *value);
```

```doxygen
/**
 * Run \param callback on the name and value of each Alias.
 *
 * @param [in] callback the name-value callback to run on the name and value of each Alias
 */
void for_each_alias(void (*callback)(const char *name, const char *value));
```

```doxygen
/**
 * Return number of commands in a single line.
 * ;, |, <, > are command delimiters (the only ones allowed now).
 * Anything in quotes are skipped.
 *
 * If there is a syntax error in the line,
 * ~i is returned, where i is the index of the syntax error.
 *
 * @param [in] line the line
 * @return the number of commands in the line or the ~index (NOT) of the syntax error
 */
static ssize_t find_num_commands(const char *line);
```

```doxygen
/**
 * Return a newly-allocated string command by either copying the old one
 * or modifying it and returning a new string.
 *
 * Unquotes the command.
 *
 * @param [in, out] command the original string command
 * @param [in] length the length of the \param command
 * @return a newly-allocated string command
 */
static const char *get_str_command(char *command, const size_t length);
```

```doxygen
/**
 * Parses a line into multiple commands.
 * Anything in quotes is skipped.
 *
 * This should be called after #find_num_commands(),
 * so \param s should be correct and \param commands should be the right size.
 *
 * @param [in] line the line to parse
 * @param [out] commands the array of commands
 */
static void parse_commands_to_strings(char *line, _StrCommand *commands);
```

```doxygen
/**
 * Return number of args in a single command and assigns \param stripped length.
 * Handles extra whitespace.
 *
 * @param [in] command the command
 * @param [out] stripped_length the length of the command once extra whitespace has been stripped
 * @return the number of args in the command
 */
static size_t find_num_args(const char *command, size_t *stripped_length);
```

```doxygen
/**
 * Copy and unquote the argument.
 *
 * @param arg the argument
 * @param length the length of the argument
 * @return the copied and unquoted argument
 */
static char *get_arg(char *const arg, const size_t length);
```

```doxygen
/**
 * Parses a command into its args, skipping extra whitespace (' ', '\t', '\n', '\r').
 * Anything inside quotes isn't parsed at all.
 * Uses a temporary buffer \param buf to hold the stripped command, but no references to it are made.
 *
 * This should be called after #find_num_args() to determine how large \param argv must be.
 *
 * @param [in] command the command to parse
 * @param [out] argv the null-terminated argument vector to place the newly-allocated args into
 * @param [out] buf the buffer to temporarily hold the whitespace-stripped command
 */
static void parse_command_with_buf(const char *command, const char **argv, char *buf);
```

```doxygen
/**
 * Set the fields of a Command from parsing a new \param command.
 *
 * The Command#delimiter field is not set.  It should be set separately.
 *
 * Command#argv and Command#buf are grown if needed.
 * Their new lengths are set in Command#max_argc and Command#buf_len.
 *
 * The Command is also recursively preprocessed using #preprocess_Command()
 * until #preprocess_Command() returns the same str command,
 * meaning there are no more preprocessing changes.
 *
 * @param [in] command_to_parse the command to parse
 * @param [out] command the Command containing the argv, argc, and buf
 */
static void parse_command(const char *const command_to_parse, Command *const command);
```

```doxygen
/**
 * Free the fields of a StrCommand.
 *
 * @param [out] command the StrCommand to free
 */
static void free_StrCommand(_StrCommand *const command);
```

```doxygen
/**
 * Free the fields of a Command.
 *
 * @param [out] command the Command to free
 */
static void free_Command(Command *const command);
```

```doxygen
/**
 * Parses a \param line into the argvv from Commands#argvv_ptr.
 * Commands#argvc_ptr and Commands#max_argvc_ptr are also updated accordingly.
 *
 * If there is no error, returns 0.
 * If there was a syntax error in the \param line, the ~index (NOT) of the syntax error is returned.
 *
 * Commands#argvv_ptr is not null-terminated.
 *
 * @param [in] line the line to parse
 * @param [out] commands the struct containing pointers to the list of Commands that will be set
 * @return the ~index (NOT) of any syntax error in \param command, or 0 if none
 */
ssize_t parse_commands(char *line, Commands *commands);
```

```doxygen
/**
 * Free and zero/NULL the fields of the \param commands.
 *
 * @param [out] commands ptr to the Commands to free
 */
void free_Commands(Commands *commands);
```

```doxygen
/**
 * Return a newly-allocated string representation of a \param command.
 *
 * @param command the command to convert to a string
 * @return a newly-allocated string repr. of \param command
 */
char *Command_to_string(const Command *command);
```

```doxygen
/**
 * Return a newly-allocated string representation of \param commands.
 *
 * @param commands the commands to convert to a string
 * @return a newly-allocated string repr. of \param commands
 */
char *Commands_to_string(const Commands *commands);
```

```doxygen
/**
 * Print a single \param command.
 *
 * @param [in] command the command to print
 */
void print_Command(const Command *command);
```

```doxygen
/**
 * Print all the commands in \param commands.
 *
 * @param [in] commands the commands to print
 */
void print_Commands(const Commands *commands);
```

```doxygen
/**
 * Preprocess a \param command by substituting aliases.
 *
 * @param [in] command the command to preprocess
 * @return the new string with the substituted aliases,
 *      or the same backing string if no aliases found
 */
static const char *preprocess_aliases(const Command *const command);
```

```doxygen
/**
 * Each CommandPreprocessor must be called successively in #preprocess_Command(),
 * so the index of the CommandPreprocessor must be maintained in-between calls.
 * When all the CommandPreprocessors are done, then cycle back to the first one.
 *
 * @return the index of the next CommandPreprocessor
 */
static size_t next_command_preprocessor_index();
```

```doxygen
/**
 * Preprocess a single argument by expanding '~'s to $HOME.
 *
 * @param [in] arg the argument to preprocess
 * @return the processed argument
 */
static const char *preprocess_tilde(const char *const arg);
```

```doxygen
/**
 * Preprocess a single argument by expanding environment variables in the form $NAME.
 *
 * @param [in] arg the argument to preprocess
 * @return the processed argument
 */
static const char *preprocess_env_vars(const char *const arg);
```

```doxygen
/**
 * Preprocess a single argument.
 * If no proprocessing is done, return the same arg.
 *
 * @param [in] arg the argument to preprocess
 * @return the processed argument
 */
static const char *preprocess_arg(const char *arg);
```

```doxygen
/**
 * Preprocess a command in its raw string form before being parsed.
 *
 * @param [in] command the string command to preprocess
 * @param [in] length the length of the \param command
 * @return the preprocessed command
 */
const char *preprocess_str_command(const char *command, size_t length);
```

```doxygen
/**
 * Preprocess a Command, returning the string form of the processed command.
 *
 * @param [in] command the Command to preprocess
 * @return the string form of the command that was preprocessed
 */
const char *preprocess_Command(const Command *command);
```

```doxygen
/**
 * Preprocess a Command by calling #preprocess_arg() on each argument
 * and replacing it with the return value of #preprocess_arg().
 *
 * @param [in, out] command the Command to preprocess
 */
void preprocess_command(const Command *command);
```

```doxygen
/**
 * Execute a single Command by execvp()'ing.
 *
 * @param [in] command the command to execute
 * @return -1 if error (execvp), never returns if success
 */
static int exec_single_command(const Command *const command);
```

```doxygen
/**
 * Set up the modified input to a command (dup2() stdin).
 *
 * @param [in] pre_delim the delimiter before the command
 * @param [in] in_pipe the input pipe for the command
 * @return 0 or -1 if error
 */
static int setup_in(const char pre_delim, const Pipe in_pipe);
```

```doxygen
/**
 * Set up the modified output to a command (dup() stdout).
 *
 * @param [in] post_delim the delimiter after the command
 * @param [in] out_pipe the output pipe for the command
 * @param [in] command the command that will be run
 * @return 0 or -1 if error
 */
static int setup_out(const char post_delim, const Pipe out_pipe, const Command *const command);
```

```doxygen
/**
 * Run a single \param command
 * setting up the modified stdin/out,
 * synchronizing all the commands' processes,
 * and calling either the Builtin if it exists or exec'ing the command.
 *
 * @param [in] command the command to run
 * @param [in] builtin the Builtin to run if it exists, else NULL
 * @param [in] pre_delim the delimiter before the \param command
 * @param [in] post_delim the delimiter after the \param command
 * @param [in] in_pipe the input pipe, if it exists, else NULL
 * @param [in] out_pipe the output pipe, if it exists, else NULL
 * @param [in, out] num_processes_ready an atomic ptr to shared, mmap'ed memory
 *      storing the number of processes ready to run their commands
 * @return the return value of the Builtin or exec'd command, or -1 if error
 */
static int run_single_command(const Command *const command, const Builtin *const builtin,
                              const char pre_delim, const char post_delim,
                              const Pipe in_pipe, const Pipe out_pipe,
                              atomic_int *const num_processes_ready);
```

```doxygen
/**
 * Setup and run a single \param command.
 * Will open pipes when needed and fork if not a Builtin.
 *
 * @param [in] command the command to run
 * @param [in] i the index or the command
 * @param [in, out] pipes the array of pipes
 * @param [in, out] num_processes_ready the mmap'ed atomic ptr for memory fencing
 * @return pid of child, -1 if error
 */
static pid_t setup_and_run_single_command(const Command *const command, const size_t i, Pipe *const pipes,
                                          atomic_int *const num_processes_ready);
```

```doxygen
/**
 * Execute all the commands in a parsed line (stored in argvv)
 *
 * @param [in] commands the Commands to run
 * @return EXIT_SUCCESS or the (usually last) sub-command return value if any return not EXIT_SUCCESS
 *      or -1 if error
 */
static int run_commands(const Commands *const commands);
```

```doxygen
/**
 * Repeatedly run the REPL loop of the shell.
 *
 * @param [in, out] commands where the commands are stored
 * @param [in] user if the shell is running for the user
 */
void loop(Commands *commands, bool user);
```

```doxygen
/**
 * Initializes the shell.
 */
static void init_shell();
```

```doxygen
/**
 * Free all of the shell's resources.
 */
static void free_shell();
```

```doxygen
/**
 * Start the program and run the shell.
 *
 * @return 0 if no error
 */
int main();
```

```doxygen
/**
 * Runs the shell forever unless "exit" is typed to terminate it or a SIGKILL, etc. is sent.
 */
void run_shell();
```

```doxygen
/**
 * Reads a line from stdin, assuming the terminal is in cooked mode.
 *
 * @return the line read from stdin
 */
static char *read_line_cooked();
```

```doxygen
/**
 * Reads a line from stdin, assuming the terminal is in raw mode.
 *
 * Ctrl C raises SIGINT
 * Ctrl Z raises SIGSTOP
 * Backspaces removed a character
 * '\n' and '\r' terminate the line
 *
 * @return the line read from stdin
 */
static char *read_line_raw();
```

```doxygen
/**
 * Sets up the terminal to be in either raw or cooked mode.
 *
 * If RAW_TTY is true, then it will be in raw mode.
 * Otherwise, it will be in cooked mode.
 *
 * Cooked mode is simpler, but the terminal blocks until a newline is entered,
 * so the shell has less control.
 *
 * Raw mode offers more control, reading each character as it's typed,
 * but it's more work to process everything accurately.
 */
void setup_tty();
```

```doxygen
/**
 * Reads a line from stdin,
 * calling the respective raw and cooked version based on RAW_TTY.
 *
 * @return the line read from stdin
 */
char *read_line();
```

```doxygen
/**
 * Add an alias.
 *
 * @param [in] command the alias command in the form alias name=value,
 *      where value may be placed in quotes
 * @return EXIT_SUCCESS if no error
 */
int alias_cmd(const Command *command);
```

```doxygen
/**
 * Get the Builtin for the \param command or NULL if none match.
 *
 * @param command the command get the associated Builtin of, if any
 * @return the Builtin or NULL if none match
 */
const Builtin *get_builtin(const Command *command);
```

```doxygen
/**
 * Run any Builtin that matches the first argument in \param command
 * and return its return value.
 * If no Builtins match, set \param ran to false.
 * If a Builtin matched and ran, set \param ran to true.
 *
 * @param [in] command the command to run
 * @param [out] ran if any Builtin ran
 * @return the return value of the Builtin that was run,
 *      or EXIT_SUCCESS if no Builtin ran.
 */
int run_builtins(const Command *command, bool *ran);
```

```doxygen
/**
 * Set CURRENT_DIR to be the current directory.
 */
void set_current_dir();
```

```doxygen
/**
 * Change the directory to the first argument in \param command.
 * Will only run correctly if there is a single argument after cd.
 *
 * @param [in] command the cd command
 * @return EXIT_SUCCESS if no error, anything else if error
 */
int cd_cmd(const Command *command);
```

```doxygen
/**
 * Terminate the shell.
 *
 * @param [in] command ignored
 * @return EXIT_FAILURE to terminate the shell
 */
int exit_cmd(const Command *command);
```

```doxygen
/**
 * Print the shell's process ID.
 *
 * @param [in] command ignored
 * @return EXIT_SUCCESS if success, anything else if error
 */
int pid_cmd(const Command *command);
```

```doxygen
/**
 * Run the contents of \param filename as source.
 *
 * @param filename the name of the file to execute as source
 */
int source_file(const char *filename);
```

```doxygen
/**
 * Run the contents of the file in the first argument as source to the shell.
 *
 * @param command the source command to run
 * @return EXIT_SUCCESS if no error
 */
int source_cmd(const Command *command);
```

```doxygen
/**
 * Unalias an alias if it exists.
 *
 * @param [in] command the unalias command
 * @return EXIT_SUCCESS if no error
 */
int unalias_cmd(const Command *command);
```

```doxygen
/**
 * Get the length of a file.
 *
 * @param fd file descriptor of file
 * @return length of the file
 */
off_t file_length(int fd);
```

```doxygen
/**
 * Create a new Mutex in mmap anonymous shared memory.
 *
 * @return the newly-mmap'ed Mutex
 */
Mutex *new_shared_Mutex();
```

```doxygen
/**
 * Free a Mutex \param mutex in mmap anonymous shared memory.
 *
 * @param mutex the shared Mutex to free
 */
void free_shared_Mutex(Mutex *mutex);
```

```doxygen
/**
 * Create a new SpinLock in mmap anonymous shared memory.
 *
 * @return the newly-mmap'ed SpinLock
 */
SpinLock *new_shared_SpinLock();
```

```doxygen
/**
 * Free a SpinLock \param lock in mmap anonymous shared memory.
 *
 * @param lock the shared SpinLock to free
 */
void free_shared_SpinLock(SpinLock *lock);
```

```doxygen
/**
 * Allocate anonymous shared mmap'ed memory of size \param size.
 *
 * @param size the size of memory to map
 * @return the pointer to the memory
 */
void *malloc_shared(size_t size);
```

```doxygen
/**
 * Free anonymous shared mmap'ed memory of size \param size.
 *
 * @param addr the address to free
 * @param size the size that was allocated for the \param addr
 * @return 0 if success, -1 if error
 */
int free_shared(void *addr, size_t size);
```

```doxygen
/**
 * Use the program addr2line to convert the backtraced symbol address to
 * the file, function, and line number in the stacktrace,
 * appending the output to \param sb.
 *
 * @param addr the address in the executable to find the line number of
 * @param sb the StringBuilder to append the line number, etc. message to
 * @return -1 if error, else the exit status of the popen() call to the addr2line program
 */
static int addr2line(const void *const addr, StringBuilder *const sb);
```

```doxygen
/**
 * Print the stacktrace of a thread to stderr, assuming a POSIX system
 * (i.e. backtrace() and backtrace_symbols() exists).
 *
 * Print the process IDs in the front as [pid : ppid].
 *
 * @param sb the StringBuilder to append the stacktrace to before printing to stderr
 */
static void posix_print_stack_trace(StringBuilder *const sb);
```

```doxygen
/**
 * Catch the actual signal and append a message to the StringBuilder \param sb.
 *
 * @param signal the signal caught
 * @param code the siginfo->si_code of the signal caught
 * @param sb the StringBuilder to append the message to
 */
static void catch_signal_and_print_msg(int signal, int code, StringBuilder *const sb);
```

```doxygen
/**
 * A signal handler that prints the current stacktrace to stderr.
 *
 * @param signal the signal caught
 * @param siginfo extra info about the signal caught
 * @param context extra context about the signal caught
 */
static void stack_trace_signal_handler_posix(int signal, siginfo_t *siginfo, void *context);
```

```doxygen
/**
 * Print the stacktrace and PID of a thread to stderr.
 */
void print_stack_trace();
```

```doxygen
/**
 * Set the signal handler to a
 * signal handler that will print the current stacktrace.
 */
void set_stack_trace_signal_handler();
```

```doxygen
/**
 * Create a new StringBuilder with an \param initial_size.
 *
 * @param [in] initial_size the initial size and capacity of the StringBuilder
 * @return the StringBuilder
 */
StringBuilder *StringBuilder_new(size_t initial_size);
```

```doxygen
/**
 * Ensure this StringBuilder has a capacity of at least \param capacity.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] capacity the minimum capacity
 */
void StringBuilder_ensure_capacity(StringBuilder *sb, size_t capacity);
```

```doxygen
/**
 * Ensure this StringBuilder has a capacity of at least
 * the current size + \param more_capacity.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] more_capacity the minimum capacity to add to the current size
 */
void StringBuilder_ensure_more_capacity(StringBuilder *sb, size_t more_capacity);
```

```doxygen
/**
 * Shrink the capacity of this StringBuilder to the current size.
 *
 * @param [in, out] sb this StringBuilder
 */
void StringBuilder_shrink_to_size(StringBuilder *sb);
```

```doxygen
/**
 * Append \param size \param bytes to this StringBuilder.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] bytes the bytes to append
 * @param [in] size the size of the \param bytes
 */
void StringBuilder_append_bytes(StringBuilder *sb, void *bytes, size_t size);
```

```doxygen
/**
 * Append a string \param s of length \param size to this StringBuilder.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] s the string to append
 * @param [in] size the size of string \param s
 */
void StringBuilder_append_string_n(StringBuilder *sb, char *s, size_t size);
```

```doxygen
/**
 * Append a null-terminated string \param s to this StringBuilder.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] s the null-terminated string to append
 */
void StringBuilder_append_string(StringBuilder *sb, char *s);
```

```doxygen
/**
 * Append the contents of a \param file stream to this StringBuilder.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] file the file to append
 */
void StringBuilder_append_stream(StringBuilder *sb, FILE *file);
```

```doxygen
/**
 * Clear this StringBuilder,
 * freeing its chars and setting its size and capacity to 0.
 *
 * @param [in, out] sb this StringBuilder
 */
void StringBuilder_clear(StringBuilder *sb);
```

```doxygen
/**
 * Free this StringBuilder, clearing this StringBuilder and freeing its pointer.
 * @param [in, out] sb
 */
void StringBuilder_free(StringBuilder *sb);
```

```doxygen
/**
 * Copy \param length chars of a string \param s into a new dynamically-allocated, null-terminated string.
 *
 * @param [in] s the string to copy
 * @param [in] length the number of chars to copy
 * @return the new dynamically-allocated, null-terminated string
 */
char *str_copy_n(const char *s, size_t length);
```

```doxygen
/**
 * Copy a null-terminated string \param s into a new dynamically-allocated, null-terminated string.
 * #strlen() is used to find the length of \param s.
 *
 * @param [in] s the string to copy
 * @return the new dynamically-allocated, null-terminated string
 */
char *str_copy(const char *s);
```

```doxygen
/**
 * Find the number of occurences of \param substring in \param s as specified by #strstr().
 *
 * @param [in] s the string to search for \param substring in
 * @param [in] substring the substring to search for
 * @return the number of occurrences of \param substring in \param s
 */
size_t str_num_occurrences(const char *s, const char *substring);
```

```doxygen
/**
 * Replace the first \param num_times substring of \param s matching \param to_replace with \param replacement
 * from an offset of \param start and return it in a newly-allocated string.
 *
 * @param [in] s the string to replace parts of
 * @param [in] to_replace the string to replace
 * @param [in] replacement the replacement string
 * @param [in] start the index to start searching for \param to_replace
 * @param [in] num_times the max number of times to replace
 * @return a newly-allocated string with \param to_replace replaced with \param replacement
 */
char *str_replace(const char *s, const char *to_replace, const char *replacement,
                  size_t start, size_t num_times);
```

```doxygen
/**
 * Join \param num_strings strings, separated/delimited by a \param delimiter
 * and with \param start at the front and \param end at the end,
 * into one newly-allocated string.
 *
 * @param [in] delimiter the delimiter string
 * @param [in] strings the strings to join
 * @param [in] num_strings the number of \param strings
 * @param [in] start the start of the joined string
 * @param [in] end the end of the joined string
 * @return the joined string
 */
char *str_join_with_ends(const char *delimiter, const char *const *strings, size_t num_strings,
               const char *start, const char *end);
```

```doxygen
/**
 * Join \param num_strings strings, separated/delimited by a \param delimiter,
 * into one newly-allocated string.
 *
 * @param [in] delimiter the delimiter string
 * @param [in] strings the strings to join
 * @param [in] num_strings the number of \param strings
 * @return the joined string
 */
char *str_join(const char *delimiter, const char *const *strings, size_t num_strings);
```

```doxygen
/**
 * Unquote a string of length \param length
 * by stripping a ' or " at the front and end if present.
 * Modifies the string and returns a different pointer to the same string.
 *
 * @param [in, out] s the string to unquote
 * @param [in] the length of the string
 * @return a pointer to the unquoted string (same string)
 */
char *unquote_n(char *s, size_t length);
```

```doxygen
/**
 * Unquote a string by stripping a ' or " at the front and end if present.
 * Modifies the string and returns a different pointer to the same string.
 *
 * @param [in, out] s the string to unquote
 * @return a pointer to the unquoted string (same string)
 */
char *unquote(char *s);
```

```doxygen
/**
 * Strip any extra whitespace around the front and back of
 * the first \param length characters of the string \param s.
 * Modified the string and returns a different pointer to the same string.
 *
 * @param [in, out] s the string to strip
 * @param [in] the length of the string
 * @return a pointer to the stripped string (same string)
 */
char *strip_n(char *s, size_t length);
```

```doxygen
/**
 * Strip any extra whitespace around the front and back of \param s.
 * Modified the string and returns a different pointer to the same string.
 *
 * @param [in, out] s the string to strip
 * @return a pointer to the stripped string (same string)
 */
char *strip(char *s);
```

```doxygen
/**
 * Initialize the HOME global the environment variable HOME.
 */
void init_home();
```

