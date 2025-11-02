#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  
  // Check arguments - need at least the program to trace
  if(argc < 2) {
    fprintf(2, "Usage: strace command [args...]\n");
    exit(1);
  }
  
  // Fork a child process
  pid = fork();
  
  if(pid < 0) {
    fprintf(2, "strace: fork failed\n");
    exit(1);
  }
  
  if(pid == 0) {
    // Child process: enable tracing on self, then exec the target program
    if(trace(getpid()) < 0) {
      fprintf(2, "strace: trace failed\n");
      exit(1);
    }
    
    // Pass all arguments except "strace" itself
    exec(argv[1], argv + 1);
    
    // If exec returns, it failed
    fprintf(2, "strace: exec %s failed\n", argv[1]);
    exit(1);
  } else {
    // Parent process: just wait for child to complete
    wait(0);
  }
  
  exit(0);
}
