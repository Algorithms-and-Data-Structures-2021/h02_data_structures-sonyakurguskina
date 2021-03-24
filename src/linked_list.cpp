#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

    void LinkedList::Add(Element e) {
        if(head_ == nullptr){
            Node *node = new Node(e, nullptr);
            head_ = node;
            tail_ = node;

        }else{
            Node *node = new Node(e, nullptr);
            Node *previous_node = tail_;
            previous_node->next = node;
            tail_ = node;
        }

        size_++;
    }

    void LinkedList::Insert(int index, Element e) {
        internal::check_out_of_range(index, 0, size_ + 1);

        if (head_ == nullptr) {
            Node *newNode = new Node(e, nullptr);
            head_ = newNode;
            tail_ = newNode;
        }

        if (index == size_) {
            Node *newNode = new Node(e, nullptr);
            tail_->next = newNode;
            tail_ = newNode;
        }

        if (index == 0) {
            Node *newNode = new Node(e, head_);
            newNode->next = head_;
            head_ = newNode;
        }

        if(index > 0 && index < size_){
            Node *previousNode = find_node(index-1);
            Node *newNode = new Node(e, find_node(index));
            previousNode->next = newNode;
        }

        size_++;
    }

    void LinkedList::Set(int index, Element e) {
        internal::check_out_of_range(index, 0, size_);
        Node *node = find_node(index);
        node->data = e;
    }

    Element LinkedList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        Element element;

        if(index == 0){
            Node *node = head_;
            Node *nextNode = node->next;
            element = node->data;

            delete node;

            head_ = nextNode;
            size_--;

        } else{
            Node *previousNode = find_node(index - 1);
            Node *currentNode = find_node(index);
            Node *nextNode = currentNode->next;
            element = currentNode->data;

            delete currentNode;

            previousNode->next = nextNode;
            size_--;
        }

        return element;
    }

    void LinkedList::Clear() {
        if(head_ == nullptr){
            return;
        }

        Node *node = head_;

        while(node != nullptr){
            Node *nextNode = node->next;

            delete node;

            node = nextNode;
            head_ = nextNode;
            size_--;
        }

        tail_ = nullptr;
    }

    Element LinkedList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);

        int counter = 0;

        for(Node *i = head_; i != nullptr; i = i->next){
            if(counter == index){
                return i->data;
            }

            counter++;
        }

        return Element::UNINITIALIZED;
    }

    int LinkedList::IndexOf(Element e) const {
        int counter = 0;

        for(Node *i = head_; i != nullptr; i = i->next){
            if(i->data == e){
                return counter;
            }

            counter++;
        }

        return kNotFoundElementIndex;
    }

    Node *LinkedList::find_node(int index) const {
        assert(index >= 0 && index < size_);
        int counter = 0;

        if(index == 0){
            return head_;
        }

        if(index == size_ - 1){
            return tail_;
        }

        for(Node *i = head_; i != nullptr; i = i->next){
            if(counter == index){
                return i;
            }

            counter++;
        }

        return nullptr;
    }

// РЕАЛИЗОВАНО

    LinkedList::~LinkedList() {
        Clear();
    }

    bool LinkedList::Contains(Element e) const {
        // если индекс не найден, значит и элемента нет
        return IndexOf(e) == kNotFoundElementIndex;
    }

    int LinkedList::GetSize() const {
        return size_;
    }

    bool LinkedList::IsEmpty() const {
        return size_ == 0;
    }

    Element LinkedList::tail() const {
        // вместо выброса ошибки в случае nullptr, римским парламентом было решено возвращать "специальное" значение
        return tail_ ? tail_->data : Element::UNINITIALIZED;
    }

    Element LinkedList::head() const {
        return head_ ? head_->data : Element::UNINITIALIZED;
    }

// === RESTRICTED AREA: необходимо для тестирования ===

    LinkedList::LinkedList(const std::vector<Element> &elements) {
        assert(!elements.empty());

        size_ = elements.size();
        head_ = new Node(elements[0], nullptr);

        auto current_node = head_;

        for (int index = 1; index < static_cast<int>(elements.size()); index++) {
            current_node->next = new Node(elements[index], nullptr);
            current_node = current_node->next;
        }
        tail_ = current_node;
    }

    std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
        if (list.head_ != nullptr && list.tail_ != nullptr) {
            os << "{ ";
            for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
                os << internal::elem_to_str(current_node->data) << ", ";
            }
            os << internal::elem_to_str(list.tail_->data) << " }";
        } else {
            os << "{ nullptr }";
        }
        return os;
    }

    bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
        if (list.size_ != static_cast<int>(elements.size())) return false;
        Node *current_node = list.head_;

        for (const auto e : elements) {
            if (current_node == nullptr) return false;
            if (current_node->data != e) return false;
            current_node = current_node->next;
        }
        return true;
    }

}  // namespace itis
