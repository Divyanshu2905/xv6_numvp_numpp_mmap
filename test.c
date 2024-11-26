#include "types.h"
#include "user.h"

#define PGSIZE 4096

int main() {
    int num_virtual_pages = numvp();  // Call numvp system call
    int num_physical_pages = numpp(); // Call numpp system call

    printf(1, "Number of virtual pages: %d\n", num_virtual_pages);
    printf(1, "Number of physical pages: %d\n", num_physical_pages);

    int size = 3 * PGSIZE; // Map 3 pages
    char *addr = (char *)mmap(size);

    if (addr == 0) {
        printf(1, "mmap failed\n");
        exit();
    }

    printf(1, "Mapped memory at address: 0x%x\n", addr);
    num_virtual_pages = numvp();  // Call numvp system call
    num_physical_pages = numpp(); // Call numpp system call

    printf(1, "Number of virtual pages: %d\n", num_virtual_pages);
    printf(1, "Number of physical pages: %d\n", num_physical_pages);

    // Access the memory to trigger page faults
    addr[0] = 'A';
    addr[PGSIZE] = 'B';
    addr[2 * PGSIZE] = 'C';

    int new_virtual_pages = numvp(); // Get the virtual page count after access
    int new_physical_pages = numpp(); // Get the physical page count after access
    printf(1, "Final Virtual Pages: %d\n", new_virtual_pages);
    printf(1, "Final Physical Pages: %d\n", new_physical_pages);

    printf(1, "Accessed memory: %c %c %c\n", addr[0], addr[PGSIZE], addr[2 * PGSIZE]);

    exit();
}
