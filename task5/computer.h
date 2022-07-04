/* 24.01.2021
 * Computer simulation object with OOAsm.
 *
 * Authors:
 * Kamil Zwierzchowski (418510)
 * Antoni Koszowski (418333)
 */


#ifndef COMPUTER_H
#define COMPUTER_H

#include "processor.h"


// Class representing computer with x64 architecture.
// Computer can boot programmes that consists of instruction of the OOAsm.
class Computer {
public:
     Computer(mem_t size);

    ~Computer() = default;

    void boot(program &p);

    void memory_dump(std::ostream &os) const;

private:
    Memory memory;
    Processor processor;
};

#endif /* COMPUTER_H */