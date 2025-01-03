#pragma

#include <memory>
#include <iterator>
#include "UtlMemory.hpp"

namespace utils
{
    template <class T, class I>
    struct UtlLinkedListElement_t
    {
        UtlLinkedListElement_t(const UtlLinkedListElement_t&) = delete;

        T element;
        I previous;
        I next;
    };

    template <class T, class S = unsigned short, bool ML = false, class I = S, class M = CUtlMemory<UtlLinkedListElement_t<T, S>, I>>
    class CUtlLinkedList
    {
    public:
        using elem_type_t = T;
        using index_type_t = S;
        using index_local_type_t = I;
        using memory_allocator_t = M;

        template <typename List_t>
        class const_iterator_t
        {
        public:
            using elem_type_t = typename List_t::elem_type_t;
            using index_type_t = typename List_t::index_type_t;

            const_iterator_t() :
                list_ptr(nullptr), index(List_t::invalid_index()) { }

            const_iterator_t(const List_t& list, index_type_t index) :
                list_ptr(&list), index(index) { }

            const_iterator_t& operator++()
            {
                index = list_ptr->next(index);
                return *this;
            }

            const_iterator_t operator++(int)
            {
                const_iterator_t copy = *this;
                ++(*this);
                return copy;
            }

            const_iterator_t& operator--()
            {
                assert(index != list_ptr->head());
                index = (index == list_ptr->invalid_index() ? list_ptr->tail() : list_ptr->previous(index));
                return *this;
            }

            const_iterator_t operator--(int)
            {
                const_iterator_t copy = *this;
                --(*this);
                return copy;
            }

            bool operator==(const const_iterator_t& other) const
            {
                assert(list_ptr == other.list_ptr);
                return index == other.index;
            }

            bool operator!=(const const_iterator_t& other) const
            {
                assert(list_ptr == other.list_ptr);
                return index != other.index;
            }

            const elem_type_t& operator*() const
            {
                return list_ptr->element(index);
            }

            const elem_type_t* operator->() const
            {
                return (&**this);
            }

        protected:
            const List_t* list_ptr;
            index_type_t index;
        };

        template <typename List_t>
        class iterator_t : public const_iterator_t<List_t>
        {
        public:
            using elem_type_t = typename List_t::elem_type_t;
            using index_type_t = typename List_t::index_type_t;
            using base_t = const_iterator_t<List_t>;

            iterator_t() { }

            iterator_t(const List_t& list, index_type_t index) :
                const_iterator_t<List_t>(list, index) { }

            iterator_t& operator++()
            {
                base_t::index = base_t::list_ptr->next(base_t::index);
                return *this;
            }

            iterator_t operator++(int)
            {
                iterator_t copy = *this;
                ++(*this);
                return copy;
            }

            iterator_t& operator--()
            {
                base_t::index = (base_t::index == base_t::list_ptr->invalid_index() ? base_t::list_ptr->tail() : base_t::list_ptr->previous(base_t::index));
                return *this;
            }

            iterator_t operator--(int)
            {
                iterator_t copy = *this;
                --(*this);
                return copy;
            }

            elem_type_t& operator*() const
            {
                List_t* mutable_list = const_cast<List_t*>(base_t::list_ptr);
                return mutable_list->element(base_t::index);
            }

            elem_type_t* operator->() const
            {
                return (&**this);
            }
        };

        CUtlLinkedList(int grow_size = 0, int size = 0) :
            memory{ nullptr, size, grow_size }, head_index(invalid_index()), tail_index(invalid_index()), first_free_index(invalid_index()), element_count(0), allocated_count(0), last_alloc(memory.first()), elements(memory.base()) { }

        ~CUtlLinkedList()
        {
            remove_all();
        }

        CUtlLinkedList(const CUtlLinkedList&) = delete;
        CUtlLinkedList& operator=(const CUtlLinkedList&) = delete;

        T& operator[](const I index)
        {
            assert(is_valid_index(index));
            return memory[index].element;
        }

        const T& operator[](const I index) const
        {
            assert(is_valid_index(index));
            return memory[index].element;
        }

        [[nodiscard]] T& element(const I index)
        {
            assert(is_valid_index(index));
            return memory[index].element;
        }

        [[nodiscard]] const T& element(const I index) const
        {
            assert(is_valid_index(index));
            return memory[index].element;
        }

        [[nodiscard]] I head() const
        {
            return head_index;
        }

        [[nodiscard]] I tail() const
        {
            return tail_index;
        }

        [[nodiscard]] I previous(const I index) const
        {
            assert(is_valid_index(index));
            return internal_element(index).previous;
        }

        [[nodiscard]] I next(const I index) const
        {
            assert(is_valid_index(index));
            return internal_element(index).next;
        }

        [[nodiscard]] static S invalid_index()
        {
            return static_cast<S>(M::invalid_index());
        }

        [[nodiscard]] bool is_valid_index(const I index) const
        {
            return memory.is_valid_index(index);
        }

        [[nodiscard]] I find(const T& source) const
        {
            for (I i = head_index; i != invalid_index(); i = next(i))
            {
                if (element(i) == source)
                    return i;
            }

            return invalid_index();
        }

        void remove_all()
        {
            for (I i = head_index; i != invalid_index();)
            {
                I next_index = next(i);
                memory[i].~UtlLinkedListElement_t();
                i = next_index;
            }

            head_index = tail_index = first_free_index = invalid_index();
            element_count = 0;
        }

        [[nodiscard]] auto begin() const
        {
            return const_iterator_t<CUtlLinkedList<T, S, ML, I, M>>(*this, head());
        }

        [[nodiscard]] auto begin()
        {
            return iterator_t<CUtlLinkedList<T, S, ML, I, M>>(*this, head());
        }

        [[nodiscard]] auto end() const
        {
            return const_iterator_t<CUtlLinkedList<T, S, ML, I, M>>(*this, invalid_index());
        }

        [[nodiscard]] auto end()
        {
            return iterator_t<CUtlLinkedList<T, S, ML, I, M>>(*this, invalid_index());
        }

    protected:
        using list_element_t = UtlLinkedListElement_t<T, S>;

        [[nodiscard]] list_element_t& internal_element(const I index)
        {
            return memory[index];
        }

        [[nodiscard]] const list_element_t& internal_element(const I index) const
        {
            return memory[index];
        }

        M memory;
        I head_index;
        I tail_index;
        I first_free_index;
        I element_count;
        I allocated_count;
        typename M::iterator_t last_alloc;
        list_element_t* elements;
    };

}
