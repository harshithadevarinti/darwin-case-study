/* Virtual memory demo: allocate with mmap, touch pages, and watch the
 * resident size grow (demand paging / zero-fill-on-demand).
 * Report section: Memory Management */
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <mach/mach.h>

static unsigned long long resident_kb(void) {
    mach_task_basic_info_data_t info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count);
    return info.resident_size / 1024;
}

int main(void) {
    const size_t size = 64UL * 1024 * 1024; /* 64 MB */

    printf("Before mmap      : resident = %llu KB\n", resident_kb());

    char *p = mmap(NULL, size, PROT_READ | PROT_WRITE,
                   MAP_ANON | MAP_PRIVATE, -1, 0);
    if (p == MAP_FAILED) { perror("mmap"); return 1; }
    printf("After mmap (64MB): resident = %llu KB  (virtual only, no pages touched)\n",
           resident_kb());

    for (size_t i = 0; i < size / 2; i += 4096) p[i] = 1;
    printf("Touched 32MB     : resident = %llu KB  (page faults filled pages)\n",
           resident_kb());

    memset(p, 1, size);
    printf("Touched all 64MB : resident = %llu KB\n", resident_kb());

    munmap(p, size);
    printf("After munmap     : resident = %llu KB\n", resident_kb());
    return 0;
}
