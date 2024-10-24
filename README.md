# MLPT, a page table simulation API

MLPT is an API designed for teaching students the ins and outs of page table allocation, translation, and deallocation. Students may use [`page_allocate()`](#page_allocate) to allocate pages for a virtual adress (i.e. map a virtual address to a physical address). After they have used allocate, it is reall helpful to be able to see said mapping. This is exactly what the [`translate()`](#translate) function is meant for. Students can use translate on a virtual address to see its mapping, or whether it has been mapped at all. Lastly, students can deallocate virtual address by calling the [`deallocate()`](#deallocate) function on a virtual address. For all of these functions, students must also use the [config.h](#configh) to specify the _LEVELS_ and _POBITS_ that should be used in the mapping. More information on how to do this will be in the [config.h]("config.h) section. Of course, any student who wants to use this API must be able to compile it in their final build. For information on how to do this please see the [libmlpt.a](#libmlpta) section in this readme. Thanks again for using this simulation and feel free to open an issue in the [repository](https://github.com/vpq5kd/pagetable?tab=readme-ov-file#page_allocate) if you encounter any bugs or problems.

## page\_allocate()
```
#include "mlpt.h" //for the MLPT API

void page_allocate(size_t va);
```
`page_allocate()` takes one argument, `size_t va`, which is a hex value representation of a virtual address (e.g. **0x12345abcdef**). This virtual address is then allocated to a physical address using the regular page allocation formula. If the `size_t ptbr` is not set, it will set it as well.`page_allocate()` adds mappings on top of a set ptbr, so once the ptbr is set once it cannot be set again unless it is released by [`deallocate()`](#deallocate). `page_allocate()` supports multi-level allocation. See [config.h](#configh) for more details.

## translate()
```
#include "mlpt.h" //for the MLPT API

size_t translate(size_t va);
```
`translate()` takes one argument, `size_t va`, which is a hex value representation of a virtual address (e.g. **0x12345abcdef**). This virtual address is then translated to a physical address using them save formula as was used to allocate it. If the virtual address is not mapped ***or*** if `size_t ptbr` has not been set, translate will return **0xffffffffffffffff**. Otherwise, it will return a `size_t physical_address` such as **0x561e56a74def**. `translate()` supports multi-level translation. See [config.h](#configh) for more details.
 
## deallocate()
```
#include "mlpt.h" //for the MLPT API

void deallocate(size_t va, ptbrOption option);
```
`deallocate()` takes two arguments, `size_t va` and `ptbrOption option`. `size_t va` is a hex value representation of a virtual address (e.g. **0x12345abcdef**). `ptbrOption option` is a `typedef enum` with two options, `FREE_PTBR` or `LEAVE_PTBR`. `deallocate()` iterates through the levels of a virtual address and sets each of their valid bits to zero (effectively eliminating the mapping). Afterwords, it does one of two things. If `deallocate()`is called with the `FREE_PTBR` option, it sets `ptbr = 0`.

 ***WARNING: DO NOT CALL DEALLOCATE WITH FREE_PTBR UNLESS YOU HAVE DEALLOCATED ALL PREVIOUSLY ALLOCATED VIRTUAL ADDRESSES.***

Calling `deallocate()` with `LEAVE_PTBR` does the exact same thing as the prior call, but it leaves the `ptbr` intact. This means that if you have allocated multiple virtual addresses, you will still be able to access their mappings.

 


## config.h
```
/** LEVELS = number of PTEs used to translate one address. */
#define LEVELS  4

/** POBITS = number of bits used for the page offset. */
#define POBITS  12
```

config.h is a c header file that defines the number of levels (`LEVELS`)used for page allocation and the amount of offset bits (`POBITS`) in a virtual address. To redefine either of these values, you ***must*** do both of the following:

First, use a text editor of your choice to change the number of levels and offset bits to your desired values.

Next, remake the library as defined by [libmlpt.a](#libmlpta). Failure to do this step will result in the API not _actually_ updating the values of these two variables.


## libmlpt.a

libmlpt.a is the static libary produced by the `Makefile` in the github [repository](https://github.com/vpq5kd/pagetable/tree/main?tab=readme-ov-file#libmlpta). To produce this library run the `make` command in the directory where the MLPT API package is located on your machine. If you aren't using a linux shell, refer to the documentation of your particular shell on how to install make. 

To use this library, you must add the following to the linking step at compile time: `-L/path/to/MLPT_API_PACKAGE -lmlpt`. This will appropriately compile the static with the rest of your object files to create your final executable.
