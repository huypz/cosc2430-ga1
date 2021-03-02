#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <fstream>

template <typename T>
class LinkedList {
    template <typename E>
    struct Node {
        Node() : next(nullptr), prev(nullptr), val(0) {};
        Node(E val) : next(nullptr), prev(nullptr), val(val) {};
        Node<E>* next;
        Node<E>* prev;
        E val;
    };
    public:
        static LinkedList<T>* merge(LinkedList<T>* list1, LinkedList<T>* list2);

        LinkedList();
        ~LinkedList();
        void add(const T& e);
        bool empty() const;
        bool removeElement(const T& e);
        void removeIndex(int i);
        void reverse();
        std::ofstream& write(std::ofstream& ofs) const;
        std::ofstream& writeCode(std::ofstream& ofs) const;
    private:
        Node<T>* header;
        Node<T>* trailer;
        int size;
        bool removeElement(Node<T>* cursor, const T& e);
        void removeIndex(Node<T>* cursor, int i);
        Node<T>* reverse(Node<T>* cursor);
};

template <typename T>
LinkedList<T>* LinkedList<T>::merge(LinkedList<T>* list1, LinkedList<T>* list2) {
    LinkedList<T>* list_merged = new LinkedList<T>();
    Node<T>* curr = list1->header->next;
    while (curr != list1->trailer) {
        list_merged->add(curr->val);
        curr = curr->next;
    }
    curr = list2->header->next;
    while (curr != list2->trailer) {
        list_merged->add(curr->val);
        curr = curr->next;
    }
    return list_merged;
}

template <typename T>
LinkedList<T>::LinkedList() : header(new Node<T>()), trailer(new Node<T>()), size(0) {
    header->next = trailer;
    trailer->prev = header;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    while (!empty())
        removeIndex(0);
    delete header;
    delete trailer;
}

template <typename T>
void LinkedList<T>::add(const T& e) {
    Node<T>* new_node = new Node<T>(e);
    new_node->prev = trailer->prev;
    new_node->next = trailer;
    trailer->prev->next = new_node;
    trailer->prev = new_node;
    size++;
}

template <typename T>
bool LinkedList<T>::empty() const {
    return (size == 0);
}

template <typename T>
bool LinkedList<T>::removeElement(const T& e) {
    if (empty()) return false;
    return removeElement(header->next, e);
}

template <typename T>
bool LinkedList<T>::removeElement(Node<T>* cursor, const T& e) {
    if (cursor == trailer) return false;
    if (cursor->val == e) {
        cursor->prev->next = cursor->next;
        cursor->next->prev = cursor->prev;
        delete cursor;
        size--;
        return true;
    }
    return removeElement(cursor->next, e);
}

template <typename T>
void LinkedList<T>::removeIndex(int i) {
    if (empty() || i < 0 || i >= size) return;
    else removeIndex(header->next, i);
}

template <typename T>
void LinkedList<T>::removeIndex(Node<T>* cursor, int i) {
    if (i == 0) {
        cursor->prev->next = cursor->next;
        cursor->next->prev = cursor->prev;
        delete cursor;
        size--;
        return;
    }
    removeIndex(cursor->next, --i);
}

template <typename T>
void LinkedList<T>::reverse() {
    if (empty() || size == 1) return;
    Node<T>* temp = header;
    header = reverse(header);
    trailer = temp;
}

template <typename T>
LinkedList<T>::Node<T>* LinkedList<T>::reverse(Node<T>* cursor) {
    if (!(cursor->next))
        return cursor;
    Node<T>* res = reverse(cursor->next);
    cursor->next->next = cursor;
    cursor->prev = cursor->next;
    cursor->next = nullptr;
    return res;
}

template <typename T>
std::ofstream& LinkedList<T>::write(std::ofstream& ofs) const {
    ofs << "[";
    Node<T>* curr = header->next;
    while (curr->next != trailer) {
        ofs << curr->val << ", ";
        curr = curr->next;
    }
    ofs << curr->val << "]";
    return ofs;
}

template <typename T>
std::ofstream& LinkedList<T>::writeCode(std::ofstream& ofs) const {
    ofs << "| ";
    Node<T>* curr = header->next;
    while (curr->next != trailer) {
        ofs << curr->val << " | ";
        curr = curr->next;
    }
    ofs << curr->val << " |";
    return ofs;
}
#endif