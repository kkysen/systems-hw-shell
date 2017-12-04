//
// Created by kkyse on 11/27/2017.
//

#include "run_commands.h"

#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdatomic.h>

#include "shell_utils.h"
#include "builtins/builtins.h"
#include "builtins/cd.h"
#include "preprocessing.h"
#include "shell_io.h"
#include "util/pipes.h"
#include "util/shared_memory.h"

#define err(s) perror(s), fprintf(stderr, s" failed\n"), -1
#define f_err(s, fn) perror(s), fprintf(stderr, s" failed for file '%s'\n", fn), -1

/**
 * Execute a single Command by execvp()'ing.
 *
 * @param [in] command the command to execute
 * @return -1 if error (execvp), never returns if success
 */
static int exec_single_command(const Command *const command) {
    execvp(command->argv[0], (char **) command->argv);
    const char *const cmd_str = Command_to_string(command);
    fprintf(stderr, "%s couldn't be executed\n", cmd_str);
    free((char *) cmd_str);
    return -1;
    //return err("execvp");
}

/**
 * Set up the modified input to a command (dup2() stdin).
 *
 * @param [in] pre_delim the delimiter before the command
 * @param [in] in_pipe the input pipe for the command
 * @return 0 or -1 if error
 */
static int setup_in(const char pre_delim, const Pipe in_pipe) {
    switch (pre_delim) {
        case 0:
        case ';':
        case '>':
        case '<':
            break;
        
        case '|': {
            close(in_pipe.write);
            const int fd = in_pipe.read;
//            printf("dup2(fd, STDIN_FILENO), fd = %d\n", fd);
            if (dup2(fd, STDIN_FILENO) == -1) {
                perror("dup2");
                return err("dup2 failed on stdin");
            }
//            pd(fileno(stdin));
            break;
        }
        
        default: {
            // shouldn't happen
            return err("illegal delimiter");
        }
    }
    return 0;
}

/**
 * Set up the modified output to a command (dup() stdout).
 *
 * @param [in] post_delim the delimiter after the command
 * @param [in] out_pipe the output pipe for the command
 * @param [in] command the command that will be run
 * @return 0 or -1 if error
 */
static int setup_out(const char post_delim, const Pipe out_pipe, const Command *const command) {
//    debug();
//    pc(post_delim);
    switch (post_delim) {
        case 0:
        case ';':
            break;
        
        case '>':
        case '<': {
//            debug();
            const bool out = post_delim == '>';
            const int old_fd = out ? STDOUT_FILENO : STDIN_FILENO;
            const int flags = out ? O_WRONLY | O_CREAT : O_RDONLY;
            const mode_t mode = out ? S_IRUSR | S_IWUSR : 0;
            const char *const filename = command[1].argv[0];
//            debug();
            const int new_fd = open(filename, flags, mode);
//            debug();
            if (new_fd == -1) {
//                debug();
                return f_err("open", filename);
            }
//            debug();
            if (dup2(new_fd, old_fd) == -1) {
//                debug();
                return f_err("dup2", filename);
            }
//            debug();
            break;
        }
        
        case '|': {
            close(out_pipe.read);
            const int fd = out_pipe.write;
//            fprintf(stderr, "dup2(fd, STDOUT_FILENO), fd = %d\n", fd);
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("dup2");
                return err("dup2 failed on stdout");
            }
//            pd(fileno(stdout));
            break;
        }
        
        default: {
            // shouldn't happen
            return err("illegal delimiter");
        }
    }
//    debug();
    return 0;
}

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
                              atomic_int *const num_processes_ready) {
//    debug();
    
    if (setup_in(pre_delim, in_pipe) == -1 || setup_out(post_delim, out_pipe, command) == -1) {
        return -1;
    }
    
//    debug();
//    print_Command(command);
//    p("syncing");
    // must sync all processes here so that
    // execution never starts before pipes and dupes are set up
//    printf("[%d] num_processes_ready (pre): %d\n", getpid(), *num_processes_ready);
    (*num_processes_ready)--;
//    printf("[%d] num_processes_ready (post): %d\n", getpid(), *num_processes_ready);
    for (int i = 0; i < 10000 && *num_processes_ready; ++i) {
//        if (i % 1000 == 0) {
//            pd(i);
//        }
        if (*num_processes_ready < 0) {
            return -1;
        }
    }
//    debug();
//
    if (pre_delim == '<' || pre_delim == '>') {
        // do nothing for redirected file
        return EXIT_SUCCESS;
    }
    if (builtin) {
        int ret_val = builtin->call(command);
        
        // undo dup2
//        fprintf(stderr, "un-dup2'ing\n");
        if (pre_delim == '|' || post_delim == '<') {
//            fprintf(stderr, "undup2 stdin\n");
            dup2(STDIN_FILENO, in_pipe.read);
        } else if (post_delim == '>' || post_delim == '|') {
            dup2(STDOUT_FILENO, out_pipe.write);
//            fprintf(stderr, "undup2 stdout\n");
        }
        
        return ret_val;
    } else {
        return exec_single_command(command);
    }
}

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
                                          atomic_int *const num_processes_ready) {
    preprocess_command(command);
    
    const char pre_delim = command->pre_delimiter[0];
    const char post_delim = command->post_delimiter[0];
    
    const Pipe null_pipe = {.fds = {-1, -1}};
    
    #define run_single_command() \
            run_single_command(command, builtin, pre_delim, post_delim, \
                    i == 0 ? null_pipe : pipes[i - 1], pipes[i], num_processes_ready)
    
    const Builtin *const builtin = get_builtin(command);
    if (!(builtin || pre_delim == '<' || pre_delim == '>')) {
        const pid_t cpid = fork();
        if (cpid == -1) {
            return -1; // fork failed
        }
        
        if (cpid == 0) {
//            debug();
            run_single_command(); // execvp failed
            return -1;
        } else {
            return cpid;
        }
    } else {
        if (run_single_command() != EXIT_SUCCESS) {
            return -2;
        }
    }
    #undef run_single_command
    return EXIT_SUCCESS;
}

/**
 * Execute all the commands in a parsed line (stored in argvv)
 *
 * @param [in] commands the Commands to run
 * @return EXIT_SUCCESS or the (usually last) sub-command return value if any return not EXIT_SUCCESS
 *      or -1 if error
 */
static int run_commands(const Commands *const commands) {
//    debug();
//    printf("[%d] running commands\n", getpid());
    
    using_argvv_cc();
    Pipe *const pipes = (Pipe *) malloc(argvc * sizeof(Pipe));
    for (int i = 0; i < (int) argvc - 1; ++i) {
        pipes[i].read = -1;
        pipes[i].write = -1;
    }
    
    // need to mmap this to share memory across processes
    atomic_int *num_processes_ready = malloc_shared(sizeof(atomic_int));
    const bool shared_fence = num_processes_ready != NULL;
    if (shared_fence) {
        *num_processes_ready = (atomic_int) argvc;
    } else {
        perror("malloc_shared for num_processes_ready memory fence");
        // default to normal memory
        // this won't synchronize processes, but at least they'll run mostly right
        num_processes_ready = malloc(sizeof(atomic_size_t));
        *num_processes_ready = 1;
    }
    
    // create pipes
    for (size_t i = 0; i < argvc; ++i) {
        if (argvv[i].post_delimiter[0] == '|') {
            if (pipe(pipes[i].fds) == -1) {
                return EXIT_SUCCESS;
            }
        }
    }
    
    pid_t *const cpids = (pid_t *) malloc(argvc * sizeof(pid_t));
    for (size_t i = 0; i < argvc; ++i) {
        const Command *const command = argvv + i;
        const pid_t cpid = setup_and_run_single_command(command, i, pipes, num_processes_ready);
        cpids[i] = cpid;
        if (cpid == -2) {
            return EXIT_FAILURE;
        }
        if (cpid == -1) {
//            debug();
//            p("exiting early");
            *num_processes_ready = -1; // signal to children to end
            break;
        }
    }
    
    int total_ret_val = EXIT_SUCCESS;
    for (size_t i = 0; i < argvc; ++i) {
        if (cpids[i] <= 0) {
            continue;
        }
        int status;
//        p("\t\twaiting for: ");
//        print_Command(argvv + i);
        waitpid(cpids[i], &status, WUNTRACED);
//        debug();
        status = WEXITSTATUS(status);
//        p("\t\twaited for: ");
//        print_Command(argvv + i);
//        debug();
        
        // must send EOF myself b/c other programs don't do it
        if (argvv[i].post_delimiter[0] == '|') {
            if (close(pipes[i].write) == -1) {
                perror("close");
                errno = 0; // suppress
            }
//            p("closed");
//            print_Command(argvv + i);
        }
        
        if (total_ret_val == EXIT_SUCCESS) {
            total_ret_val = status;
        }
    }
    free(cpids);
    free(pipes);
    if (shared_fence) {
        free_shared(num_processes_ready, sizeof(atomic_size_t));
    } else {
        free(num_processes_ready);
    }
    return total_ret_val;
}

#undef err
#undef f_err

void loop(Commands *const commands, const bool user) {
    int status;
    do {
        if (user) {
            printf("\n%s\n$ ", CURRENT_DIR);
        }
        
        char *const line = read_line();
        if (!line) {
            break;
        }
        
        #if (DEBUG_SHELL)
        printf("read line \"%s\"\n", line);
        #endif
        
        if (line[0] == '#') {
            continue; // '#' is a comment
        }
        const ssize_t i = parse_commands(line, commands);
        if (i < 0) {
            printf("syntax error at index %zd\n", ~i);
            continue;
        }
        
        #if (DEBUG_SHELL)
        print_Commands(commands);
        #endif
        
        status = run_commands(commands);
        free_Commands(commands);
    } while (status == EXIT_SUCCESS);
}