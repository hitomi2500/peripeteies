#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

void cartridge_memory_test(void);
bool test_memory_range(void * address, int size, int speed, bool preread);
bool test_memory_range_aliased(void * write_address, void *  read_address, int size, int speed, bool preread);

#endif