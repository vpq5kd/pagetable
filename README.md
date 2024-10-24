# MLPT, a page table simulation API

MLPT is an API designed for teaching students the ins and outs of page table allocation, translation, and deallocation. Students may use [page\_allocate](#page_allocate) to allocate pages for a virtual adress (i.e. map a virtual address to a physical address). After they have used allocate, it is reall helpful to be able to see said mapping. This is exactly what the [translate](#translate) function is meant for. Students can use translate on a virtual address to see its mapping, or whether it has been mapped at all. Lastly, students can deallocate virtual address by calling the [deallocate](#deallocate) function on a virtual address. For all of these functions, students must also use the [config.h](#config.h) to specify the _LEVELS_ and _POBITS_ that should be used in the mapping. More information on how to do this will be in the [config.h]("config.h) section. Of course, any student who wants to use this API must be able to compile it in their final build. For information on how to do this please see the [libmlpt.a](#libmlpt.a) section in this readme. Thanks again for using this simulation and feel free to open an issue in the [repository](https://github.com/vpq5kd/pagetable?tab=readme-ov-file#page_allocate) if you encounter any bugs or problems.

## page\_allocate
```
#include "mlpt.h" //for the MLPT API

void page_allocate(size_t va);
```
page\_allocate takes one argument, `size_t va`, which is a hex value representation of a virtual address (e.g. **0x12345abcdef**). This virtual address is then allocated to a physical address using the regular page allocation formula. If the `size_t ptbr` is not set, it will set it as well. page\_allocate adds mappings on top of a set ptbr, so once the ptbr is set once it cannot be set again unless it is released by [deallocate](#deallocate). page\_allocate supports multi-level allocation. See [config.h](#config.h) for more details.

## translate
```
#include "mlpt.h" //for the MLPT API

size_t translate(size_t va);
```

## deallocate

## config.h

## libmlpt.a
