#include "elements.h"


Identifier::Identifier(const char *id): _id(id) {
    if (!is_valid())
        throw InvalidIdentifier();
};

const char * Identifier::get_id() const {
    return _id;
}

bool Identifier::is_valid() const {
    const size_t len = std::char_traits<char>::length(_id);
    return (len >= 1 && len <= 10);
}

Num::Num(word_t val): _val(val) {};

const word_t *Num::evaluate([[maybe_unused]] Memory *memory) const {
    return &_val;
}

Lea::Lea(Identifier id) : _id(id) {};

const word_t *Lea::evaluate(Memory *memory) const {
    return memory->find_variable(_id.get_id());
}

Mem::Mem(RVal_ptr addr) : _addr(addr) {};

const word_t *Mem::evaluate(Memory *memory) const {
    return memory->get_val(_addr->evaluate(memory));
}

Declaration::Declaration(Identifier id, Num_ptr val): _id(id), _val(val) {};

void Declaration::execute([[maybe_unused]] Memory *memory) {};

void Declaration::init(Memory *memory) {
    memory->add_variable(_id.get_id(), *_val->evaluate(memory));
}

Operation::Operation(LVal_ptr arg1, RVal_ptr arg2): _arg1(arg1), _arg2(arg2) {};

void Operation::init([[maybe_unused]] Memory *memory) {}

void Operation::set_flags(word_t val, Memory *memory) {
    val == 0 ? memory->set_ZF(true) : memory->set_ZF(false);
    val < 0 ? memory->set_SF(true) : memory->set_SF(false);
}

Mov::Mov(LVal_ptr arg1, RVal_ptr arg2): Operation(arg1, arg2) {};

void Mov::execute(Memory *memory) {
    memory->set_val(_arg1->evaluate(memory), *_arg2->evaluate(memory));
}

Add::Add(LVal_ptr arg1, RVal_ptr arg2): Operation(arg1, arg2) {};

void Add::execute(Memory *memory) {
    word_t val = *_arg1->evaluate(memory) + *_arg2->evaluate(memory);
    set_flags(val, memory);
    memory->set_val(_arg1->evaluate(memory), *_arg1->evaluate(memory) + *_arg2->evaluate(memory));
}

Sub::Sub(LVal_ptr arg1, RVal_ptr arg2): Operation(arg1, arg2) {};

void Sub::execute(Memory *memory) {
    word_t val = *_arg1->evaluate(memory) - *_arg2->evaluate(memory);
    set_flags(val, memory);
    memory->set_val(_arg1->evaluate(memory), val);
}

Assignment::Assignment(LVal_ptr arg): _arg(arg) {};

void Assignment::init([[maybe_unused]] Memory *memory) {}

One::One(LVal_ptr arg): Assignment(arg) {};

void One::execute(Memory *memory) {
    memory->set_val(_arg->evaluate(memory), *Num(1).evaluate(memory));
}

Onez::Onez(LVal_ptr arg): Assignment(arg) {};

void Onez::execute(Memory *memory) {
    if (memory->get_ZF())
        memory->set_val(_arg->evaluate(memory), *Num(1).evaluate(memory));
}

Ones::Ones(LVal_ptr arg): Assignment(arg) {};

void Ones::execute(Memory *memory) {
    if (memory->get_SF()) {
        memory->set_val(_arg->evaluate(memory), *Num(1).evaluate(memory));
	}
}










