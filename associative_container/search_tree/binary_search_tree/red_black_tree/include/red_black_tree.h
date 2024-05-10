#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class red_black_tree final:
    public binary_search_tree<tkey, tvalue>
{

public:

    enum class node_color
    {
        RED = 0,
        BLACK = 1
    };


private:

    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {

    public:

        typename red_black_tree<tkey, tvalue>::node_color color;

    public:

        explicit node(
                tkey const &key,
                tvalue const &value);

        explicit node(
                tkey const &key,
                tvalue &&value);

    };

public:

    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {
        friend void red_black_tree<tkey, tvalue>::inject_additional_data(
                typename binary_search_tree<tkey, tvalue>::iterator_data *,
                typename binary_search_tree<tkey, tvalue>::node const *) const;

    public:

        typename red_black_tree<tkey, tvalue>::node_color _color;

        typename red_black_tree<tkey, tvalue>::node_color get_color() const;

    public:

        iterator_data();

        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            typename red_black_tree<tkey, tvalue>::node_color color);

        explicit iterator_data(unsigned int depth,
                      node **source_node);

        iterator_data(iterator_data const &other);

        iterator_data(iterator_data &&other) noexcept;

        iterator_data &operator=(iterator_data const &other);

        iterator_data &operator=(iterator_data &&other) noexcept;

        ~iterator_data() noexcept final;

    };

private:

    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {

    public:

        explicit insertion_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

    private:

        void balance(
                std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
                typename binary_search_tree<tkey, tvalue>::node* node_to_dispose = nullptr) override;


    };

    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {

    public:

        explicit disposal_template_method(
                red_black_tree<tkey, tvalue>* tree,
                typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    private:
        void balance(
                std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
                typename binary_search_tree<tkey, tvalue>::node* node_to_dispose = nullptr) override;

        void call_small_rotation(
                typename binary_search_tree<tkey, tvalue>::node** node_to_rotate,
                void *root,
                bool is_right) const;

        void call_big_rotation(
                typename binary_search_tree<tkey, tvalue>::node** node_to_rotate,
                void *root,
                bool is_right) const;
    };

public:

    explicit red_black_tree(
        std::function<int(tkey const &, tkey const &)> comparer = search_tree<tkey, tvalue>::default_keys_comparer(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

    ~red_black_tree() noexcept final;

    red_black_tree(
        red_black_tree<tkey, tvalue> const &other);

    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> const &other);

    red_black_tree(
        red_black_tree<tkey, tvalue> &&other) noexcept;

    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> &&other) noexcept;
protected:


private:
    size_t get_node_size() const noexcept override;

    void call_node_constructor(
            typename binary_search_tree<tkey,tvalue>::node *raw_space,
            tkey const &key,
            tvalue const &value) override
    {
        allocator::construct(reinterpret_cast<typename red_black_tree<tkey, tvalue>::node*>(raw_space), key, value);
    }

    void call_node_constructor(
            typename binary_search_tree<tkey,tvalue>::node *raw_space,
            tkey const &key,
            tvalue &&value) override
    {
        allocator::construct(reinterpret_cast<typename red_black_tree<tkey, tvalue>::node*>(raw_space), key, std::move(value));
    }

    void inject_additional_data(typename binary_search_tree<tkey, tvalue>::iterator_data *destination,
                                typename binary_search_tree<tkey, tvalue>::node const *source) const override;

    void inject_additional_data(typename binary_search_tree<tkey, tvalue>::node *destination,
                                typename binary_search_tree<tkey, tvalue>::node const *source) const override;

    typename binary_search_tree<tkey, tvalue>::iterator_data *create_iterator_data() const override;

    typename binary_search_tree<tkey, tvalue>::iterator_data *create_iterator_data(unsigned int depth, typename binary_search_tree<tkey,tvalue>::node *&src_node) const override;

    static typename red_black_tree<tkey, tvalue>::node_color get_color(typename binary_search_tree<tkey, tvalue>::node* node) noexcept;

    static inline void set_color(typename binary_search_tree<tkey, tvalue>::node* node, typename red_black_tree<tkey, tvalue>::node_color color) noexcept;

};

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::node::node(tkey const &key, tvalue const &value):
        binary_search_tree<tkey, tvalue>::node(key, value),
        color(red_black_tree<tkey, tvalue>::node_color::RED)
{

}

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::node::node(const tkey &key, tvalue &&value):
        binary_search_tree<tkey, tvalue>::node(key, std::move(value)),
        color(red_black_tree<tkey, tvalue>::node_color::RED)
{

}

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::node_color red_black_tree<tkey, tvalue>::iterator_data::get_color() const
{
    if (this->_is_state_initialized)
    {
        return _color;
    }
    throw std::logic_error("Cannot read data from uninitialized iterator");
}


template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int depth, red_black_tree::node** source_node):
        binary_search_tree<tkey, tvalue>::iterator_data(depth, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**> (source_node))
{
    if (*source_node != nullptr)
    {
        _color = (*source_node)->color;
    }
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    typename red_black_tree<tkey, tvalue>::node_color color):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
    _color(color)
{

}

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data():
        binary_search_tree<tkey, tvalue>::iterator_data(),
        _color( red_black_tree<tkey, tvalue>::node_color::RED)
{

}

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(red_black_tree::iterator_data const &other):
        binary_search_tree<tkey, tvalue>::iterator_data(other),
        _color(other._color)
{

}

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(red_black_tree::iterator_data &&other) noexcept :
        binary_search_tree<tkey, tvalue>::iterator_data(other),
        _color(std::move(other._color))
{

}

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::iterator_data &red_black_tree<tkey, tvalue>::iterator_data::operator=(red_black_tree::iterator_data const &other)
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(other);
        _color = other._color;
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::iterator_data &red_black_tree<tkey, tvalue>::iterator_data::operator=(red_black_tree::iterator_data &&other) noexcept
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(std::move(other));
        _color = std::move(other._color);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::~iterator_data() noexcept
{

}


template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{

}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{

}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>(
                new (std::nothrow) red_black_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
                new (std::nothrow) typename binary_search_tree<tkey,tvalue>::obtaining_template_method(this),
                new (std::nothrow) red_black_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
                comparer, allocator, logger)
{
    if (this->_insertion_template == nullptr || this->_obtaining_template == nullptr || this->_disposal_template == nullptr)
    {
        delete this->_insertion_template;
        delete this->_obtaining_template;
        delete this->_disposal_template;
        throw std::bad_alloc();
    }
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    red_black_tree<tkey, tvalue> const &other):
        binary_search_tree<tkey, tvalue>(
                new (std::nothrow) red_black_tree<tkey, tvalue>::insertion_template_method(this, other._insertion_template->_insertion_strategy),
                new (std::nothrow) typename binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
                new (std::nothrow) red_black_tree<tkey, tvalue>::disposal_template_method(this, other._disposal_template->_disposal_strategy),
                other._keys_comparer, other._allocator, other._logger)
{

    try
    {
        if (this->_insertion_template == nullptr || this->_obtaining_template == nullptr || this->_disposal_template == nullptr)
        {
            throw std::bad_alloc();
        }
        this->_root = this->copy(reinterpret_cast<node*>(other._root));
    }
    catch (const std::bad_alloc &ex)
    {
        this->clear(reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**>(&this->_root));
        delete this->_insertion_template;
        delete this->_obtaining_template;
        delete this->_disposal_template;
        throw;
    }
}

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
        red_black_tree<tkey, tvalue> &&other) noexcept:
        binary_search_tree<tkey, tvalue>(std::move(other))
{

}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(other);
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(std::move(other));
    }
    return *this;
}


template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::~red_black_tree() noexcept
{

}

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::node_color red_black_tree<tkey, tvalue>::get_color(
        typename binary_search_tree<tkey, tvalue>::node* node) noexcept
{
    auto rbt_node = dynamic_cast<red_black_tree<tkey, tvalue>::node*>(node);
    return rbt_node ? rbt_node->color : red_black_tree<tkey, tvalue>::node_color::BLACK;
}

template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::set_color(
        typename binary_search_tree<tkey, tvalue>::node* node, node_color color) noexcept
{
    dynamic_cast<red_black_tree<tkey, tvalue>::node*>(node)->color = color;
}

template<
        typename tkey,
        typename tvalue>
size_t red_black_tree<tkey, tvalue>::get_node_size() const noexcept
{
    return sizeof(red_black_tree<tkey, tvalue>::node);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *red_black_tree<tkey, tvalue>::create_iterator_data() const
{
    return new iterator_data;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *red_black_tree<tkey, tvalue>::create_iterator_data(unsigned int depth,
                                                                                                             typename binary_search_tree<tkey,tvalue>::node *&src_node) const
{
    return new iterator_data(depth, reinterpret_cast<red_black_tree<tkey, tvalue>::node**>(&src_node));
}


template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::inject_additional_data(
        typename binary_search_tree<tkey, tvalue>::iterator_data* destination,
        typename binary_search_tree<tkey, tvalue>::node const *source) const
{
    auto* rbt_destination = dynamic_cast<red_black_tree<tkey, tvalue>::iterator_data*>(destination);
    auto* rbt_source = dynamic_cast<red_black_tree<tkey, tvalue>::node const*>(source);

    rbt_destination->_color = rbt_source->color;
}

template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::inject_additional_data(
        typename binary_search_tree<tkey, tvalue>::node* destination,
        typename binary_search_tree<tkey, tvalue>::node const *source) const
{
    auto* rbt_destination = dynamic_cast<red_black_tree<tkey, tvalue>::node*>(destination);
    auto* rbt_source = dynamic_cast<red_black_tree<tkey, tvalue>::node const*>(source);

    rbt_destination->color = rbt_source->color;
}

template <
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::insertion_template_method::balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path, typename binary_search_tree<tkey, tvalue>::node* node_to_dispose)
{

    while (path.size() >= 3)
    {
        typename binary_search_tree<tkey, tvalue>::node **current = path.top();
        path.pop();
        typename binary_search_tree<tkey, tvalue>::node **parent = path.top();
        path.pop();
        typename binary_search_tree<tkey, tvalue>::node **grand_dad = path.top();
        typename binary_search_tree<tkey, tvalue>::node **uncle = (*grand_dad)->left_subtree == *parent
                ? &((*grand_dad)->right_subtree)
                : &((*grand_dad)->left_subtree);

        if (get_color(*current) == node_color::BLACK || get_color(*parent) == node_color::BLACK)
        {
            break;
        }
        if (get_color(*uncle) == node_color::RED)
        {
            set_color(*grand_dad,  node_color::RED);
            set_color(*parent,  node_color::BLACK);
            set_color(*uncle,  node_color::BLACK);
        }
        else
        {
            set_color(*grand_dad, node_color::RED);

            if ((*grand_dad)->left_subtree == *parent)
            {
                if ((*parent)->left_subtree == *current)
                {
                    set_color(*parent, node_color::BLACK);
                    dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_right_rotation(*grand_dad);
                }
                else
                {
                    set_color(*current, node_color::BLACK);
                    dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*grand_dad);
                }
            }
            else
            {
                if ((*parent)->right_subtree == *current)
                {
                    set_color(*parent, node_color::BLACK);
                    dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_left_rotation(*grand_dad);
                }
                else
                {
                    set_color(*current, node_color::BLACK);
                    dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*grand_dad);
                }
            }

        }
    }
    if (path.size() == 1)
    {
        auto *node = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*path.top());
        node->color = node_color::BLACK;
    }
}

template <
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::disposal_template_method::balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
                                                                     typename binary_search_tree<tkey, tvalue>::node* node_to_dispose)
{
    if (get_color(node_to_dispose) == node_color::RED)
    {
        return;
    }

    if (node_to_dispose->left_subtree || node_to_dispose->right_subtree)
    {
        dynamic_cast<red_black_tree<tkey, tvalue>::node*>(*(path.top()))->color = node_color::BLACK;
        return;
    }

    bool black_height_failure = true;

    while (path.size() >= 2 && black_height_failure)
    {
        black_height_failure = false;

        typename binary_search_tree<tkey, tvalue>::node** current_node = path.top();
        path.pop();
        typename binary_search_tree<tkey, tvalue>::node** parent = path.top();

        bool is_right_subtree = (*parent)->right_subtree == *current_node;

        typename binary_search_tree<tkey, tvalue>::node** brother = is_right_subtree
                                                                    ? &(*parent)->left_subtree
                                                                    : &(*parent)->right_subtree;

        typename binary_search_tree<tkey, tvalue>::node** nephew = is_right_subtree
                                                                   ? &(*brother)->right_subtree
                                                                   : &(*brother)->left_subtree;

        typename binary_search_tree<tkey, tvalue>::node** second_nephew = is_right_subtree
                                                                            ? &(*brother)->left_subtree
                                                                            : &(*brother)->right_subtree;

        if (get_color(*parent) == node_color::RED) //Case 1: Red parent
        {
            if (get_color(*brother) == node_color::BLACK)
            {
                if (get_color(*nephew) == node_color::BLACK && get_color(*second_nephew) == node_color::BLACK)
                {
                    set_color(*parent, node_color::BLACK);
                    set_color(*brother, node_color::RED);
                }
                if (get_color(*second_nephew) == node_color::RED)
                {
                    set_color(*brother, node_color::RED);
                    set_color(*parent, node_color::BLACK);
                    set_color(*second_nephew, node_color::BLACK);

                    if (is_right_subtree)
                    {
                        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_right_rotation(*parent);
                    }
                    else
                    {
                        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_left_rotation(*parent);
                    }
                }
                else
                {
                    set_color(*parent, node_color::BLACK);
                    if (is_right_subtree)
                    {
                        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*parent);
                    }
                    else
                    {
                        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*parent);
                    }
                }
            }
        }
        else
        {
            if (get_color(*brother) == node_color::RED) //Red brother
            {
                typename binary_search_tree<tkey, tvalue>::node** grandnephew = is_right_subtree
                                                                                ? &(*nephew)->right_subtree
                                                                                : &(*nephew)->left_subtree;

                typename binary_search_tree<tkey, tvalue>::node** second_grandnephew = is_right_subtree
                                                                                         ? &(*nephew)->left_subtree
                                                                                         : &(*nephew)->right_subtree;

                if (get_color(*grandnephew) == node_color::BLACK && get_color(*second_nephew) == node_color::BLACK)
                {
                    set_color(*brother, node_color::BLACK);
                    set_color(*nephew, node_color::RED);
                    if (is_right_subtree)
                    {
                        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_right_rotation(*parent);
                    }
                    else
                    {
                        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_left_rotation(*parent);
                    }
                }
                else
                {
                    if (get_color(*second_grandnephew) == node_color::RED)
                    {
                        set_color(*second_grandnephew, node_color::BLACK);
                        if (is_right_subtree)
                        {
                            dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*parent);
                        }
                        else
                        {
                            dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*parent);
                        }
                    }
                    else
                    {
                        if (is_right_subtree)
                        {
                            dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*parent);
                        }
                        else
                        {
                            dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*parent);
                        }
                    }
                }

            }
            else
            {
                if (get_color(*nephew) == node_color::BLACK && get_color(*second_nephew) == node_color::BLACK)
                {
                    set_color(*brother, node_color::RED);
                    black_height_failure = true;
                }
                else
                {
                    if (get_color(*nephew) == node_color::RED)
                    {
                        set_color(*nephew, node_color::BLACK);
                        if (is_right_subtree)
                        {
                            dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*parent);
                        }
                        else
                        {
                            dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*parent);
                        }
                    }
                    else
                    {
                        set_color(*second_nephew, node_color::BLACK);
                        if (is_right_subtree)
                        {
                            dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_right_rotation(*parent);
                        }
                        else
                        {
                            dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_left_rotation(*parent);
                        }
                    }
                }
            }
        }
    }
}
#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
