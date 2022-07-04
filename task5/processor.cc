#include "processor.h"

void Processor::execute(program &p, Memory *memory) {
	for (auto const &instr : p) {
		instr->init(memory);
	}
	for (auto const &instr : p) {
		instr->execute(memory);
	}
}
