#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H

#include <binary_search_tree.h>

template<
        typename tkey,
        typename tvalue>
class AVL_tree final:
        public binary_search_tree<tkey, tvalue>
{

private:

    struct node final :
            binary_search_tree<tkey, tvalue>::node
    {

    public:

        size_t node_subtree_height;

    public:

        explicit node(
                tkey const &key,
                tvalue const &value) :
                binary_search_tree<tkey, tvalue>::node(key, value),
                node_subtree_height(1)
        {

        }

        explicit node(
                tkey const &key,
                tvalue &&value) :
                binary_search_tree<tkey, tvalue>::node(key, std::move(value)),
                node_subtree_height(1)
        {

        }

    };

public:

    struct iterator_data final :
            public binary_search_tree<tkey, tvalue>::iterator_data
    {
        friend void AVL_tree<tkey, tvalue>::inject_additional_data(
                typename binary_search_tree<tkey, tvalue>::iterator_data *,
                typename binary_search_tree<tkey, tvalue>::node const *) const;
    private:

        size_t _subtree_height;

    public:
        size_t get_subtree_height() const;

    public:

        iterator_data();

        explicit iterator_data(
                unsigned int depth,
                tkey const &key,
                tvalue const &value,
                size_t subtree_height);

        explicit iterator_data(unsigned int depth, node** source_node);

        iterator_data(iterator_data const &other);

        iterator_data(iterator_data &&other);

        iterator_data &operator=(iterator_data const &other);

        iterator_data &operator=(iterator_data &&other);

        virtual ~iterator_data() noexcept;

    };

private:
    void balace();

private:

    void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path);

    class insertion_template_method final :
            public binary_search_tree<tkey, tvalue>::insertion_template_method
    {

    public:

        explicit insertion_template_method(
                AVL_tree<tkey, tvalue>* tree,
                typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

    private:

        void balance(
                std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path) override;

    };

    class disposal_template_method final :
            public binary_search_tree<tkey, tvalue>::disposal_template_method
    {

    public:

        explicit disposal_template_method(
                AVL_tree<tkey, tvalue>* tree,
                typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    private:

        void balance(
                std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path) override;

    };

public:

    explicit AVL_tree(
            std::function<int(tkey const &, tkey const &)> comparer = search_tree<tkey, tvalue>::default_keys_comparer(),
            allocator *allocator = nullptr,
            logger *logger = nullptr,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

    ~AVL_tree() noexcept final;

    AVL_tree(
            AVL_tree<tkey, tvalue> const &other);

    AVL_tree(
            AVL_tree<tkey, tvalue> &&other) noexcept;

    AVL_tree<tkey, tvalue> &operator=(
            AVL_tree<tkey, tvalue> const &other);

    AVL_tree<tkey, tvalue> &operator=(
            AVL_tree<tkey, tvalue> &&other) noexcept;

private:

    size_t get_node_size() const noexcept override
    {
        return sizeof(typename AVL_tree<tkey, tvalue>::node);
    }

    void call_node_constructor(
            typename binary_search_tree<tkey,tvalue>::node *raw_space,
            tkey const &key,
            tvalue const &value) override
    {
        allocator::construct(reinterpret_cast<typename AVL_tree<tkey, tvalue>::node*>(raw_space), key, value);
    }

    void call_node_constructor(
            typename binary_search_tree<tkey,tvalue>::node *raw_space,
            tkey const &key,
            tvalue &&value) override
    {
        allocator::construct(reinterpret_cast<typename AVL_tree<tkey, tvalue>::node*>(raw_space), key, std::move(value));
    }

    void inject_additional_data(typename binary_search_tree<tkey, tvalue>::iterator_data *destination,
                                typename binary_search_tree<tkey, tvalue>::node const *source) const override;

    void inject_additional_data(typename binary_search_tree<tkey, tvalue>::node *destination,
                                typename binary_search_tree<tkey, tvalue>::node const *source) const override;

    typename binary_search_tree<tkey, tvalue>::iterator_data *create_iterator_data() const override;

    typename binary_search_tree<tkey, tvalue>::iterator_data *create_iterator_data(unsigned int depth, typename binary_search_tree<tkey,tvalue>::node *&src_node) const override;

    inline size_t get_subtree_height(typename binary_search_tree<tkey, tvalue>::node *node) const noexcept;

    inline int get_balance_factor(typename binary_search_tree<tkey, tvalue>::node *node) const noexcept;

    void update_node_data(typename  binary_search_tree<tkey, tvalue>::node *node) const noexcept override;

};

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data():
        binary_search_tree<tkey, tvalue>::iterator_data(),
        _subtree_height(0)
{

}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
        unsigned int depth,
        tkey const &key,
        tvalue const &value,
size_t subtree_height):
        binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
        _subtree_height(subtree_height)
{

}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
        unsigned int depth, node** source_node):
        binary_search_tree<tkey, tvalue>::iterator_data(depth, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**> (source_node))
{
            if (*source_node != nullptr)
            {
                _subtree_height = (*source_node)->node_subtree_height;
            }
}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(iterator_data const &other):
        binary_search_tree<tkey, tvalue>::iterator_data(other),
        _subtree_height(other._subtree_height)
{

}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(iterator_data &&other):
        binary_search_tree<tkey, tvalue>::iterator_data(std::move(other)),
        _subtree_height(std::move(other._subtree_height))
{

}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data &AVL_tree<tkey, tvalue>::iterator_data::operator=(AVL_tree::iterator_data const &other)

{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(other);
        _subtree_height = other._subtree_height;
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data &AVL_tree<tkey, tvalue>::iterator_data::operator=(AVL_tree::iterator_data &&other)

{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(std::move(other));
        _subtree_height = std::move(other._subtree_height);
    }
    return *this;
}


template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::~iterator_data() noexcept
{

}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *AVL_tree<tkey, tvalue>::create_iterator_data() const
{
    return new iterator_data;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *AVL_tree<tkey, tvalue>::create_iterator_data(
        unsigned int depth,
        typename binary_search_tree<tkey, tvalue>::node *&source) const
{
    return new iterator_data(depth, reinterpret_cast<AVL_tree<tkey, tvalue>::node**>(&source));
}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
        AVL_tree<tkey, tvalue> *tree,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
        binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{

}

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::insertion_template_method::balance(
        std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    dynamic_cast<AVL_tree<tkey, tvalue> *>(this->_tree)->balance(path);
}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
        AVL_tree<tkey, tvalue> *tree,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{

}

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::disposal_template_method::balance(
        std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    dynamic_cast<AVL_tree<tkey, tvalue> *>(this->_tree)->balance(path);
}

//endregion


template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
        std::function<int(const tkey &, const tkey &)> comparer,
        allocator *allocator,
        logger *logger,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>(
                new (std::nothrow) AVL_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
                new (std::nothrow) typename binary_search_tree<tkey,tvalue>::obtaining_template_method(this),
                new (std::nothrow) AVL_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
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
AVL_tree<tkey, tvalue>::AVL_tree(
        AVL_tree<tkey, tvalue> const &other):

        binary_search_tree<tkey, tvalue>(
                new (std::nothrow) AVL_tree<tkey, tvalue>::insertion_template_method(this, other._insertion_template->_insertion_strategy),
                new (std::nothrow) typename binary_search_tree<tkey,tvalue>::obtaining_template_method(this),
                new (std::nothrow) AVL_tree<tkey, tvalue>::disposal_template_method(this, other._disposal_template->_disposal_strategy),
                other._keys_comparer, other.get_allocator(), other.get_logger())
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

template <
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(AVL_tree<tkey, tvalue> && other) noexcept:
        binary_search_tree<tkey, tvalue>(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
        AVL_tree<tkey, tvalue> const &other)
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
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept
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
AVL_tree<tkey, tvalue>::~AVL_tree() noexcept
{

}

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::update_node_data(typename binary_search_tree<tkey, tvalue>::node* node) const noexcept
{
    auto *avl_node = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(node);
    size_t left_subtree_height = get_subtree_height(avl_node->left_subtree);
    size_t right_subtree_height = get_subtree_height(avl_node->right_subtree);
    avl_node->node_subtree_height = std::max(left_subtree_height, right_subtree_height) + 1;
}

template<
        typename tkey,
        typename tvalue>
size_t AVL_tree<tkey, tvalue>::get_subtree_height(typename binary_search_tree<tkey, tvalue>::node* node) const noexcept
{
    auto *tree_node = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(node);
    return tree_node ? tree_node->node_subtree_height : 0;
}

template<
        typename tkey,
        typename tvalue>
size_t AVL_tree<tkey, tvalue>::iterator_data::get_subtree_height() const
{
    if (this->_is_state_initialized)
    {
        return _subtree_height;
    }
    throw std::logic_error("Cannot read data from uninitialized iterator");
}

template<
        typename tkey,
        typename tvalue>
int AVL_tree<tkey, tvalue>::get_balance_factor(typename binary_search_tree<tkey, tvalue>::node* node) const noexcept
{
    return static_cast<long>(get_subtree_height(node->right_subtree)) - static_cast<long>(get_subtree_height(node->left_subtree));
}


template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    while (!path.empty())
    {
        typename binary_search_tree<tkey, tvalue>::node** current_node = path.top();
        path.pop();

        if (*current_node == nullptr)
        {
            continue;
        }
        update_node_data(*current_node);

        if (get_balance_factor(*current_node) == 2)
        {
            auto *right_subtree = (*current_node)->right_subtree;
            if (get_balance_factor(right_subtree) < 0)
            {
                this->big_left_rotation(*current_node);
            }
            else
            {
                this->small_left_rotation(*current_node);
            }
        }
        else if (get_balance_factor(*current_node) == -2)
        {
            auto *left_subtree = (*current_node)->left_subtree;
            if (get_balance_factor(left_subtree) > 0)
            {
                this->big_right_rotation(*current_node);
            }
            else
            {
                this->small_right_rotation(*current_node);
            }
        }

    }

}

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::inject_additional_data(
        typename binary_search_tree<tkey, tvalue>::iterator_data* destination,
        typename binary_search_tree<tkey, tvalue>::node const *source) const
{
    auto *avl_destination = dynamic_cast<AVL_tree<tkey, tvalue>::iterator_data*>(destination);
    auto *avl_source = dynamic_cast<AVL_tree<tkey, tvalue>::node const*>(source);
    avl_destination->_subtree_height = avl_source->node_subtree_height;
}


template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::inject_additional_data(
        typename binary_search_tree<tkey,tvalue>::node *destination,
        typename binary_search_tree<tkey,tvalue>::node const *source) const
{
    auto *avl_destination = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(destination);
    auto *avl_source = dynamic_cast<AVL_tree<tkey, tvalue>::node const*>(source);
    avl_destination->node_subtree_height = avl_source->node_subtree_height;
}





#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H