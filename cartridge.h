#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#define CHIPSELECT_0 (0)
#define CHIPSELECT_1 (1)

void cartridge_memory_test(void);
void cartridge_backup_restore(void);
bool test_memory_range(void * address, int size, int speed, int chipselect, bool preread);
bool test_memory_range_aliased(void * write_address, void *  read_address, int size, int speed, int chipselect, bool preread);

#endif