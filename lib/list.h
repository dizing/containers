#if !defined(CONTAINERS_LIB_LIST_H)
#define CONTAINERS_LIB_LIST_H

#include <memory>

struct BaseListNode {
  BaseListNode *prev;
  BaseListNode *next;
};
template <typename T>
struct ListNode : public BaseListNode {
  T value_;

  ListNode(const T &value) : value_(value) {}
  ListNode(T &&value) : value_(std::move(value)) {}
  template <typename... Args>
  ListNode(Args &&...value) : value_(T(std::forward<Args>(value)...)) {}
};

template <typename T, bool isConst>
struct ListIterator {
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using base_node = BaseListNode;
  using node = ListNode<value_type>;
  base_node *node_;

  ListIterator(base_node *node) : node_(node) {}

  ListIterator<T, isConst> &operator++() {
    node_ = node_->next;
    return *this;
  }
  reference operator*() const { return static_cast<node *>(node_)->value_; }
  base_node *GetNode() { return node_; }
  bool operator==(const ListIterator &other) const {
    return node_ == other.node_;
  }
  bool operator!=(const ListIterator &other) const { return !(*this == other); }
};

template <typename T, typename Allocator = std::allocator<T>>
class list {
 public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = ListIterator<value_type, false>;
  using const_iterator = ListIterator<value_type, true>;
  using size_type = std::size_t;
  using base_node = BaseListNode;
  using node = ListNode<value_type>;
  using node_alloc = typename Allocator::rebind<node>::other;
  using node_pointer = node *;
  using base_node_pointer = base_node *;
  base_node fakeNode_ = {nullptr, nullptr};

  list() : alloc_(node_alloc()), size_(0) {
    fakeNode_.next = &fakeNode_;
    fakeNode_.prev = &fakeNode_;
  }

  // Creates List Node with value_type element inside.
  // Receives universal reference parameter pack.
  // It can be lvalue value_type, rvalue value_type, or pack of arguments to
  // value_type constructor.
  template <typename... Args>
  node_pointer CreateNode(Args &&...value) {
    node_pointer temp = std::allocator_traits<node_alloc>::allocate(alloc_, 1);
    std::allocator_traits<node_alloc>::construct(alloc_, temp,
                                                 std::forward<Args>(value)...);
    return temp;
  }

  // Receives the element before which the new one will be created and parameter
  // pack for CreateNode
  template <typename... Args>
  void InsertNodeBefore(iterator pos, Args &&...value) {
    node_pointer new_node = CreateNode(std::forward<Args>(value)...);
    base_node_pointer node_from_pos = pos.GetNode();
    new_node->next = node_from_pos;
    new_node->prev = node_from_pos->prev;
    new_node->prev->next = new_node;
    node_from_pos->prev = new_node;
    ++size_;
  }

  iterator begin() { return fakeNode_.next; }

  iterator end() { return &fakeNode_; }

  void push_back(const_reference value_) {
    InsertNodeBefore(end(), value_);
    return;
  }

  void push_back(value_type &&value_) {
    InsertNodeBefore(end(), std::move(value_));
  }

 private:
  node_alloc alloc_;
  size_type size_;
};

#endif  // CONTAINERS_LIB_LIST_H
