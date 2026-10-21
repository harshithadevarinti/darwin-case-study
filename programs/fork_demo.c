/* Process creation on Darwin: fork + exec + wait.
 * Report section: Process and Thread Management */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("Parent PID=%d, PPID=%d\n", getpid(), getppid());
    fflush(stdout);

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        printf("Child  PID=%d, PPID=%d\n", getpid(), getppid());
        fflush(stdout);
        execlp("uname", "uname", "-a", (char *)NULL);
        perror("execlp");
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    printf("Child %d exited with status %d\n", pid, WEXITSTATUS(status));
    return 0;
}
