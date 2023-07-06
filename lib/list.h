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

  ListNode(const T& value) : value_(value) {
  }
  ListNode(T&& value) : value_(std::move(value)) {
  }
  template <typename... Args>
  ListNode(Args&&... value) : value_(T(std::forward<Args>(value)...)){
  }
  
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
  base_node fakeNode_ = {nullptr, nullptr};

  list() : alloc_(node_alloc()) {}

  template <typename... Args>  
  node_pointer CreateNode(Args&&... value) {
    node_pointer temp = std::allocator_traits<node_alloc>::allocate(alloc_, 1);
    std::allocator_traits<node_alloc>::construct(
        alloc_, temp, std::forward<Args>(value)...);
    return temp;
  }
  template <typename... Args>  
  void InsertNode(node_pointer pos, Args&&... value) {
    node_pointer new_node = CreateNode(std::forward<Args>(value)...);
    pos->next = new_node;
    new_node->prev = pos;
  }

 private:
  node_alloc alloc_;
};

#endif  // CONTAINERS_LIB_LIST_H
