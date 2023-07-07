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

template <typename T, typename Allocator = std::allocator<T>>
class list {
 public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  //   using iterator = value_type *;
  //   using const_iterator = const value_type *;
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
  void InsertNodeBefore(base_node_pointer pos, Args &&...value) {
    node_pointer new_node = CreateNode(std::forward<Args>(value)...);
    new_node->next = pos;
    new_node->prev = pos->prev;
    new_node->prev->next = new_node;
    pos->prev = new_node;
    ++size_;
  }

 private:
  node_alloc alloc_;
  size_type size_;
};

#endif  // CONTAINERS_LIB_LIST_H
