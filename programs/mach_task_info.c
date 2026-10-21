/* Query the Mach task layer directly via task_info().
 * Report section: Memory Management / Kernel Architecture (Mach layer) */
#include <stdio.h>
#include <mach/mach.h>

int main(void) {
    mach_task_basic_info_data_t info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;

    kern_return_t kr = task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                                 (task_info_t)&info, &count);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "task_info failed: %s\n", mach_error_string(kr));
        return 1;
    }

    printf("Task port (mach_task_self): %u\n", mach_task_self());
    printf("Virtual size  : %llu KB\n", (unsigned long long)info.virtual_size / 1024);
    printf("Resident size : %llu KB\n", (unsigned long long)info.resident_size / 1024);
    printf("Peak resident : %llu KB\n", (unsigned long long)info.resident_size_max / 1024);
    printf("Suspend count : %d\n", info.suspend_count);

    vm_size_t pagesize;
    host_page_size(mach_host_self(), &pagesize);
    printf("Page size     : %lu bytes\n", (unsigned long)pagesize);

    vm_statistics64_data_t vm;
    mach_msg_type_number_t vcount = HOST_VM_INFO64_COUNT;
    host_statistics64(mach_host_self(), HOST_VM_INFO64,
                      (host_info64_t)&vm, &vcount);
    printf("Free pages    : %llu\n", (unsigned long long)vm.free_count);
    printf("Active pages  : %llu\n", (unsigned long long)vm.active_count);
    printf("Inactive pages: %llu\n", (unsigned long long)vm.inactive_count);
    printf("Wired pages   : %llu\n", (unsigned long long)vm.wire_count);
    printf("Compressed    : %llu\n", (unsigned long long)vm.compressor_page_count);
    return 0;
}
