    



#include <iostream>
#include <unistd.h>


using namespace std;









int main()
{
    int filedes[2];
    if (pipe(filedes) == -1) {
      perror("pipe");
      exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
      perror("fork");
      exit(1);
    } else if (pid == 0) {
      while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
      close(filedes[1]);
      close(filedes[0]);
      execl(cmdpath, cmdname, (char*)0);
      perror("execl");
      _exit(1);
    }
    close(filedes[1]);

    char buffer[4096];
    while (1) {
      ssize_t count = read(filedes[0], buffer, sizeof(buffer));
      if (count == -1) {
        if (errno == EINTR) {
          continue;
        } else {
          perror("read");
          exit(1);
        }
      } else if (count == 0) {
        break;
      } else {
        handle_child_process_output(buffer, count);
      }
    }
    close(filedes[0]);
    wait(0);
  }