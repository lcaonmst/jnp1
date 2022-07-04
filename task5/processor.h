#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "ooasm.h"

// Class representing processor, responsible for executing programme instructions.
class Processor {
public:
    Processor() =  default;

    ~Processor() = default;

    void execute(program &p, Memory *memory);
};

#endif /* PROCESSOR_H */
