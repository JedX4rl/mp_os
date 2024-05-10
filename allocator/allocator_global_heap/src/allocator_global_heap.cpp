#include <not_implemented.h>
#include <exception>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger) : _logger(logger)
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(logger *logger) : _logger(logger) called");
    trace_with_guard("allocator_global_heap::allocator_global_heap(logger *logger) : _logger(logger) ended");
}

allocator_global_heap::~allocator_global_heap()
{
    trace_with_guard("allocator_global_heap::~allocator_global_heap() called");
    trace_with_guard("allocator_global_heap::~allocator_global_heap() ended");
};

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) called");
    _logger = other._logger;
    other._logger = nullptr;
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) ended");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    trace_with_guard("allocator_global_heap &allocator_global_heap::operator=(allocator_global_heap &&other) call");
    if (this == &other)
    {
        return *this;
    }
    _logger = other._logger;
    other._logger = nullptr;
    trace_with_guard("allocator_global_heap &allocator_global_heap::operator=(allocator_global_heap &&other) ended");
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    debug_with_guard("void *allocator_global_heap::allocate called");
    size_t requested_size = value_size * values_count;
    auto common_size = requested_size + sizeof(size_t) + sizeof(allocator*);
    void* memory;
    try
    {
        memory = ::operator new(common_size);
    }
    catch(std::bad_alloc &)
    {
        error_with_guard("Cannot allocate memory");
        throw std::bad_alloc();
    }
    auto** alloc = reinterpret_cast<allocator**> (memory);
    auto* block_size = reinterpret_cast<size_t*>(alloc + 1);
    *block_size = requested_size;
    *alloc = this;
    debug_with_guard("void *allocator_global_heap::allocate ended");
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(memory) + sizeof(size_t) + sizeof(allocator*));
}


void allocator_global_heap::deallocate(
    void *at)
{
    debug_with_guard("allocator_global_heap::deallocate(void *at) call");
    void* target_block_size = reinterpret_cast<unsigned char*>(at) - sizeof(size_t);
    auto* block_size = reinterpret_cast<size_t*>(target_block_size);
    size_t object_size = *block_size;
    std::string dump;
    auto* bytes = reinterpret_cast<unsigned char*>(at);
    for (int i = 0; i < object_size; ++i)
    {
        dump += std::to_string(static_cast<int>(bytes[i]));
    }
    debug_with_guard(dump);

    allocator* alloc;
    alloc = *reinterpret_cast<allocator**>(block_size - 1);
    if (alloc != this)
    {
        error_with_guard("block does not belong the allocator");
        throw std::logic_error("block does not belong the allocator");
    }
   ::operator delete (reinterpret_cast<void*>(alloc));
    debug_with_guard("allocator_global_heap::deallocate(void *at) end");
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return "allocator_global_heap";
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}