#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int numpp_helper(struct proc*p);
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_numvp(void)
{
    struct proc *p = myproc(); // Get the current process
    uint sz = p->sz;          // Process size in bytes
    int pages = sz / PGSIZE;  // Calculate the number of pages

    // Include the stack guard page
    if (sz % PGSIZE > 0) 
        pages++;

    return pages;
}

int sys_numpp(void)
{
  return numpp_helper(myproc());
}

int sys_mmap(void) {
    int num_bytes;

    // Retrieve the number of bytes from the argument
    if (argint(0, &num_bytes) < 0 || num_bytes <= 0 || num_bytes % PGSIZE != 0) {
        return 0; // Invalid input
    }

    struct proc *p = myproc();

    // Calculate the starting virtual address for the new memory
    uint old_sz = p->sz;
    uint new_sz = old_sz + num_bytes;

    // Increase the process's virtual address space
    p->sz = new_sz;

    // Return the starting virtual address of the newly mapped region
    return old_sz;
}

