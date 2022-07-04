#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "memory.h"
#include <cassert>

// Class representing variable identifier which can contain any characters,
// provided that its length is between 1 to 10 including.
class Identifier {
public:
    explicit Identifier(const char *id);

    const char * get_id() const;

private:
    const char * const _id;

    bool is_valid() const;
};


class RValue {
public:
    RValue() = default;

    virtual ~RValue() = default;

    virtual const word_t *evaluate(Memory *memory) const = 0;
};

using RVal_ptr = std::shared_ptr<RValue>;

class LValue : public RValue {
public:
    LValue() = default;

    virtual ~LValue() = default;

    virtual const word_t *evaluate(Memory *memory) const = 0;
};

using LVal_ptr = std::shared_ptr<LValue>;

// Class representing integer numeric literal.
class Num : public RValue {
public:
    explicit Num(word_t val);

    ~Num() = default;

    const word_t *evaluate([[maybe_unused]] Memory *memory) const override;

private:
    const word_t _val;
};

using Num_ptr = std::shared_ptr<Num>;

// Class enabling to get effective address of variable with given identifier.
class Lea : public RValue {
public:
    explicit Lea(Identifier id);

    ~Lea() = default;

    const word_t *evaluate(Memory *memory) const override;

private:
    const Identifier _id;
};

// Class enabling to get access to computer memory cell under given address.
class Mem : public LValue {
public:
    explicit Mem(RVal_ptr addr);

    ~Mem() = default;

    const word_t *evaluate(Memory *memory) const override;

private:
    const RVal_ptr _addr;
};


class Instruction {
public:
    Instruction() = default;

    virtual ~Instruction() = default;

    virtual void execute(Memory *memory) = 0;

    virtual void init(Memory *memory) = 0;
};

using Instr_ptr = std::shared_ptr<Instruction>;

// Class enabling declaration of variable.
class Declaration : public Instruction {
public:
    Declaration(Identifier id, Num_ptr val);

    ~Declaration() = default;

    void execute(Memory *memory) override;

    void init(Memory *memory) override;

private:
    const Identifier _id;
    const Num_ptr _val;
};

class Operation : public Instruction {
public:
    Operation(LVal_ptr arg1, RVal_ptr arg2);

    virtual ~Operation() = default;

    virtual void execute(Memory *memory) = 0;

    void init(Memory *memory) override;

protected:
    const LVal_ptr _arg1;
    const RVal_ptr _arg2;

    void set_flags(word_t val, Memory *memory);
};

// Class enabling operation of copying value from source [arg1]
// to destination [arg2].
class Mov : public Operation {
public:
    Mov(LVal_ptr arg1, RVal_ptr arg2);

    ~Mov() = default;

    void execute(Memory *memory) override;
};

// Class enabling addition of value evaluated from [arg2] to element stored
// in [arg2]. After arithmetic operation depending on result
// sets processor flags.
class Add : public Operation {
public:
    Add(LVal_ptr arg1, RVal_ptr arg2);

    ~Add() = default;

    void execute(Memory *memory) override;
};

// Class enabling subtraction of value evaluated from [arg2] to element stored
// in [arg2]. After arithmetic operation depending on result
// sets processor flags.
class Sub : public Operation {
public:
    Sub(LVal_ptr arg1, RVal_ptr arg2);

    ~Sub() = default;

    void execute(Memory *memory) override;
};

class Assignment : public Instruction {
public:
    explicit Assignment(LVal_ptr arg);

    virtual void execute(Memory *memory) = 0;

    void init(Memory *memory) override;

protected:
    const LVal_ptr _arg;
};

// Class enabling to set value under address evaluated from [arg] value 1.
class One : public Assignment {
public:
    explicit One(LVal_ptr arg);

    void execute(Memory *memory) override;
};

// Class enabling to set value under address evaluated from [arg] value 1,
// in case ZF is equal 1.
class Onez : public Assignment {
public:
    explicit Onez(LVal_ptr arg);

    void execute(Memory *memory) override;
};

// Class enabling to set value under address evaluated from [arg] value 1,
// in case SF is equal 1.
class Ones : public Assignment {
public:
    explicit Ones(LVal_ptr arg);

    void execute(Memory *memory) override;
};

#endif /* ELEMENTS_H */
