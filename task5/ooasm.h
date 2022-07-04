#ifndef OOASM_H
#define OOASM_H

#include "elements.h"

// This is a kind of wrapper for OOAsm elements, while using smart pointers
// enables to effectively make use of class hierarchy without slicing errors.

using Num_ptr = std::shared_ptr<Num>;
using RVal_ptr = std::shared_ptr<RValue>;
using LVal_ptr = std::shared_ptr<LValue>;
using Instr_ptr = std::shared_ptr<Instruction>;

using program = std::vector<Instr_ptr>;

Num_ptr num(word_t val);

RVal_ptr lea(const char* id);

LVal_ptr mem(const RVal_ptr &addr);

Instr_ptr data(const char *id, const Num_ptr &val);

Instr_ptr mov(const LVal_ptr &dst, const RVal_ptr &src);

Instr_ptr add(const LVal_ptr &arg1, const RVal_ptr &arg2);

Instr_ptr inc(const LVal_ptr &arg);

Instr_ptr sub(const LVal_ptr &arg1, const RVal_ptr &arg2);

Instr_ptr dec(const LVal_ptr &arg);

Instr_ptr one(const LVal_ptr &arg);

Instr_ptr onez(const LVal_ptr &arg);

Instr_ptr ones(const LVal_ptr &arg);

#endif /* OOASM_H */
