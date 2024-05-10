#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <optional>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>
#include <string>

template<
    typename tkey,
    typename tvalue>
class binary_search_tree:
    public search_tree<tkey, tvalue>
{

protected:
    
    struct node
    {
    
    public:
        
        tkey key;
        
        tvalue value;
        
        node *left_subtree = nullptr;
        
        node *right_subtree = nullptr;
    
    public:
        
        explicit node(
            tkey const &key,
            tvalue const &value);
        
        explicit node(
            tkey const &key,
            tvalue &&value);

    public:

        virtual ~node() noexcept = default;
    };

private:
    class iterator;
    class const_iterator;

public:
    
    // region iterators definition

//    class prefix_iterator;
//    class prefix_const_iterator;
//    class prefix_reverse_iterator;
//    class prefix_reverse_const_iterator;
//    class infix_iterator;
//    class infix_const_iterator;
//    class infix_reverse_iterator;
//    class infix_reverse_const_iterator;
//    class postfix_iterator;
//    class postfix_const_iterator;
//    class postfix_reverse_iterator;
//    class postfix_reverse_const_iterator;

    struct iterator_data
    {

        friend class const_iterator;
        friend void binary_search_tree<tkey, tvalue>::inject_additional_data(iterator_data *, const node*) const;
    protected:
        tkey *_key;
        tvalue *_value;
        unsigned int _depth;
        bool _is_state_initialized;

        friend class prefix_iterator;
        friend class prefix_const_iterator;
        friend class prefix_reverse_iterator;
        friend class prefix_reverse_const_iterator;
        friend class infix_iterator;
        friend class infix_const_iterator;
        friend class infix_reverse_iterator;
        friend class infix_reverse_const_iterator;
        friend class postfix_iterator;
        friend class postfix_const_iterator;
        friend class postfix_reverse_iterator;
        friend class postfix_reverse_const_iterator;

    public:

        inline tkey const &get_key() const;

        inline tvalue const &get_value() const;

        inline unsigned int get_depth() const;

        inline bool is_state_initialized() const noexcept;

    public:

        iterator_data();

        iterator_data(unsigned int depth, tkey const &key, tvalue const &value);

        iterator_data(unsigned int depth, node** source_node);

        iterator_data(iterator_data const &other);

        iterator_data(iterator_data &&other);

        iterator_data &operator=(iterator_data const &other);

        iterator_data &operator=(iterator_data  &&other);

        virtual ~iterator_data() noexcept;

    };

private:
    class const_iterator
    {
        friend class binary_search_tree<tkey, tvalue>;
    protected:
        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> const *_holder;

    public:
        explicit const_iterator(binary_search_tree<tkey, tvalue> const *holder); //constructor

        const_iterator(binary_search_tree<tkey, tvalue> const *holder, //constructor
                       std::stack<node *> const &state);

        const_iterator(const_iterator const &other); //rule of five

        const_iterator(const_iterator &&other) noexcept ;

        const_iterator &operator=(const_iterator const &other);

        const_iterator &operator=(const_iterator &&other);

        virtual ~const_iterator() noexcept;

    public:

        bool operator==(
                const_iterator const &other) const noexcept; // ==

        bool operator!=(
                const_iterator const &other) const noexcept; // !=

        const_iterator &operator++(); // ++a

        const_iterator const operator++(
                int not_used); //a++

        iterator_data *operator*() const; //*a

    protected:
        void assign_data();
        virtual void next_node()
        {

        };
    };

private:
    class iterator:
            public const_iterator
    {
    public:

        iterator(binary_search_tree<tkey, tvalue> const *holder);

        iterator(binary_search_tree<tkey, tvalue> const *holder,
                       std::stack<node *> const &state);

        iterator(iterator const &other);

        iterator(iterator &&other);

        iterator &operator=(iterator const &other);

        iterator &operator=(iterator &&other);

        virtual ~iterator() noexcept;

    public:
        void set_value(tvalue const &value);
        void set_value(tvalue &&value);
    };

    class prefix_iterator final:
            public iterator
    {
    public:
        
        explicit prefix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit prefix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state);
    public:
        
        prefix_iterator(prefix_iterator const &other);

        prefix_iterator(prefix_iterator && other);

        prefix_iterator &operator=(prefix_iterator const &other);

        prefix_iterator &operator=(prefix_iterator &&other);

        ~prefix_iterator();

    private:
        void next_node() override;
        
    };
    
    class prefix_const_iterator final:
            public const_iterator
    {
    public:
        explicit prefix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit prefix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:

        prefix_const_iterator(prefix_const_iterator const &other);

        prefix_const_iterator(prefix_const_iterator && other);

        prefix_const_iterator &operator=(prefix_const_iterator const &other);

        prefix_const_iterator &operator=(prefix_const_iterator &&other) noexcept;

        ~prefix_const_iterator();

    private:
        void next_node() override;
    };
    
    class prefix_reverse_iterator final:
            public iterator
    {
    public:

        explicit prefix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit prefix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:

        prefix_reverse_iterator(prefix_reverse_iterator const &other);

        prefix_reverse_iterator(prefix_reverse_iterator && other);

        prefix_reverse_iterator &operator=(prefix_reverse_iterator const &other);

        prefix_reverse_iterator &operator=(prefix_reverse_iterator &&other);

        ~prefix_reverse_iterator();

    private:
        void next_node() override;
        
    };
    
    class prefix_const_reverse_iterator final:
            public const_iterator
    {

    public:

    explicit prefix_const_reverse_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root);

    explicit prefix_const_reverse_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
    std::stack<node *> const &state);

    public:

        prefix_const_reverse_iterator(prefix_const_reverse_iterator const &other);

        prefix_const_reverse_iterator(prefix_const_reverse_iterator && other);

        prefix_const_reverse_iterator &operator=(prefix_const_reverse_iterator const &other);

        prefix_const_reverse_iterator &operator=(prefix_const_reverse_iterator &&other);

    ~prefix_const_reverse_iterator();

    private:
    void next_node() override;
        
    };
    
    class infix_iterator final:
        public iterator
    {

        public:

        explicit infix_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit infix_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

        public:

        infix_iterator(infix_iterator const &other);

        infix_iterator(infix_iterator && other);

        infix_iterator &operator=(infix_iterator const &other);

        infix_iterator &operator=(infix_iterator &&other);

        ~infix_iterator();

        private:
        void next_node() override;

    };

    class infix_const_iterator final:
            public const_iterator
    {

    public:

        explicit infix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit infix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:

        infix_const_iterator(infix_const_iterator const &other);

        infix_const_iterator(infix_const_iterator && other);

        infix_const_iterator &operator=(infix_const_iterator const &other);

        infix_const_iterator &operator=(infix_const_iterator &&other) noexcept ;

        ~infix_const_iterator();

    private:
        void next_node() override;

    };

    class infix_reverse_iterator final:
            public iterator
    {
    public:
        explicit infix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit infix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:
        infix_reverse_iterator(infix_reverse_iterator const &other);

        infix_reverse_iterator(infix_reverse_iterator && other);

        infix_reverse_iterator &operator=(infix_reverse_iterator const &other);

        infix_reverse_iterator &operator=(infix_reverse_iterator &&other);

        ~infix_reverse_iterator();

    private:
        void next_node() override;
    };
    
    class infix_const_reverse_iterator final:
            public const_iterator
    {

    public:

        explicit infix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit infix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:

        infix_const_reverse_iterator(infix_const_reverse_iterator const &other);

        infix_const_reverse_iterator(infix_const_reverse_iterator && other);

        infix_const_reverse_iterator &operator=(infix_const_reverse_iterator const &other);

        infix_const_reverse_iterator &operator=(infix_const_reverse_iterator &&other);

        ~infix_const_reverse_iterator();

    private:
        void next_node() override;

    };

    class postfix_iterator final:
            public iterator
    {
    public:

        explicit postfix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit postfix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:

        postfix_iterator(postfix_iterator const &other);

        postfix_iterator(postfix_iterator && other);

        postfix_iterator &operator=(postfix_iterator const &other);

        postfix_iterator &operator=(postfix_iterator &&other);

        ~postfix_iterator();

    private:
        void next_node() override;
    };

    
    class postfix_const_iterator final:
            public const_iterator
    {
    public:

        explicit postfix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit postfix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:

        postfix_const_iterator(postfix_const_iterator const &other);

        postfix_const_iterator(postfix_const_iterator && other);

        postfix_const_iterator &operator=(postfix_const_iterator const &other);

        postfix_const_iterator &operator=(postfix_const_iterator &&other);

        ~postfix_const_iterator();

    private:
        void next_node() override;
    };
    
    class postfix_reverse_iterator final:
            public iterator
    {
    public:

        explicit postfix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit postfix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:

        postfix_reverse_iterator(postfix_reverse_iterator const &other);

        postfix_reverse_iterator(postfix_reverse_iterator && other);

        postfix_reverse_iterator &operator=(postfix_reverse_iterator const &other);

        postfix_reverse_iterator &operator=(postfix_reverse_iterator &&other);

        ~postfix_reverse_iterator();

    private:
        void next_node() override;
    };
    
    class postfix_const_reverse_iterator final:
            public const_iterator
    {

    public:

        explicit postfix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit postfix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node *> const &state);

    public:

        postfix_const_reverse_iterator(postfix_const_reverse_iterator const &other);

        postfix_const_reverse_iterator(postfix_const_reverse_iterator && other);

        postfix_const_reverse_iterator &operator=(postfix_const_reverse_iterator const &other);

        postfix_const_reverse_iterator &operator=(postfix_const_reverse_iterator &&other);

        ~postfix_const_reverse_iterator();

    private:
        void next_node() override;
        
    };
    
    // endregion iterators definition

public:
    
    // region target operations strategies definition
    
    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };
    
    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };
    
    // endregion target operations strategies definition
    
    // region target operations associated exception types
    
    class insertion_of_existent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit insertion_of_existent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    class obtaining_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
        
    public:
        
        explicit obtaining_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
        
    };
    
    class disposal_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit disposal_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    // endregion target operations associated exception types
    
    // region template methods definition

protected:
    
    class template_method_basics:
        public logger_guardant
    {
    
    protected:
    
        binary_search_tree<tkey, tvalue> *_tree;
        
    public:
    
        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);
        
    protected:
        
        std::stack<node **> find_path(
            tkey const &key) const;

        virtual void balance(
            std::stack<node **> &path,
            typename binary_search_tree<tkey, tvalue>::node* node_to_dispose = nullptr);

    protected:
    
        [[nodiscard]] logger *get_logger() const noexcept final;
        
    };

    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    public:

        binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strategy;
    
    public:
        
        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
        
    public:
        
        void insert(
            tkey const &key,
            tvalue const &value);
        
        void insert(
            tkey const &key,
            tvalue &&value);
    
        void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
//
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };
    
    class obtaining_template_method:
        public template_method_basics
    {
    
    public:
        
        explicit obtaining_template_method(
            binary_search_tree<tkey, tvalue> *tree);
    
    public:
        
        tvalue const &obtain(
            tkey const &key);

        std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool lower_bound_inclusive,
            bool upper_bound_inclusive)
        {
            std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;
            std::stack<node *> path;
            node *current = static_cast<node*>(this->_tree->_root);
            int comparison_result;
            while (true)
            {
                path.push(current);

                if (current == nullptr || (comparison_result = this->_tree->_keys_comparer(lower_bound, current->key)) == 0)
                {
                    break;
                }

                current = comparison_result < 0 ? current->left_subtree : current->right_subtree;
            }
            if (path.top() == nullptr)
            {
                path.pop();

                if (!path.empty() && (this->_tree->_keys_comparer(lower_bound, path.top()->key)) == 1)
                {
                    do
                    {
                        current = path.top();
                        path.pop();
                    }
                    while (!path.empty() && path.top()->right_subtree == current);
                }
            }
            if (path.empty())
            {
                return range;
            }

            if (lower_bound_inclusive)
            {
                range.push_back({path.top()->key, path.top()->value});
            }

            auto it = binary_search_tree<tkey, tvalue>::infix_const_iterator(this->_tree, path);
            ++it;
            while ((it != this->_tree->end_infix()) && (this->_tree->_keys_comparer((*it)->get_key(), upper_bound) < (upper_bound_inclusive
                ? 1
                : 0)))
            {
                range.push_back({ (*it)->get_key(), (*it)->get_value()});
                ++it;
            }

            return range;
        }
        
    };
    
    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {

    public:

        binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy _disposal_strategy;
    
    public:
        
        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
    public:
        
        virtual tvalue dispose(
            tkey const &key);
        
        void set_disposal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };
    
    // endregion template methods definition

protected:
    
    insertion_template_method *_insertion_template = nullptr;
    
    obtaining_template_method *_obtaining_template = nullptr;
    
    disposal_template_method *_disposal_template = nullptr;

protected:
    
    explicit binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
        std::function<int(tkey const &, tkey const &)>,
        allocator *allocator,
        logger *logger);

public:
    
    explicit binary_search_tree(
        std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    ~binary_search_tree() override;

protected:

    void clear(
        node **subtree_root)
    {
        if (*subtree_root == nullptr)
        {
            return;
        }
        clear(&(*subtree_root)->left_subtree);
        clear(&(*subtree_root)->right_subtree);
        (*subtree_root)->~node();
        this->deallocate_with_guard(*subtree_root);

        *subtree_root = nullptr;
    }

    node *copy(
        node const *subtree_root)
    {
        if (subtree_root == nullptr)
        {
            return nullptr;
        }

        node *subtree_root_copied = nullptr;

        try
        {
            subtree_root_copied = reinterpret_cast<node *>(this->allocate_with_guard(this->get_node_size(), 1));
            call_node_constructor(subtree_root_copied, subtree_root->key, subtree_root->value);
            inject_additional_data(subtree_root_copied, subtree_root);
            subtree_root_copied->left_subtree = copy(subtree_root->left_subtree);
            subtree_root_copied->right_subtree = copy(subtree_root->right_subtree);
        }
        catch (std::bad_alloc &ex)
        {
            clear(&subtree_root_copied);
            throw;
        }
        return subtree_root_copied;
    }

    virtual size_t get_node_size() const noexcept
    {
        return sizeof(typename binary_search_tree<tkey, tvalue>::node);
    }

    virtual inline void update_node_data(node *node) const noexcept;

    virtual void call_node_constructor(
        node *raw_space,
        tkey const &key,
        tvalue const &value)
    {
        allocator::construct(raw_space, key, value);
    }

    virtual void call_node_constructor(
        node *raw_space,
        tkey const &key,
        tvalue &&value)
    {
        allocator::construct(raw_space, key, std::move(value));
    }

    virtual void inject_additional_data(
        node *destination,
        node const *source) const
    {

    }

    virtual void inject_additional_data(
            iterator_data *destination,
            node const *source) const
    {

    }

    virtual iterator_data *create_iterator_data() const
    {
        return new iterator_data;
    }

    virtual iterator_data *create_iterator_data(unsigned int depth, node *&src_node) const
    {
        return new iterator_data(depth, &src_node);
    }

public:
    
    void insert(
        tkey const &key,
        tvalue const &value) final;
    
    void insert(
        tkey const &key,
        tvalue &&value) final;
    
    tvalue const &obtain(
        tkey const &key) final;
    
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) final;
    
    tvalue dispose(
        tkey const &key) final;
    
public:
    
    void set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    void set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:
    
    // region iterators requests definition
    
    prefix_iterator begin_prefix() const noexcept;
    
    prefix_iterator end_prefix() const noexcept;
    
    prefix_const_iterator cbegin_prefix() const noexcept;
    
    prefix_const_iterator cend_prefix() const noexcept;
    
    prefix_reverse_iterator rbegin_prefix() const noexcept;
    
    prefix_reverse_iterator rend_prefix() const noexcept;
    
    prefix_const_reverse_iterator crbegin_prefix() const noexcept;
    
    prefix_const_reverse_iterator crend_prefix() const noexcept;
    
    infix_iterator begin_infix() const noexcept;
    
    infix_iterator end_infix() const noexcept;
    
    infix_const_iterator cbegin_infix() const noexcept;
    
    infix_const_iterator cend_infix() const noexcept;
    
    infix_reverse_iterator rbegin_infix() const noexcept;
    
    infix_reverse_iterator rend_infix() const noexcept;
    
    infix_const_reverse_iterator crbegin_infix() const noexcept;
    
    infix_const_reverse_iterator crend_infix() const noexcept;
    
    postfix_iterator begin_postfix() const noexcept;
    
    postfix_iterator end_postfix() const noexcept;
    
    postfix_const_iterator cbegin_postfix() const noexcept;
    
    postfix_const_iterator cend_postfix() const noexcept;
    
    postfix_reverse_iterator rbegin_postfix() const noexcept;
    
    postfix_reverse_iterator rend_postfix() const noexcept;
    
    postfix_const_reverse_iterator crbegin_postfix() const noexcept;
    
    postfix_const_reverse_iterator crend_postfix() const noexcept;
    
    // endregion iterators requests definition

protected:
    
    // region subtree rotations definition
    
    void small_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void small_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void double_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    void double_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    // endregion subtree rotations definition
private:
    std::mutex _mutex;
};

// region binary_search_tree<tkey, tvalue>::node methods implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value):
        key(key),
        value(value),
        left_subtree(nullptr),
        right_subtree(nullptr)
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value):
        key(key),
        value(std::move(value)),
        left_subtree(nullptr),
        right_subtree(nullptr)
{

}

// endregion binary_search_tree<tkey, tvalue>::node methods implementation

// region iterators implementation

// region iterator data implementation



// endregion iterator data implementation

// region prefix_iterator implementation


//        inline tkey const &get_key() const;
//
//        inline tvalue const &get_value() const noexcept;
//
//        inline unsigned int get_depth() const;
//
//        inline bool is_state_initialized() const noexcept;


template<
        typename tkey,
        typename tvalue>
inline tkey const &binary_search_tree<tkey, tvalue>::iterator_data::get_key() const
{
    if (_is_state_initialized)
    {
        return *_key;
    }
    throw std::logic_error("Cannot get uninitialized iterator data key");
}

template<
        typename tkey,
        typename tvalue>
inline tvalue const &binary_search_tree<tkey, tvalue>::iterator_data::get_value() const
{
    if (_is_state_initialized)
    {
        return *_value;
    }
    throw std::logic_error("Cannot get uninitialized iterator data value");
}

template<
        typename tkey,
        typename tvalue>
inline unsigned int binary_search_tree<tkey, tvalue>::iterator_data::get_depth() const
{
    if (_is_state_initialized)
    {
        return _depth;
    }
    throw std::logic_error("Cannot get uninitialized iterator data value");
}

template<
        typename tkey,
        typename tvalue>
inline bool binary_search_tree<tkey, tvalue>::iterator_data::is_state_initialized() const noexcept
{
    return _is_state_initialized;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data():
    _is_state_initialized(false)
{
    try
    {
        _key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
        _value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
    }

    catch (const std::bad_alloc &)
    {
        ::operator delete(_key);
        ::operator delete(_value);
        throw;
    }
}


//
//iterator_data &operator=(iterator_data const &other);
//
//iterator_data &operator=(iterator_data  &&other);
//
//virtual ~iterator_data() noexcept;

//        iterator_data(
//            unsigned int depth,
//            tkey const &key,
//            tvalue const &value):
//            depth(depth),
//            _key(reinterpret_cast<tkey *>(::operator new(sizeof(tkey)))),
//            _value(reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue)))),
//            _is_state_initialized(true)
//        {
//            allocator::construct(_key, key);
//            allocator::construct(_value, value);
//        }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::
                                iterator_data(unsigned int depth,
                                tkey const &key,
                                tvalue const &value):
                                iterator_data()
{
    _depth = depth;
    _is_state_initialized = true;
        try
        {
            _key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
            _value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
        }
        catch (std::bad_alloc &)
        {
            ::operator delete(_key);
            ::operator delete(_value);
            throw;
        }
    allocator::construct(_key, key);
    allocator::construct(_value, value);

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int depth, node** source_node):
                                    _depth(depth),
                                    _key(nullptr),
                                    _value(nullptr),
                                    _is_state_initialized(false)
{
    if (*source_node)
    {


        try
        {
            _key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
            _value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
        }
        catch (std::bad_alloc &)
        {
            ::operator delete(_key);
            ::operator delete(_value);
            throw;
        }
        allocator::construct(_key, (*source_node)->key);
        allocator::construct(_value, (*source_node)->value);
        _is_state_initialized = true;
    }
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(iterator_data const &other):
        _depth(other._depth),
        _is_state_initialized(other._is_state_initialized)
{
    try
    {
        _key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
        _value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
    }
    catch (std::bad_alloc &)
    {
        ::operator delete(_key);
        ::operator delete(_value);
        throw;
    }
    allocator::construct(_key, *other._key);
    allocator::construct(_value, *other._value);
}


template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(iterator_data &&other):
      _depth(std::move(other._depth)),
      _key(std::move(other._key)),
      _value(std::move(other._value)),
      _is_state_initialized(std::move(other._is_state_initialized))
{

}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data &binary_search_tree<tkey, tvalue>::iterator_data::operator=(iterator_data const &other)

{
    if (this != &other)
    {
        _depth = other._depth;
        _is_state_initialized = other._is_state_initialized;
        try
        {
            _key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
            _value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
        }
        catch (std::bad_alloc &)
        {
            ::operator delete(_key);
            ::operator delete(_value);
            throw;
        }
        allocator::construct(_key, *other._key);
        allocator::construct(_value, *other._value);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data &binary_search_tree<tkey, tvalue>::iterator_data::operator=(iterator_data &&other)

{
    if (this != &other)
    {
        _depth(std::move(other._depth));
        _key(std::move(other._key));
        _value(std::move(other._value));
        _is_state_initialized(std::move(other._is_state_initialized));
        other._key = nullptr;
        other._value = nullptr;
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::~iterator_data() noexcept
{
    if (_is_state_initialized)
    {
        allocator::destruct(_key);
        allocator::destruct(_value);
    }
    ::operator delete (_key);
    ::operator delete (_value);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::const_iterator(binary_search_tree<tkey, tvalue> const *holder):
        _holder(holder)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::const_iterator(binary_search_tree<tkey, tvalue> const  *holder, std::stack<node*> const &state):
        _holder(holder),
        _state(state)
{
    if (_state.empty())
    {
        _data = _holder->create_iterator_data();
    }
    else
    {
        _data = _holder->create_iterator_data(_state.size() - 1, _state.top());
    }
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::const_iterator(const_iterator const &other):
        _holder(other._holder),
        _state(other._state)
{

    _data = _state.empty() ? _holder->create_iterator_data() : _holder->create_iterator_data(_state.size() - 1, _state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::const_iterator(const_iterator &&other) noexcept:
        _holder(std::move(other._holder)),
        _state(std::move(other._state)),
        _data(std::move(other._data))
{
    other._holder = nullptr;
    other._data = nullptr;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::const_iterator &binary_search_tree<tkey, tvalue>::const_iterator::operator=(const_iterator const &other)

{
    if (this != &other)
    {
        _holder = other._holder;
        _state = other._state;
        _data = _state.empty() ? _holder->create_iterator_data() : _holder->create_iterator_data(_state.size() - 1, _state.top());
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::const_iterator &binary_search_tree<tkey, tvalue>::const_iterator::operator=(const_iterator &&other)

{
    if (this != &other)
    {
        _holder = std::move(other._holder);
        _state = std::move(other._state);
        _data = std::move(other._data);
        other._holder = nullptr;
        other._state = nullptr;
        other._data = nullptr;
    }
    return *this;
}

template <
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::~const_iterator() noexcept
{
    delete _data;
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::const_iterator::operator==(typename binary_search_tree<tkey,tvalue>::const_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}


template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::const_iterator::operator!=(typename binary_search_tree<tkey,tvalue>::const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::const_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("Cannot get data from empty iterator");
    }
    return _data;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::const_iterator &binary_search_tree<tkey, tvalue>::const_iterator::operator++()
{
    next_node();

    if (!_state.empty())
    {
        this->assign_data();
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::const_iterator const binary_search_tree<tkey, tvalue>::const_iterator::operator++(int not_used)
{
    auto iterator = *this;
    ++*this;
    return iterator; //TODO is it correct?
}


template <
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::const_iterator::assign_data()
{
    if (_data->is_state_initialized())
    {
        *(_data->_key) = _state.top()->key;
        *(_data->_value) = _state.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _state.top()->key);
        allocator::construct(_data->_value, _state.top()->value);
        _data->_is_state_initialized = true;
    }

    _data->_depth = _state.size() - 1;
    _holder->inject_additional_data(_data, _state.top());
}

template <
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::iterator(const binary_search_tree<tkey, tvalue>* holder):
        const_iterator(holder)
{
}

template <
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::iterator(binary_search_tree<tkey, tvalue> const  *holder, std::stack<node*> const &state):
        const_iterator(holder, state)
{

}

template <
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::iterator(iterator const &other):
        const_iterator(other)
{

}

template <
        typename tkey, //TODO check if it works or should i add std::move
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::iterator(iterator &&other):
        const_iterator(std::move(other))
{

}

template <
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator &binary_search_tree<tkey, tvalue>::iterator::operator=(iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }
    return *this;
}

template <
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator &binary_search_tree<tkey, tvalue>::iterator::operator=(iterator &&other)
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }
    return *this;
}

template <
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::~iterator() noexcept
{

}

template <
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator::set_value(tvalue const &value)
{
    this->_state.top()->value = value;
}

template <
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator::set_value(tvalue &&value)
{
    this->_state.top()->value = std::move(value);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(const binary_search_tree<tkey, tvalue>* holder,
typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        iterator(holder)
{
    node* node = subtree_root;
    if (node)
    {
        this->_state.push(node);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(const binary_search_tree<tkey, tvalue>* holder,
const std::stack<node*> &state):
        iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(prefix_iterator const &other):
        iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(prefix_iterator &&other):
        iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator=(prefix_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator=(prefix_iterator &&other)
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::~prefix_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_iterator::next_node()
{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }
    if (_state.top()->left_subtree)
    {
        _state.push(_state.top()->left_subtree);
    }
    else if (_state.top()->right_subtree)
    {
        _state.push(_state.top()->right_subtree);
    }
    else
    {
        while (true)
        {
            node* curr_node = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return;
            }
            if (_state.top()->left_subtree == curr_node &&_state.top()->right_subtree)
            {
                _state.push(_state.top()->right_subtree);
                return;
            }
        }
    }

}

// endregion prefix_iterator implementation

// region prefix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder, typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        const_iterator(holder)
{
    node* node = subtree_root;
    if (node)
    {
        this->_state.push(node);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
        binary_search_tree<tkey, tvalue> const  *holder, std::stack<node*> const &state):
        const_iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(prefix_const_iterator const &other):
        const_iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(prefix_const_iterator &&other):
        const_iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator=(prefix_const_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator=(prefix_const_iterator &&other) noexcept
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::~prefix_const_iterator()
{
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_const_iterator::next_node()
{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }
    if (_state.top()->left_subtree)
    {
        _state.push(_state.top()->left_subtree);
    }
    else if (_state.top()->right_subtree)
    {
        _state.push(_state.top()->right_subtree);
    }
    else
    {
        while (true)
        {
            node* curr_node = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return;
            }
            if (_state.top()->left_subtree == curr_node &&_state.top()->right_subtree)
            {
                _state.push(_state.top()->right_subtree);
                return;
            }
        }
    }

}

// endregion prefix_const_iterator implementation

// region prefix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder, typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        iterator(holder)
{
    node* node = subtree_root;
    if (node)
    {
        this->_state.push(node);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const  *holder, std::stack<node*> const &state):
        iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(prefix_reverse_iterator const &other):
        iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(prefix_reverse_iterator &&other):
        iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator=(
        prefix_reverse_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator=(
        prefix_reverse_iterator &&other)
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::~prefix_reverse_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->right_subtree)
    {
        _state.push(_state.top()->right_subtree);
    }
    else if (_state.top()->left_subtree)
    {
        _state.push(_state.top()->left_subtree);
    }
    else
    {
        while (true)
        {
            node* curr_node = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return;
            }
            if (_state.top()->right_subtree == curr_node && _state.top()->left_subtree)
            {
                _state.push(_state.top()->left_subtree);
                return;
            }
        }
    }
}


// endregion prefix_reverse_iterator implementation

// region prefix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder, typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        const_iterator(holder)
{
    node* node = subtree_root;
    if (node)
    {
        this->_state.push(node);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(binary_search_tree<tkey, tvalue> const *holder,
                                                                                               std::stack<node*> const &state):
        const_iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(prefix_const_reverse_iterator const &other):
        const_iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(prefix_const_reverse_iterator &&other):
        const_iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator=(prefix_const_reverse_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator=(prefix_const_reverse_iterator &&other)
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::~prefix_const_reverse_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->right_subtree)
    {
        _state.push(_state.top()->right_subtree);
    }
    else if (_state.top()->left_subtree)
    {
        _state.push(_state.top()->left_subtree);
    }
    else
    {
        while (true)
        {
            node* curr_node = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return;
            }
            if (_state.top()->right_subtree == curr_node && _state.top()->left_subtree)
            {
                _state.push(_state.top()->left_subtree);
                return;
            }
        }
    }
}

// endregion prefix_const_reverse_iterator implementation

// region infix_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(binary_search_tree<tkey, tvalue> const *holder,
                                                                 typename binary_search_tree<tkey, tvalue>::node* subtree_root):
                                                                 iterator(holder)
{
    while (subtree_root)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree;
    }
    node* node = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(binary_search_tree<tkey, tvalue> const *holder,
                                                                 std::stack<node*> const &state):
                                                                 iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(infix_iterator const &other):
        iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(infix_iterator &&other):
        iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator=(infix_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator=(infix_iterator &&other)
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::~infix_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_iterator::next_node()
{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }
    if (_state.top()->right_subtree)
    {
        _state.push(_state.top()->right_subtree);
        while (_state.top()->left_subtree)
        {
            _state.push(_state.top()->left_subtree);
        }
    }
    else
    {
        while (true)
        {
            node* curr_node = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return;
            }
            if (_state.top()->left_subtree == curr_node)
            {
                return;
            }
        }
    }
}

// endregion infix_iterator implementation

// region infix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder, typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        const_iterator(holder)
{
    while (subtree_root)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree;
    }
    node* node = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder, const std::stack<node*> &state):
        const_iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(infix_const_iterator const &other):
        const_iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(infix_const_iterator &&other):
        const_iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator=(infix_const_iterator const &other)

{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator=(infix_const_iterator &&other) noexcept

{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::~infix_const_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_const_iterator::next_node()
{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }
    if (_state.top()->right_subtree)
    {
        _state.push(_state.top()->right_subtree);
        while (_state.top()->left_subtree)
        {
            _state.push(_state.top()->left_subtree);
        }
    }
    else
    {
        while (true)
        {
            node* curr_node = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return;
            }
            if (_state.top()->left_subtree == curr_node)
            {
                return;
            }
        }
    }
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder, typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        iterator(holder)
{
    while (subtree_root)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->right_subtree;
    }
    node* node = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder, const std::stack<node*> &state):
        iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        infix_reverse_iterator const &other):
        iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        infix_reverse_iterator &&other):
        iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator=(infix_reverse_iterator const &other)

{
    if (*this != other)
    {
        iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator=(infix_reverse_iterator &&other)

{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::~infix_reverse_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_reverse_iterator::next_node()

{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }
    if (_state.top()->left_subtree)
    {
        _state.push(_state.top()->left_subtree);
        while (_state.top()->right_subtree)
        {
            _state.push(_state.top()->right_subtree);
        }
    }
    else
    {
        while (true)
        {
            node* curr_node = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return;
            }
            if (_state.top()->right_subtree == curr_node)
            {
                return;
            }
        }
    }
}

// endregion infix_reverse_iterator implementation

// region infix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder, typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        const_iterator(holder)
{
    while (subtree_root)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->right_subtree;
    }
    node* node = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder, const std::stack<node*> &state):
    const_iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        infix_const_reverse_iterator const &other):
        const_iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        infix_const_reverse_iterator &&other):
        const_iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator=(infix_const_reverse_iterator const &other)

{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator=(infix_const_reverse_iterator &&other)

{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::~infix_const_reverse_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::next_node()

{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }
    if (_state.top()->left_subtree)
    {
        _state.push(_state.top()->left_subtree);
        while (_state.top()->right_subtree)
        {
            _state.push(_state.top()->right_subtree);
        }
    }
    else
    {
        while (true)
        {
            node* curr_node = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return;
            }
            if (_state.top()->right_subtree == curr_node)
            {
                return;
            }
        }
    }
}
// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        iterator(holder)
{
    while (subtree_root)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree ? subtree_root->left_subtree : subtree_root->right_subtree;
    }
    node* node = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(const binary_search_tree<tkey, tvalue>* holder,
                                                                     const std::stack<node*> &state):
                                                                     iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(postfix_iterator const &other):
    iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(postfix_iterator &&other):
        iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator=(postfix_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator=(postfix_iterator &&other)
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::~postfix_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_iterator::next_node()

{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }

    node* curr_node = _state.top();
    _state.pop();

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->right_subtree == curr_node || _state.top()->right_subtree == nullptr)
    {
        return;
    }

    curr_node = _state.top()->right_subtree;
    while (curr_node)
    {
        _state.push(curr_node);
        curr_node = curr_node->left_subtree ? curr_node->left_subtree : curr_node->right_subtree;
    }
}

// endregion postfix_iterator implementation

// region postfix_const_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(binary_search_tree<tkey, tvalue> const *holder,
                                                                     typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        const_iterator(holder)
{
    while (subtree_root)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree ? subtree_root->left_subtree : subtree_root->right_subtree;
    }
    node* node = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(const binary_search_tree<tkey, tvalue>* holder,
                                                                     const std::stack<node*> &state):
        const_iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(postfix_const_iterator const &other):
        const_iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(postfix_const_iterator &&other):
        const_iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator=(postfix_const_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator=(postfix_const_iterator &&other)
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::~postfix_const_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_const_iterator::next_node()

{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }

    node* curr_node = _state.top();
    _state.pop();

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->right_subtree == curr_node || _state.top()->right_subtree == nullptr)
    {
        return;
    }

    curr_node = _state.top()->right_subtree;
    while (curr_node)
    {
        _state.push(curr_node);
        curr_node = curr_node->left_subtree ? curr_node->left_subtree : curr_node->right_subtree;
    }
}


// endregion postfix_const_iterator implementation

// region postfix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(binary_search_tree<tkey, tvalue> const *holder,
                                                                     typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        iterator(holder)
{
    while (subtree_root)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->right_subtree ? subtree_root->right_subtree : subtree_root->left_subtree;
    }
    node* node = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(const binary_search_tree<tkey, tvalue>* holder,
                                                                     const std::stack<node*> &state):
        iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(postfix_reverse_iterator const &other):
        iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(postfix_reverse_iterator &&other):
        iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator=(postfix_reverse_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator=(postfix_reverse_iterator &&other)
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::~postfix_reverse_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::next_node()

{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }

    node* curr_node = _state.top();
    _state.pop();

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->left_subtree == curr_node || _state.top()->left_subtree == nullptr)
    {
        return;
    }

    curr_node = _state.top()->left_subtree;
    while (curr_node)
    {
        _state.push(curr_node);
        curr_node = curr_node->right_subtree ? curr_node->right_subtree : curr_node->left_subtree;
    }
}


// endregion postfix_reverse_iterator implementation

// region postfix_const_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(binary_search_tree<tkey, tvalue> const *holder,
                                                                                 typename binary_search_tree<tkey, tvalue>::node* subtree_root):
        const_iterator(holder)
{
    while (subtree_root)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->right_subtree ? subtree_root->right_subtree : subtree_root->left_subtree;
    }
    node* node = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, node);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(const binary_search_tree<tkey, tvalue>* holder,
                                                                                 const std::stack<node*> &state):
        const_iterator(holder, state)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(postfix_const_reverse_iterator const &other):
        const_iterator(other)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(postfix_const_reverse_iterator &&other):
        const_iterator(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator=(postfix_const_reverse_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator=(postfix_const_reverse_iterator &&other)
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }
    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::~postfix_const_reverse_iterator()
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::next_node()

{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return;
    }

    node* curr_node = _state.top();
    _state.pop();

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->left_subtree == curr_node || _state.top()->left_subtree == nullptr)
    {
        return;
    }

    curr_node = _state.top()->left_subtree;
    while (curr_node)
    {
        _state.push(curr_node);
        curr_node = curr_node->right_subtree ? curr_node->right_subtree : curr_node->left_subtree;
    }
}
// endregion postfix_const_reverse_iterator implementation

// endregion iterators implementation

// region target operations associated exception types implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to insert already existing key inside the tree."),
    _key(key)
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to obtain a value by non-existing key from the tree."),
    _key(key)
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to dispose a value by non-existing key from the tree."),
    _key(key)
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

// endregion target operations associated exception types implementation

// region template methods implementation

// region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
    binary_search_tree<tkey, tvalue> *tree)
    : _tree(tree)
{

}

template<
        typename tkey,
        typename tvalue>
std::stack<typename binary_search_tree<tkey, tvalue>::node**> binary_search_tree<tkey, tvalue>::template_method_basics::find_path(tkey const &key) const
{
    auto const &comparer = _tree->_keys_comparer;
    std::stack<node**> path;

    node **path_finder = reinterpret_cast<node**>(&(_tree->_root));
    int comparison_result;

    while (true)
    {
        path.push(path_finder);
        if (*path_finder == nullptr || (comparison_result = comparer(key, (*path_finder)->key)) == 0)
        {
            break;
        }
        path_finder = comparison_result < 0 ? &((*path_finder)->left_subtree) : &((*path_finder)->right_subtree);
    }
    return path;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::template_method_basics::balance(
        std::stack<node**> &path, typename binary_search_tree<tkey, tvalue>::node* node_to_dispose)
{ }


template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    return this->_tree->get_logger();
}

// endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

// region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree),
    _insertion_strategy(insertion_strategy)
{

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue const &value)
{
    auto path = this->find_path(key);
    if (*(path.top()) != nullptr)
    {
        switch (_insertion_strategy)
        {
            case insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw insertion_of_existent_key_attempt_exception(key);
            case insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = value;
                break;
        }

        return;
    }

    this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(allocate_with_guard(this->_tree->get_node_size(), 1)), key, value);

    this->balance(path);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value)
{
    auto path = this->find_path(key);
    if (*(path.top()) != nullptr)
    {
        switch (_insertion_strategy)
        {
            case insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw insertion_of_existent_key_attempt_exception(key);
            case insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = std::move(value);
                break;
        }

        return;
    }

    this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(allocate_with_guard(this->_tree->get_node_size(), 1)), key, std::move(value));

    this->balance(path);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    _insertion_strategy = insertion_strategy;
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

// endregion search_tree<tkey, tvalue>::insertion_template_method implementation

// region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{

}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
    tkey const &key)
{
    auto path = this->find_path(key);
    if (*(path.top()) == nullptr)
    {
        throw obtaining_of_nonexistent_key_attempt_exception(key);
    }

    tvalue const &got_value = (*(path.top()))->value;

    this->balance(path);

    return got_value;
}

// endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

// region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics(tree)
{

}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
    tkey const &key)
{
    auto path = this->find_path(key);
    if (*(path.top()) == nullptr)
    {
        switch (_disposal_strategy)
        {
            case disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
                throw disposal_of_nonexistent_key_attempt_exception(key);
            case disposal_of_nonexistent_key_attempt_strategy::do_nothing:
                return tvalue();
        }
    }

    if ((*(path.top()))->left_subtree != nullptr && (*(path.top()))->right_subtree != nullptr)
    {
        auto *target_to_swap = *(path.top());
        auto **current = &((*(path.top()))->left_subtree);

        while (*current != nullptr)
        {
            path.push(current);
            current = &((*current)->right_subtree);
        }

        std::swap(target_to_swap->key, (*(path.top()))->key);
        std::swap(target_to_swap->value, (*(path.top()))->value);
    }

    tvalue value = std::move((*(path.top()))->value);
    node *leftover_subtree = (*(path.top()))->left_subtree == nullptr
        ? (*(path.top()))->right_subtree
        : (*(path.top()))->left_subtree;

    node *deleted_node = *(path.top());

    *(path.top()) = leftover_subtree;

    this->balance(path, deleted_node);
    allocator::destruct(deleted_node);
    deallocate_with_guard(deleted_node);
    return value;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _disposal_strategy = disposal_strategy;
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

// endregion search_tree<tkey, tvalue>::disposal_template_method implementation

// endregion template methods

// region construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
    typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
    typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger):
    search_tree<tkey, tvalue>(comparer, allocator, logger),
    _insertion_template(insertion_template),
    _obtaining_template(obtaining_template),
    _disposal_template(disposal_template),
    _mutex()
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    search_tree<tkey, tvalue>(keys_comparer, allocator, logger),
    _mutex()
{
        try
        {
            _insertion_template = new binary_search_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy);
            _obtaining_template = new binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
            _disposal_template = new binary_search_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy);
        }
        catch (const std::bad_alloc &err)
        {
            delete _insertion_template;
            delete _obtaining_template;
            delete _disposal_template;
            throw;
        }

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> const &other):
        search_tree<tkey, tvalue>(other._keys_comparer, other.get_allocator(), other.get_logger()),
        _insertion_template(new binary_search_tree<tkey, tvalue>::insertion_template_method(this, binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception)),
        _obtaining_template(new binary_search_tree<tkey, tvalue>::obtaining_template_method(this)),
        _disposal_template(new binary_search_tree<tkey, tvalue>::disposal_template_method(this, binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception)),
        _mutex()
{
    this->_root = copy(reinterpret_cast<node*>(other._root));






//    try
//    {
//        this->_root = copy(reinterpret_cast<node*>(other._root));
//        _insertion_template = new binary_search_tree<tkey, tvalue>::insertion_template_method(this, other._insertion_template->_insertion_strategy);
//        _obtaining_template = new binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
//        _disposal_template = new binary_search_tree<tkey, tvalue>::disposal_template_method(this, other._disposal_template->_disposal_strategy);
//    }
//    catch (const std::bad_alloc& ex)
//    {
//        clear(reinterpret_cast<node**>(&this->_root));
//        delete _insertion_template;
//        delete _obtaining_template;
//        delete _disposal_template;
//        throw;
//    }
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> &&other) noexcept:
        search_tree<tkey, tvalue>(other._keys_comparer, other.get_allocator(), other.get_logger())
{
    other._logger = nullptr;
    other._allocator = nullptr;

    _insertion_template = other._insertion_template;
    other._insertion_template = nullptr;

    _obtaining_template = other._obtaining_template;
    other._obtaining_template = nullptr;

    _disposal_template = other._disposal_template;
    other._disposal_template = nullptr;

    this->_root = other._root;
    other._root = nullptr;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        clear(reinterpret_cast<node**>(&this->_root));
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        this->_keys_comparer = other._keys_comparer;
        this->_root = copy(reinterpret_cast<node*>(other._root));

        *_insertion_template = *(other._insertion_template);
        *_obtaining_template = *(other._obtaining_template);
        *_disposal_template = *(other._disposal_template);

    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        clear(reinterpret_cast<node**>(&this->_root));


        this->_keys_comparer = std::move(other._keys_comparer);

        this->_logger = std::move(other._logger);

        this->_allocator = std::move(other._allocator);

        this->_root = std::move(other._root);


        _insertion_template = std::move(other._insertion_template);
        _obtaining_template = std::move(other._obtaining_template);
        _disposal_template = std::move(other._disposal_template);

        other._allocator = nullptr;
        other._logger = nullptr;
        other._root = nullptr;
        other._insertion_template = nullptr;
        other._obtaining_template = nullptr;
        other._disposal_template = nullptr;
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    clear(reinterpret_cast<node**>(&this->_root));
    delete _insertion_template;
    delete _obtaining_template;
    delete _disposal_template;
}

// endregion construction, assignment, destruction implementation

// region associative_container<tkey, tvalue> contract implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _insertion_template->insert(key, value);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _insertion_template->insert(key, std::move(value));
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _obtaining_template->obtain(key);
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _obtaining_template->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _disposal_template->dispose(key);
}

// endregion associative_containers contract implementations

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::set_insertion_strategy(typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::set_removal_strategy(typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy) noexcept", "your code should be here...");
}

// region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(this, nullptr);
}

// endregion iterators request implementation

// region subtree rotations implementation

template<
        typename tkey,
        typename tvalue>
inline void binary_search_tree<tkey, tvalue>::update_node_data(node *node) const noexcept
{

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if (validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr))
    {
        throw std::logic_error("Cannot perform small left rotation");
    }
    binary_search_tree<tkey, tvalue>::node *new_subtree_root = subtree_root->right_subtree;
    subtree_root->right_subtree = new_subtree_root->left_subtree;
    new_subtree_root->left_subtree = subtree_root;
    subtree_root = new_subtree_root;
    update_node_data(subtree_root->left_subtree);
    update_node_data(new_subtree_root);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr))
    {
        throw std::logic_error("Cannot perform small right rotation");
    }
    binary_search_tree<tkey, tvalue>::node *new_subtree_root = subtree_root->left_subtree;
    subtree_root->left_subtree = new_subtree_root->right_subtree;
    new_subtree_root->right_subtree = subtree_root;
    subtree_root = new_subtree_root;
    update_node_data(subtree_root->right_subtree);
    update_node_data(new_subtree_root);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if (validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr || subtree_root->right_subtree->left_subtree ==
                                                                                                 nullptr))
    {
        throw std::logic_error("Cannot perform big left rotation");
    }
    small_right_rotation(subtree_root->right_subtree, false);
    small_left_rotation(subtree_root, false);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr || subtree_root->left_subtree->right_subtree ==
                                                                                          nullptr))
    {
        throw std::logic_error("Cannot perform big right rotation");
    }
    small_left_rotation(subtree_root->left_subtree, false);
    small_right_rotation(subtree_root, false);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::double_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::double_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool, bool) const", "your code should be here...");
}

// endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H