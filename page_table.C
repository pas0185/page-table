/*
    File: page_table.C

    Author: Patrick Sheehan
            Department of Computer Science
            Texas A&M University
    Date  : 9/28/15

    Description: Basic Paging.

*/

#include "page_table.H"

  static void init_paging(FramePool * _kernel_mem_pool,
                          FramePool * _process_mem_pool,
                          const unsigned long _shared_size) {

  /* Set the global parameters for the paging subsystem. */

    	paging_enabled = FALSE;
    	kernel_mem_pool = _kernel_mem_pool;
    	process_mem_pool = _process_mem_pool;
    	shared_size = _shared_size;
    }


	PageTable::PageTable() {

    // Initialize the Page Directory (or Outer Page Table) with an address in the Kernel
    page_directory = kernel_mem_pool->get_frame() * PAGE_SIZE;

    // Set up the directory's first page_table which maps to the Kernel's 4MB
    unsigned long * kernel_page_table = kernel_mem_pool->get_frame() * PAGE_SIZE;

    unsigned long address = 0; // holds the physical address of where a page is
    unsigned int i;

    // map the first 4MB of memory
    for(i = 0; i < ENTRIES_PER_PAGE; i++) {
      page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
      address = address + PAGE_SIZE; // 4096 = 4kb
    };

    page_directory[0] = page_table;

	}
  /* Initializes a page table with a given location for the directory and the
     page table proper.
     NOTE: The PageTable object still needs to be stored somewhere! Probably it is best
           to have it on the stack, as there is no memory manager yet...
     NOTE2: It may also be simpler to create the first page table *before* paging
           has been enabled.
  */

  void PageTable::load() {

  }
  /* Makes the given page table the current table. This must be done once during
     system startup and whenever the address space is switched (e.g. during
     process switching). */

  static void enable_paging() {

  }
  /* Enable paging on the CPU. Typically, a CPU start with paging disabled, and
     memory is accessed by addressing physical memory directly. After paging is
     enabled, memory is addressed logically. */

  static void handle_fault(REGS * _r) {

  }
  /* The page fault handler. */
