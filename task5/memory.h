#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <map>
#include <memory>

using word_t = int64_t;
using mem_t = uint64_t;
using word_ptr = std::shared_ptr<word_t>;
using vector_t = std::vector<word_t>;
using map_t = std::map<const char *, word_ptr>;

class IndexOutOfBound : public std::exception {
public:
    const char *what() const noexcept override {
        return "index out of bound";
    }
};

class NotEnoughSpaceForVariables : public std::exception {
public:
    const char *what() const noexcept override {
        return "not enough space for variables";
    }
};

class InvalidIdentifier : public std::exception {
public:
    const char *what() const noexcept override {
        return "invalid identifier";
    }
};


// Class representing memory of the computer with x64 architecture.
// It enables user to set / get content of memory cells, read zero or sign flags
// but also to map variable identifiers to corresponding them memory cells.
class Memory {
public:
    explicit Memory(mem_t size);

    void set_val(const word_t *adr, word_t val);

    const word_t *get_val(const word_t *adr) const;

    void add_variable(const char *id, word_t val);

    const word_t *find_variable(const char *id) const;

    void set_ZF(bool val);

    bool get_ZF() const;

    void set_SF(bool val);

    bool get_SF() const;

    void memory_dump(std::ostream &os) const;
    
    void memory_clear();

private:
    vector_t memory;
    map_t aliases;
    mem_t mem_size;
    mem_t aliases_count;
    bool ZF;
    bool SF;

    bool valid_address(const word_t *adr) const;
};

#endif /* MEMORY_H */
