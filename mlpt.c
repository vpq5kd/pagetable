#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //must be compiled with -lm
#include "mlpt.h"
#include "config.h"

//defined for page_allocate
#define PTESIZE 8
#define PAGE_SIZE (1 << POBITS) 
#define ALIGNMENT (1 << POBITS)

//defined for translate
#define OFFSET_MASK (PAGE_SIZE - 1)
#define PAGE_TABLE_ENTRIES (PAGE_SIZE / PTESIZE)

size_t ptbr = 0;

size_t * interpret_va(size_t va){
	size_t page_segment_size = log(PAGE_TABLE_ENTRIES) / log(2);
	size_t offset = va & OFFSET_MASK;
	int segments_size = LEVELS + 1;
	size_t * segments = malloc(segments_size * sizeof(size_t));
	size_t va_copy = va; 
	size_t segment_mask = ((1UL << page_segment_size) - 1);
	
	for (int i = segments_size - 1; i >= 0; i -= 1){
		if (i == segments_size - 1){
			segments[i] = offset;
			va_copy >>= POBITS;
		}
		else{
			segments[i] = va_copy & segment_mask;
			va_copy >>= page_segment_size;
		}
	}
	return segments;
}

size_t translate(size_t va){
	if (ptbr == 0){
		return ~0;
	}
	size_t* segments = interpret_va(va);
	
	size_t offset = va & OFFSET_MASK;
	size_t * pt = (size_t *) ptbr;
	for (int i = 0; i < LEVELS; i += 1){

		pt = (size_t *) (pt[segments[i]]);
		
		if (((size_t) pt & 1ul) == 0){
			return ~0;
		} 
		pt = (size_t *)((size_t) pt & ~OFFSET_MASK);
			
		
	}
	size_t physical_address = ((size_t) pt) | offset;
	
	free(segments);
	return physical_address;
}
void page_allocate(size_t va){
	if (ptbr == 0){
 		posix_memalign((void **)&ptbr, ALIGNMENT, PAGE_SIZE);		
		memset((void *)ptbr, 0, PAGE_SIZE);	
	}	


	size_t * segments = interpret_va(va);
	size_t * pt = (size_t *) ptbr;
	for (int i = 0; i < LEVELS; i += 1){
		size_t vpn = segments[i];
        if (pt[vpn] != 0 && ((pt[vpn] & 1UL) == 1)){
			pt = (size_t *) (pt[vpn] & ~1UL);
                	continue;
        };

        size_t *new_entry;
       	posix_memalign((void**)&new_entry, ALIGNMENT, PAGE_SIZE);
       	memset(new_entry, 0, PAGE_SIZE);

       	pt[vpn] = (size_t)new_entry | (1UL);
		pt = new_entry;
	}	
}


void deallocate(size_t va, ptbrOption option){
	if (ptbr == 0){
		return;
	}
	
	size_t * segments = interpret_va(va);
	size_t * pt = (size_t *) ptbr;
	for (int i = 0; i < LEVELS; i += 1){
		size_t vpn = segments[i];
		
        if (pt[vpn] != 0 && ((pt[vpn] & 1UL) == 1)){
			size_t * temp = (size_t *) (pt[vpn] & ~1UL);
			pt[vpn] = 0;
			pt = temp;
            continue;
        }
	}
	if (option == FREE_PTBR){
		ptbr = 0;
	}
}
