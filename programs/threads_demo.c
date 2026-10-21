/* POSIX threads on Darwin (each pthread maps to a Mach thread).
 * Report section: Process and Thread Management / CPU Scheduling */
#include <stdio.h>
#include <pthread.h>
#include <mach/mach.h>
#include <sys/sysctl.h>

#define NTHREADS 4

static void *worker(void *arg) {
    long id = (long)arg;
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    printf("Thread %ld: pthread_t=%p, tid=%llu, mach_thread=%u\n",
           id, (void *)pthread_self(), (unsigned long long)tid,
           mach_thread_self());

    volatile unsigned long x = 0;
    for (unsigned long i = 0; i < 50000000UL; i++) x += i;
    return NULL;
}

int main(void) {
    int ncpu = 0;
    size_t len = sizeof(ncpu);
    sysctlbyname("hw.ncpu", &ncpu, &len, NULL, 0);
    printf("Logical CPUs: %d\n", ncpu);

    pthread_t t[NTHREADS];
    for (long i = 0; i < NTHREADS; i++)
        pthread_create(&t[i], NULL, worker, (void *)i);

    thread_act_array_t threads;
    mach_msg_type_number_t count;
    task_threads(mach_task_self(), &threads, &count);
    printf("Mach reports %u threads in this task\n", count);

    for (int i = 0; i < NTHREADS; i++) pthread_join(t[i], NULL);
    return 0;
}
