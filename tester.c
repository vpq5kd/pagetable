#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //must be compiled with -lm
#include <stdalign.h>
#include <assert.h>
#include "mlpt.h"
#include "config.h"

//alignas(4096) static size_t testing_page_table[512];
//static void set_testing_ptbr(void){
//	ptbr = (size_t) &testing_page_table[0];
//}


int main() {
    // 0 pages have been allocated
    assert(ptbr == 0);
	page_allocate(0x123456abcdef);
    page_allocate(0x456789abcdef);
	size_t p = translate(0x456789abcdef);
	size_t q = translate(0x123456abcdef);
	printf("deallocate (before): %zx\n", p);
	printf("regular (before): %zx\n", q);			
	deallocate(0x456789abcdef, LEAVE_PTBR);
	p = translate(0x456789abcdef);
	q = translate(0x123456abcdef);
	assert(ptbr != 0); 
	printf("deallocate (after): %zx\n", p);	
	printf("regular (after): %zx\n", q);
    // 5 pages have been allocated: 4 page tables and 1 data
    assert(ptbr != 0);

    page_allocate(0x456789abcd00);
    // no new pages allocated (still 5)
    
    int *p1 = (int *)translate(0x456789abcd00);
    *p1 = 0xaabbccdd;
    short *p2 = (short *)translate(0x456789abcd02);
    printf("%04hx\n", *p2); // prints "aabb\n"

    assert(translate(0x456789ab0000) == 0xFFFFFFFFFFFFFFFF);
    
    page_allocate(0x456789ab0000);
    // 1 new page allocated (now 6; 4 page table, 2 data)

    assert(translate(0x456789ab0000) != 0xFFFFFFFFFFFFFFFF);
    
    page_allocate(0x456780000000);
    // 2 new pages allocated (now 8; 5 page table, 3 data)
	
	return 0;


}
