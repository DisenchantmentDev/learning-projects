/* A rewrite of sleep() in C that compiles with gcc's -nostdlib flag
 * This project is entirely pointless but it's a great way of learning
 * how to code with the compiler. This is not a complete rewrite
 * of sleep; the actual sleep binary has quite a bit of functionality
 * that is not accounted for here, the goal is just to have a very
 * basic reimplementation of sleep from scratch.
 * Compile with:
 * gcc sleep.c -nostdlib -fno-stack-protector -o sleep*/

#define SYS_write 1
#define SYS_nanosleep 35
#define SYS_exit 60

typedef struct timespec
{
    long tv_sec;
    long tv_nsec;
} timespec;

/* Pretty much a wrapper around asm behavior for linux syscalls. This is called
 * by our own exit implementation, with the use of 60 because that's just
 * the x86_64 linux code for exit. Gotta love magic numbers in the kernel.*/
long
syscall1 (long number, long arg1)
{
    // register order: rdi, rsi, rdx, r10, r8, r9
    long result;
    __asm__ __volatile__ ("syscall"
                          : "=a"(result)
                          : "a"(number), "D"(arg1)
                          : "rcx", "r11", "memory");

    return result;
}

long
syscall2 (long number, long arg1, long arg2)
{
    long result;
    __asm__ __volatile__ ("syscall"
                          : "=a"(result)
                          : "a"(number), "D"(arg1), "S"(arg2)
                          : "rcx", "r11", "memory");

    return result;
}

long
syscall3 (long number, long arg1, long arg2, long arg3)
{
    long result;
    __asm__ __volatile__ ("syscall"
                          : "=a"(result)
                          : "a"(number), "D"(arg1), "S"(arg2), "d"(arg3)
                          : "rcx", "r11", "memory");

    return result;
}

int
parse_int (char *raw_int)
{
    int result = 0;

    char *cursor = raw_int;

    while (*cursor >= '0' && *cursor <= '9')
        {
            result = result * 10 + (*cursor - '0');
            cursor++;
        }
    return result;
}

long unsigned
strlen (char *str)
{
    char *cursor = str;
    while (*cursor)
        {
            cursor++;
        }

    long unsigned result = cursor - str;
    return result;
}

void
print (char *str)
{
    syscall3 (SYS_write, 1, (long)str, (long)strlen (str));
}

void
sleep (long seconds)
{
    timespec duration = { 0 };
    duration.tv_sec = seconds;
    syscall2 (SYS_nanosleep, (long)(&duration), 0);
}

int
main (int argc, char *argv[])
{
    if (argc != 2)
        {
            print ("Usage: sleep #\nPause for # seconds\n");
            return 1;
        }

    char *raw_seconds = argv[1];
    long seconds = parse_int (raw_seconds);

    print ("Sleeping for ");
    print (raw_seconds);
    print (" seconds\n");
    sleep (seconds);

    return 0;
}

void
exit (int code)
{
    syscall1 (SYS_exit, code);

    // this is purely to make gcc shut up about returning in a void function
    for (;;)
        {
        }
}

/* A rewrite of _start since we compile with -nostdlib. __attribute__ ((naked))
 * tells gcc to not do any prologue work on the entry point. Without that, the
 * registers we are working with are filled with a bunch of random junk that
 * cuases a seg fault. But this also means we pretty much have to write
 * the entire thing in in-line asm.
 * Line by line breakdown:
 * 1: we need to initialize the program by setting the stack frame pointer to
 *    zero. Do this by just doing xoring the stack pointer with itself
 * 2: move the value pointed to by memory address stored in rsp into rdi.
 *    the dereference is done via the paranthesis around rsp. rsp does not
 *    store the argument itself, but instead the memory address of that
 *    argument.
 * 3: Same thing as line 2, except instead of move, we use lea (load effective
 *    address) which allows us to mov the data offset by 8 bytes in memory. We
 *    then move that into rsi.
 * 4: Before we can call main, we need to account for the inherent 16 bit
 *    allignment on process entry. In order to ensure the stack is aligned, we
 *    just do and on rsp with 0xFFFFFFFFFFFFFFF0 (max 64 bit value minus last
 *    four bits).
 * 5: call main
 * 6: move the exit code returned by main into rdi (exit code is stored in next
 *    available register, which in this case is rax).
 * */
__attribute__ ((naked)) void
_start ()
{
    __asm__ __volatile__ ("xor %ebp, %ebp\n"
                          "mov (%rsp), %rdi\n"
                          "lea 8(%rsp), %rsi\n"
                          "and $-16, %rsp\n"
                          "call main\n"
                          "mov %rax, %rdi\n"
                          "call exit\n");
}
