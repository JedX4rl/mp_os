#include <not_implemented.h>
#include <mutex>
#include <sstream>
#include <iomanip>
#include "../include/allocator_boundary_tags.h"

allocator_boundary_tags::~allocator_boundary_tags()
{
    if (get_logger() != nullptr)
    {
        trace_with_guard(get_typename() + "::~allocator_boundary_tags() was called");
    }
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
    if (get_logger() != nullptr)
    {
        trace_with_guard(get_typename() + "::~allocator_boundary_tags() finished");
    }
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept:
    _trusted_memory(other._trusted_memory)
{
    trace_with_guard(get_typename() + "::allocator_boundary_tags(allocator_boundary_tags &&other) was called");
    other._trusted_memory = nullptr;
    trace_with_guard(get_typename() + "::allocator_boundary_tags(allocator_boundary_tags &&other) finished");
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
    trace_with_guard(get_typename() + "operator=(allocator_boundary_tags &&other) was called");
    if (this == &other)
    {
        return *this;
    }
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;
    trace_with_guard(get_typename() + "operator=(allocator_boundary_tags &&other) finished");
    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->trace(get_typename() + "::allocator_boundary_tags (size_t space_size, allocator *parent_allocator, logger *logger, allocator_with_fit_mode::fit_mode allocate_fit_mode) was called");
    }
    if (space_size < get_block_meta_size())
    {
        if (logger != nullptr)
        {
            logger->error("Cannot initialize allocator: size of:" +  std::to_string(space_size) + "is too small. Minimum size is:" + std::to_string(get_block_meta_size()));
        }
        throw std::logic_error("Cannot initialize allocator");
    }
    block_size_t size_to_alloc = space_size + get_allocator_meta_size();
    try
    {
        _trusted_memory = parent_allocator == nullptr
                ? ::operator new(space_size)
                : parent_allocator->allocate(size_to_alloc, 1);
    }
    catch (std::bad_alloc const &err)
    {
        throw err;
    }

    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);

    *reinterpret_cast<allocator**>(temp_pointer) = parent_allocator;
    temp_pointer += sizeof(allocator*);

    *reinterpret_cast<class logger**>(temp_pointer) = logger;
    temp_pointer += sizeof(class logger*);

    new(reinterpret_cast<std::mutex*>(temp_pointer)) std::mutex();
    temp_pointer += sizeof(std::mutex);

    *reinterpret_cast<fit_mode*>(temp_pointer) = allocate_fit_mode;
    temp_pointer += sizeof(fit_mode);

    *reinterpret_cast<block_size_t*>(temp_pointer) = space_size;
    temp_pointer += sizeof(block_size_t);

    *reinterpret_cast<block_size_t*>(temp_pointer) = space_size;
    temp_pointer += sizeof(block_size_t);

    *reinterpret_cast<block_pointer_t*>(temp_pointer) = nullptr;

    trace_with_guard(get_typename() + "::allocator_boundary_tags (size_t space_size, allocator *parent_allocator, logger *logger, allocator_with_fit_mode::fit_mode allocate_fit_mode) finished");
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count)
{
    std::lock_guard<std::mutex> guard(get_mutex());
    debug_with_guard(get_typename() + "::allocate(size_t value_size, size_t values_count) was called");

    block_size_t requested_size = values_count * value_size;
    block_size_t size_to_alloc = requested_size + get_block_meta_size();
    block_size_t block_meta_size = get_block_meta_size();
    block_size_t allocator_meta_size = get_allocator_meta_size();

    block_pointer_t target_prev = nullptr;
    block_pointer_t target_block = nullptr;
    block_pointer_t target_next = nullptr;
    block_size_t target_size = 0;

    block_pointer_t &first_block = get_first_block();
    block_pointer_t current_block = first_block;
    block_pointer_t next_block = nullptr;

    fit_mode fit = get_fit_mode();

    block_size_t size = current_block == nullptr
            ? get_allocator_data_size()
            : (reinterpret_cast<unsigned char*>(current_block) - block_meta_size) -
                                (reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size);
    if (size >= size_to_alloc)
    {
        target_block = reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size + block_meta_size;
        target_size = size;
    }
    while (current_block != nullptr)
    {
        if (fit == fit_mode::first_fit && target_block)
        {
            break;
        }
        next_block = get_next_block(current_block);
        auto* right_boarder = next_block == nullptr
                              ? reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size + get_allocator_data_size()
                              : reinterpret_cast<unsigned char*>(next_block) - block_meta_size;
        auto* left_boarder = reinterpret_cast<unsigned char*>(current_block) + get_block_data_size(current_block);

        size = right_boarder - left_boarder;

        if ((size >= size_to_alloc) && (target_block == nullptr || (fit == fit_mode::the_best_fit && (size - requested_size < target_size - requested_size))
        || (fit == fit_mode::the_worst_fit && (size - requested_size > target_size - requested_size))))
        {
            target_prev = current_block;
            target_block = reinterpret_cast<unsigned char*>(left_boarder) + block_meta_size;
            target_size = size;
        }
        current_block = next_block;
    }
    if (target_block == nullptr)
    {
        error_with_guard(get_typename() + "There is no space to allocate memory to allocate" + std::to_string(requested_size) + "bytes");
        throw std::bad_alloc();
    }
    if (target_size - size_to_alloc < block_meta_size)
    {
        warning_with_guard("Requested size was changed from " + std::to_string(requested_size) + "to " + std::to_string(target_size - block_meta_size) + "bytes");
        requested_size = target_size - block_meta_size;
        size_to_alloc = target_size;
    }
    if (target_prev != nullptr)
    {
        target_next = get_next_block(target_prev);
        get_next_block(target_prev) = target_block;
    }
    else
    {
        target_next = first_block;
    }
    if (target_next != nullptr)
    {
        get_prev_block(target_next) = target_block;
    }

    get_block_data_size(target_block) = requested_size;
    get_block_allocator(target_block) = this;
    get_prev_block(target_block) = target_prev;
    get_next_block(target_block) = target_next;

    if (first_block == nullptr || first_block == get_next_block(target_block))
    {
        get_first_block() = target_block;
    }
    get_allocator_available_size() -= size_to_alloc;
    debug_with_guard(get_typename() + "::allocate(size_t value_size, size_t values_count) : allocated " + std::to_string(requested_size) +
                     "(+" + std::to_string(block_meta_size) + ") bytes.");

    trace_with_guard(get_typename() + "::allocate(size_t value_size, size_t values_count) finished");

    return target_block;
}

void allocator_boundary_tags::deallocate(
    void *at)
{
    std::lock_guard<std::mutex> guard(get_mutex());
    trace_with_guard(get_typename() + "::deallocate(void *at) was called");
    if (at == nullptr)
    {
        return;
    }
    log_blocks_info(get_typename() + "::deallocate(void *)");
    auto* at_allocator = get_block_allocator(at);
    block_size_t allocator_meta_size = get_allocator_meta_size();
    auto* mem_begin = reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size;
    auto* mem_end = reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size + get_allocator_data_size();
    if (at_allocator != this || at < mem_begin || at >= mem_end)
    {
        error_with_guard(get_typename() + "::deallocate(void *at) trying to deallocate non-related memory");
        throw std::logic_error(get_typename() + "::deallocate(void *at) trying to deallocate non-related memory");
    }
    block_pointer_t previous_block = get_prev_block(at);
    block_pointer_t next_block = get_next_block(at);
    block_size_t size = get_block_data_size(at);

    if (previous_block == nullptr)
    {
        get_first_block() = next_block;

    }
    else
    {
        get_next_block(previous_block) = next_block;
    }
    if (next_block)
    {
        get_prev_block(next_block) = previous_block;
    }
    get_allocator_available_size() += get_block_meta_size() + size;
    std::ostringstream out_stream(size > 0 ? " with data:" : "", std::ios::ate);
    out_stream << std::hex << std::setfill('0');

    for (size_t i = 0; i < size; ++i)
    {
        out_stream << " 0x" << std::setw(2) << static_cast<unsigned int>(
                *reinterpret_cast<unsigned char*>(at) + i);
    }

    debug_with_guard(get_typename() + "::deallocate(void *) : deallocated " + std::to_string(size)
                     + "(+" + std::to_string(get_block_meta_size()) + ") bytes" + out_stream.str() + ".");
    log_blocks_info(get_typename() + "::deallocate(void *)");
    information_with_guard(get_typename() + "::deallocate(void *) : available size is " +
                           std::to_string(get_allocator_available_size()) + " bytes.")->
    trace_with_guard(get_typename() + "::deallocate(void *) : finished.")->
    debug_with_guard(get_typename() + "::deallocate(void *) : finished.")->
    trace_with_guard(get_typename() + "::deallocate(void *at) finished");
}


inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(temp_pointer) = mode;
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    std::lock_guard<std::mutex> mutex (get_mutex());
    trace_with_guard(get_typename() + "::get_blocks_info() was called");
    auto blocks_info = create_blocks_info();
    trace_with_guard(get_typename() + "::get_blocks_info() finished");
    return blocks_info;
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return "allocator_boundary_tags";
}

inline allocator::block_size_t allocator_boundary_tags::get_allocator_meta_size() const
{
    return (sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode) + 2 * sizeof(block_size_t) + sizeof(block_pointer_t));
}

inline allocator *allocator_boundary_tags::get_allocator() const
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(_trusted_memory));
}

inline logger *allocator_boundary_tags::get_logger() const
{
    return *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*));
}

inline std::mutex &allocator_boundary_tags::get_mutex() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*);
    return *reinterpret_cast<std::mutex*>(temp_pointer);
}

inline allocator_with_fit_mode::fit_mode &allocator_boundary_tags::get_fit_mode() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex);
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(temp_pointer);
}

//allocator* + logger* + mutex + fit_mode + size + available_size + pointer

inline allocator::block_size_t allocator_boundary_tags::get_allocator_data_size() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode);
    return *reinterpret_cast<block_size_t*>(temp_pointer);
}

inline allocator::block_size_t &allocator_boundary_tags::get_allocator_available_size() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode) + sizeof(block_size_t);
    return *reinterpret_cast<block_size_t*>(temp_pointer);
}

//UPDATE )))))))))))) block: size + prev + next + allocator

inline allocator::block_pointer_t &allocator_boundary_tags::get_first_block() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex)
                    + sizeof(fit_mode) + 2 * sizeof(block_size_t);
    return *reinterpret_cast<block_pointer_t*>(temp_pointer);
}

inline allocator::block_size_t allocator_boundary_tags::get_block_meta_size() const
{
    return (sizeof(block_size_t) + 2 * sizeof(block_pointer_t) + sizeof(allocator*));
}

inline allocator::block_size_t &allocator_boundary_tags::get_block_data_size(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<block_size_t*>(reinterpret_cast<unsigned char*>(block) - get_block_meta_size());
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_next_block(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block) - sizeof(allocator*) - sizeof(block_pointer_t));
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_prev_block(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block) - sizeof(allocator*) - 2 * sizeof(block_pointer_t));
}

inline allocator *&allocator_boundary_tags::get_block_allocator(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(block) - sizeof(allocator*));
}

void allocator_boundary_tags::log_blocks_info(const std::string& func_name) const
{
    std::ostringstream out_string;
    auto blocks_info = create_blocks_info();
    auto meta_size = get_block_meta_size();
    for (auto data: blocks_info)
    {
        if (data.is_block_occupied)
        {
            out_string << "occupied " << data.block_size - meta_size << "|";
        }
        else
        {
            out_string << "available " << data.block_size << "|";
        }
    }
    debug_with_guard(func_name + "memory status: |" + out_string.str());
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::create_blocks_info() const noexcept
{
    trace_with_guard(get_typename() + "::create_blocks_info() was called");
    std::vector<allocator_test_utils::block_info> blocks_info(0);
    block_pointer_t curr_block = get_first_block(); //первый блок
    block_pointer_t next_block;

    block_size_t available_size, occupied_size;

    available_size = curr_block == nullptr
                     ? get_allocator_data_size()
                     : (reinterpret_cast<unsigned char*>(curr_block) - get_block_meta_size()) - (reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_meta_size());

    if (available_size)
    {
        blocks_info.push_back({.block_size = available_size, .is_block_occupied = false});
    }
    while (curr_block != nullptr)
    {
        next_block = get_next_block(curr_block);
        auto* right_boarder = next_block == nullptr
                              ? reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_meta_size() + get_allocator_data_size()
                              : reinterpret_cast<unsigned char*>(next_block) - get_block_meta_size();
        auto* left_boarder = reinterpret_cast<unsigned char*>(curr_block) + get_block_data_size(curr_block);

        available_size = right_boarder - left_boarder;
        occupied_size = get_block_meta_size() + get_block_data_size(curr_block);
        blocks_info.push_back({.block_size = occupied_size, .is_block_occupied = true});
        if (available_size)
        {
            blocks_info.push_back({.block_size = available_size, .is_block_occupied = false});
        }
        curr_block = next_block;
    }
    trace_with_guard(get_typename() + "::create_blocks_info() finished");
    return blocks_info;

}







