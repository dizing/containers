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
    
  node_pointer CreateNodeByCopy(const_reference value) {
    node_pointer temp = std::allocator_traits<node_alloc>::allocate(alloc_, 1);
    std::allocator_traits<node_alloc>::construct(
        alloc_, temp, value);
    return temp;
  }

 private:
  node_alloc alloc_;
};

#endif  // CONTAINERS_LIB_LIST_H
