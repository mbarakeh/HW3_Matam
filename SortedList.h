#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template <typename T>
    class SortedList {
        class Node{
        public:
         T data;
         Node* next;

         explicit  Node(T data) : data(data),next(nullptr) {}
        };

        Node* head;
        int size;

        void copy(const SortedList &other);

        void clear_list();


    public:
        SortedList();

        ~SortedList();

        SortedList(const SortedList &other);

        SortedList &operator=(const SortedList &other);

        class ConstIterator;

        ConstIterator begin() const { return ConstIterator(head, 0); }

        ConstIterator end() const { return ConstIterator(nullptr, size); }

        void insert(const T &data);

        void remove(const ConstIterator &it);

        int length() const;


        template<class Condition>
        SortedList filter(Condition c) const;

        template<typename Functor>
        SortedList<T> apply(Functor operation) const;


    };

    template<typename T>
    void SortedList<T>::copy(const SortedList &other) {
        if (other.head == nullptr) {
            head = nullptr;
            size = 0;
            return;
        }

        try {
            head = new Node(other.head->data);
        } catch (const std::bad_alloc &) {
            clear_list();
            throw;
        }

        Node *currentThis = head;
        Node *currentOther = other.head->next;

        while (currentOther != nullptr) {
            try {
                currentThis->next = new Node(currentOther->data);
            } catch (const std::bad_alloc &) {
                clear_list();
                throw;
            }
            currentThis = currentThis->next;
            currentOther = currentOther->next;
        }
        this->size = other.size;
    }

    template<typename T>
    void SortedList<T>::clear_list() {
        Node *current = head;
        while (current) {
            Node *nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        size = 0;
    }

    template<typename T>
    SortedList<T>::SortedList() : head(nullptr), size(0) {}

    template<typename T>
    SortedList<T>::~SortedList() {
        clear_list();
    }

    template<typename T>
    SortedList<T>::SortedList(const SortedList<T> &other) : head(nullptr), size(0) {
        copy(other);
    }

    template<typename T>
    SortedList<T> &SortedList<T>::operator=(const SortedList &other) {
        if (this == &other) {
            return *this;
        }
        SortedList<T> temp;

        try {
            temp.copy(other);
        } catch (const std::bad_alloc &) {
            throw;
        }
        clear_list();
        head = temp.head;
        size = temp.size;

        temp.head = nullptr;
        temp.size = 0;
        return *this;
    }



    template<typename T>
    void SortedList<T>::insert(const T &data) {
        Node *newNode = nullptr;
        try {
            newNode = new Node(data);
        } catch (const std::bad_alloc &) {
            clear_list();
            throw;
        }

        if (!head || data > head->data) {
            newNode->next = head;
            head = newNode;
        } else {
            Node *current = head;
            while (current->next && current->next->data > data) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        size++;
    }

    template<typename T>
    int SortedList<T>::length() const {
        return this->size;
    }

    template<typename T>
    void SortedList<T>::remove(const SortedList::ConstIterator &it) {
        if (head == nullptr || it.current == nullptr) {
            return;
        }

        if (head == it.current) {
            Node *temp = head;
            head = head->next;
            delete temp;
        } else {
            Node *current = head;
            while (current->next && current->next != it.current) {
                current = current->next;
            }
            if (current->next) {
                Node *temp = current->next;
                current->next = current->next->next;
                delete temp;
            }
        }
        size--;
    }

    template<typename T>
    template<typename Functor>
    SortedList<T> SortedList<T>::apply(Functor operation) const {
        SortedList<T> result;
        for (const T &elem: *this) {
            result.insert(operation(elem));
        }
        return result;
    }


    template<typename T>
    template<class Condition>
    SortedList<T> SortedList<T>::filter(Condition c) const {
        SortedList<T> result;
        for (const T &elem: *this) {
            if (c(elem)) {
                result.insert(elem);
            }
        }
        return result;
    }






    template <class T>
    class SortedList<T>::ConstIterator {
        const Node *current;
        int index;

        explicit ConstIterator(const Node *node, int idx = 0) : current(node), index(idx) {}

        friend class SortedList<T>;

    public:
        ConstIterator(const ConstIterator &other) = default;

        ~ConstIterator() = default;

        ConstIterator &operator=(const ConstIterator &other) = default;

        ConstIterator &operator++();

        bool operator!=(const ConstIterator &other) const;

        const T &operator*() const;
    };

    template<typename T>
    bool SortedList<T>::ConstIterator::operator!=(const ConstIterator &other) const {
        return current != other.current;
    }

    template<typename T>
    typename SortedList<T>::ConstIterator &SortedList<T>::ConstIterator::operator++() {
        if (!current) {
            throw std::out_of_range("Iterator cannot be incremented");
        }
        current = current->next;
        index++;
        return *this;
    }

    template<typename T>
    const T &SortedList<T>::ConstIterator::operator*() const {
        if (!current) {
            throw std::out_of_range("Iterator is out of range");
        }
        return current->data;
    }

}

