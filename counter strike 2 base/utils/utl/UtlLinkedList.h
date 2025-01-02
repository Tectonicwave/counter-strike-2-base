#pragma

#include <memory>
#include <iterator>

namespace utils
{

    template <class T, class I>
    struct utl_linked_list_element_t
    {
        T elem;    // The actual data of the linked list element
        I previous; // Index of the previous element
        I next;     // Index of the next element

        utl_linked_list_element_t(I prev = -1, I nxt = -1)
            : previous(prev), next(nxt) {}
    };

    template <class T, class I, class M>
    class CUtlLinkedList
    {
    public:
        using elem_type_t = T;
        using index_type_t = I;
        using memory_allocator_t = M;

        // Iterator Class
        template <typename list_t>
        class const_iterator_t
        {
        public:
            using elem_type_t = typename list_t::elem_type_t;
            using index_type_t = typename list_t::index_type_t;

            const_iterator_t() noexcept : list(nullptr), index(list_t::invalid_index()) {}
            const_iterator_t(const list_t& lst, index_type_t idx) noexcept
                : list(&lst), index(idx) {}

            const_iterator_t& operator++() noexcept
            {
                index = list->next(index);
                return *this;
            }

            const_iterator_t operator++(int) noexcept
            {
                const_iterator_t tmp = *this;
                ++(*this);
                return tmp;
            }

            const_iterator_t& operator--() noexcept
            {
                index = (index == list->invalid_index()) ? list->tail() : list->prev(index);
                return *this;
            }

            const_iterator_t operator--(int) noexcept
            {
                const_iterator_t tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const const_iterator_t& other) const noexcept { return index == other.index; }
            bool operator!=(const const_iterator_t& other) const noexcept { return index != other.index; }

            const elem_type_t& operator*() const { return list->element(index); }
            const elem_type_t* operator->() const { return &(**this); }

        private:
            const list_t* list;
            index_type_t index;
        };

        // Constructor with optional grow size and initial size
        explicit CUtlLinkedList(int nGrowSize = 0, int nSize = 0)
            : _memory(nGrowSize, nSize), _head(invalid_index()), _tail(invalid_index()),
            _first_free(invalid_index()), _elem_count(0), _num_allocated(0),
            _last_alloc(_memory.invalid_iterator()), _elements(_memory.base()) {}

        ~CUtlLinkedList() noexcept = default;

        // Disallow copying
        CUtlLinkedList(const CUtlLinkedList&) = delete;
        CUtlLinkedList& operator=(const CUtlLinkedList&) = delete;

        // Accessor for elements
        T& operator[](const I idx) { return _memory[idx].elem; }
        const T& operator[](const I idx) const { return _memory[idx].elem; }

        T& element(const I idx) { return _memory[idx].elem; }
        const T& element(const I idx) const { return _memory[idx].elem; }

        // Head and tail of the list
        I head() const noexcept { return _head; }
        I tail() const noexcept { return _tail; }

        // Get previous and next elements by index
        I prev(const I idx) const noexcept { return _memory[idx].previous; }
        I next(const I idx) const noexcept { return _memory[idx].next; }

        // Invalid index
        static I invalid_index() noexcept { return static_cast<I>(-1); }

        // Iterators
        auto begin() const { return const_iterator_t<CUtlLinkedList<T, I, M>>(*this, head()); }
        auto begin() { return iterator_t<CUtlLinkedList<T, I, M>>(*this, head()); }
        auto end() const { return const_iterator_t<CUtlLinkedList<T, I, M>>(*this, invalid_index()); }
        auto end() { return iterator_t<CUtlLinkedList<T, I, M>>(*this, invalid_index()); }

    protected:
        using list_element_t = utl_linked_list_element_t<T, I>;

        // Helper function to get internal element by index
        list_element_t& internal_element(const I idx) { return _memory[idx]; }
        const list_element_t& internal_element(const I idx) const { return _memory[idx]; }

        M _memory; // Memory allocator
        I _head;   // Head of the list
        I _tail;   // Tail of the list
        I _first_free; // First free element (unused element)
        I _elem_count; // Number of elements in the list
        I _num_allocated; // Number of elements allocated
        typename M::iterator_t _last_alloc; // Iterator for last allocated memory
        list_element_t* _elements; // Base pointer to elements
    };

}
