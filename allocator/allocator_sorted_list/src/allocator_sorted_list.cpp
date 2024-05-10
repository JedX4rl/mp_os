#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"

allocator_sorted_list::~allocator_sorted_list()
{
    throw not_implemented("allocator_sorted_list::~allocator_sorted_list()", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list const &other)
{
    throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list const &)", "your code should be here...");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list const &other)
{
    throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list const &)", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{

    if (space_size < sizeof(block_size_t) + sizeof(block_pointer_t))
    {
        // error_with_guard(""); // this is BAD BAD BAD BAD
        if (logger != nullptr)
        {
            logger->error("TODO");
        }
        throw std::logic_error("can't initialize allocator instance");
    }

    auto common_size = space_size + get_ancillary_space_size();
    try
    {
        _trusted_memory = parent_allocator == nullptr
                          ? ::operator new(common_size)
                          : parent_allocator->allocate(common_size, 1);
    }
    catch (std::bad_alloc const &ex)
    {
        logger->error("TODO");
    }

    allocator **parent_allocator_space_address = reinterpret_cast<allocator **>(_trusted_memory);
    *parent_allocator_space_address = parent_allocator;

    class logger **logger_space_address = reinterpret_cast<class logger **>(parent_allocator_space_address + 1);
    *logger_space_address = logger;

    size_t *space_size_space_address = reinterpret_cast<size_t *>(logger_space_address + 1);
    *space_size_space_address = space_size;

    allocator_with_fit_mode::fit_mode *fit_mode_space_address = reinterpret_cast<allocator_with_fit_mode::fit_mode *>(space_size_space_address + 1);
    *fit_mode_space_address = allocate_fit_mode;

    void **first_block_address_space_address = reinterpret_cast<void **>(fit_mode_space_address + 1);
    *first_block_address_space_address = reinterpret_cast<void *>(first_block_address_space_address + 1);

    *reinterpret_cast<void **>(*first_block_address_space_address) = nullptr;

    *reinterpret_cast<size_t *>(reinterpret_cast<void **>(*first_block_address_space_address) + 1) = space_size - sizeof(block_size_t);
}

[[nodiscard]] void *allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{

    auto requested_size = value_size * values_count;

    if (requested_size < sizeof(block_pointer_t))
    {
        requested_size = sizeof(block_pointer_t);
        warning_with_guard("requested space size was changed");
    }

    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();

    void *target_block = nullptr;
    void *previous_to_target_block = nullptr;
    void *next_to_target_block = nullptr;

    {
        void *previous_block = nullptr;
        void *current_block = get_first_avaible_block();

        while (current_block != nullptr)
        {
            size_t current_block_size = get_avaible_block_size(current_block);

            if (current_block_size >= requested_size &&
                (fit_mode == allocator_with_fit_mode::fit_mode::first_fit ||
                 fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit &&
                 (target_block == nullptr || get_avaible_block_size(target_block) > current_block_size) ||
                 fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit && (target_block == nullptr ||
                                                                                  get_avaible_block_size(target_block) <
                                                                                  current_block_size)))
            {
                previous_to_target_block = previous_block;
                target_block = current_block;
                next_to_target_block = get_avaible_block_next_block_address(current_block);
            }

            previous_block = current_block;
            current_block = get_avaible_block_next_block_address(current_block);
        }
    }

    if (target_block == nullptr)
    {
        error_with_guard("can't allocate block!");
        throw std::bad_alloc();
    }

    auto blocks_sizes_difference = get_avaible_block_size(target_block) - requested_size;
    if (blocks_sizes_difference > 0 && blocks_sizes_difference < sizeof(void *))
    {
        warning_with_guard("requested space size was changed");
        requested_size = get_avaible_block_size(target_block);
    }

    // TODO: You can do it! :)
}

void allocator_sorted_list::deallocate(
    void *at)
{
    throw not_implemented("void allocator_sorted_list::deallocate(void *)", "your code should be here...");
}

inline void allocator_sorted_list::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    throw not_implemented("inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    throw not_implemented("inline allocator *allocator_sorted_list::get_allocator() const", "your code should be here...");
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_sorted_list::get_logger() const
{
    return *reinterpret_cast<logger **>(reinterpret_cast<allocator **>(_trusted_memory) + 1);
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    throw not_implemented("inline std::string allocator_sorted_list::get_typename() const noexcept", "your code should be here...");
}

size_t allocator_sorted_list::get_ancillary_space_size() noexcept
{
    return sizeof(logger *) + sizeof(allocator *) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(void *);
}

allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const noexcept
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t));
}

void *allocator_sorted_list::get_first_avaible_block() const noexcept
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode));
}

allocator::block_size_t allocator_sorted_list::get_avaible_block_size(
        void *block_address) noexcept
{
    return *reinterpret_cast<allocator::block_size_t *>(reinterpret_cast<void **>(block_address) + 1);
}

void *allocator_sorted_list::get_avaible_block_next_block_address(
        void *block_address) noexcept
{
    return *reinterpret_cast<void **>(block_address);
}

allocator::block_size_t allocator_sorted_list::get_occupied_block_size(
        void *block_address) noexcept
{
    return *reinterpret_cast<allocator::block_size_t *>(block_address);
}