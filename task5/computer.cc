#include "computer.h"
#include "processor.h"
#include "memory.h"

Computer::Computer(mem_t size) : memory(size), processor() {}

void Computer::boot(program &p) {
	memory.memory_clear();
	processor.execute(p, &memory);
}

void Computer::memory_dump(std::ostream &os) const {
	memory.memory_dump(os);
}
