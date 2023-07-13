#if !defined(CONTAINERS_LIB_LIST_H)
#define CONTAINERS_LIB_LIST_H

#include <memory>

struct BaseListNode {
  BaseListNode *prev_;
  BaseListNode *next_;

  BaseListNode() = default;
  BaseListNode(const BaseListNode &) = delete;

  void HookBefore(BaseListNode *other) noexcept {
    next_ = other;
    prev_ = other->prev_;
    prev_->next_ = this;
    other->prev_ = this;
  }
  void Unhook() noexcept {
    prev_->next_ = next_;
    next_->prev_ = prev_;
    prev_ = this;
    next_ = this;
  }
  static void swap(BaseListNode &lhs, BaseListNode &rhs) {
    BaseListNode *lhs_next = lhs.next_;
    BaseListNode *rhs_next = rhs.next_;
    lhs.Unhook();
    rhs.Unhook();
    if (rhs_next != &rhs) {
      lhs.HookBefore(rhs_next);
    };
    if (lhs_next != &lhs) {
      rhs.HookBefore(lhs_next);
    };
  }
};

template <typename T>
struct ListNode : public BaseListNode {
  T value_;

  // ListNode(const T &value) : value_(value) {}
  // ListNode(T &&value) : value_(std::move(value)) {}
  // template <typename... Args>
  // ListNode(Args &&...value) : value_(T(std::forward<Args>(value)...)) {}
};

template <typename T, bool isConst>
struct ListIterator {
  using value_type = T;
  using reference =
      typename std::conditional_t<isConst, const value_type &, value_type &>;
  using pointer =
      typename std::conditional_t<isConst, const value_type *, value_type *>;
  using base_node_pointer =
      typename std::conditional_t<isConst, const BaseListNode *,
                                  BaseListNode *>;
  using node_pointer =
      typename std::conditional_t<isConst, const ListNode<value_type> *,
                                  ListNode<value_type> *>;
  base_node_pointer node_;

  ListIterator(base_node_pointer node) : node_(node) {}

  ListIterator<T, isConst> &operator++() {
    node_ = node_->next_;
    return *this;
  }
  ListIterator<T, isConst> &operator--() {
    node_ = node_->prev_;
    return *this;
  }

  reference operator*() const {
    return static_cast<node_pointer>(node_)->value_;
  }
  base_node_pointer GetNode() const noexcept { return node_; }
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
  using value_traits = std::allocator_traits<Allocator>;
  using node_alloc = typename value_traits::rebind_alloc<
      node>;  // typename Allocator::rebind<node>::other;
  using node_traits = typename value_traits::rebind_traits<node>;
  using node_pointer = node *;
  using base_node_pointer = base_node *;

  // Constructors

  list()
      : node_alloc_(node_alloc()),
        val_alloc_(Allocator()),
        size_(0),
        fakeNode_({&fakeNode_, &fakeNode_}) {}

  explicit list(size_type n, const Allocator &alloc = Allocator())
      : node_alloc_(node_alloc()),
        val_alloc_(alloc),
        size_(0),
        fakeNode_({&fakeNode_, &fakeNode_}) {
    for (size_type i = 0; i < n; ++i) {
      emplace_back();
    }
  }

  list(const std::initializer_list<value_type> &items)
      : node_alloc_(node_alloc()),
        val_alloc_(Allocator()),
        size_(0),
        fakeNode_({&fakeNode_, &fakeNode_}) {
    try {
      for (auto &element : items) {
        push_back(element);
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  list(const list &other)
      : node_alloc_(other.node_alloc_),
        val_alloc_(other.val_alloc_),
        size_(0),
        fakeNode_({&fakeNode_, &fakeNode_}) {
    try {
      for (auto &element : other) {
        push_back(element);
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  list(list &&other)
      : node_alloc_(std::move(other.node_alloc_)),
        val_alloc_(std::move(other.val_alloc_)),
        size_(other.size_),
        fakeNode_({&fakeNode_, &fakeNode_}) {
    if (other.size_ > 0) {
      fakeNode_.HookBefore(&other.fakeNode_);
      other.fakeNode_.Unhook();
      other.size_ = 0;
    }
  }

  ~list() { clear(); }

  // Element Access
  const_reference front() const { return *begin(); }

  const_reference back() const { return *(--end()); }

  // Iterators

  iterator begin() { return fakeNode_.next_; }
  iterator end() { return &fakeNode_; }
  const_iterator begin() const { return fakeNode_.next_; }
  const_iterator end() const { return &fakeNode_; }

  // Capacity

  bool empty() const { return size() == 0; }
  size_type size() const { return size_; }
  size_type max_size() const { return node_traits::max_size(node_alloc_); }

  // Modifiers
  iterator insert(iterator pos, const_reference value) {
    InsertNodeBefore(pos, value);
    return --pos;
  };
  iterator insert(iterator pos, value_type &&value) {
    InsertNodeBefore(pos, std::move(value));
    return --pos;
  }
  iterator erase(iterator pos) {
    return erase(pos, iterator(pos.GetNode()->next_));
  }
  iterator erase(iterator first, iterator last) {
    iterator tmp = first;
    while (first != last) {
      tmp = iterator(first.GetNode()->next_);  // iterator++
      EraseNode(first);
      first = tmp;
    }
    return last;
  }

  void push_back(const_reference value) { InsertNodeBefore(end(), value); }

  void push_back(value_type &&value) {
    InsertNodeBefore(end(), std::move(value));
  }

  void pop_back() { EraseNode(--end()); }

  template <typename... Args>
  void emplace_back(Args &&...value) {
    InsertNodeBefore(end(), std::forward<Args>(value)...);
  }

  void push_front(const_reference value) { InsertNodeBefore(begin(), value); }

  void push_front(value_type &&value) {
    InsertNodeBefore(begin(), std::move(value));
  }

  void pop_front() { EraseNode(begin()); }

  void swap(list &other) {
    // node_alloc node_alloc_tmp = node_alloc_;
    // node_alloc_ = other.node_alloc_;
    // other.node_alloc_ = node_alloc_tmp;
    std::swap(node_alloc_, other.node_alloc_);
    std::swap(val_alloc_, other.val_alloc_);
    std::swap(size_, other.size_);
    base_node::swap(fakeNode_, other.fakeNode_);
  }

  void clear() { erase(begin(), end()); }

 private:
  node_alloc node_alloc_;
  Allocator val_alloc_;
  size_type size_;
  base_node fakeNode_;

  // Creates List Node with value_type element inside.
  // Receives universal reference parameter pack.
  // It can be lvalue value_type, rvalue value_type, or pack of arguments to
  // value_type constructor.
  template <typename... Args>
  node_pointer CreateNode(Args &&...value) {
    node_pointer temp = node_traits::allocate(node_alloc_, 1);
    try {
      // node_traits::construct(node_alloc_, temp,
      // std::forward<Args>(value)...);
      value_traits::construct(val_alloc_, &(temp->value_),
                              std::forward<Args>(value)...);
    } catch (...) {
      node_traits::deallocate(node_alloc_, temp, 1);
      throw;
    }
    return temp;
  }
  // Remove node on pos from list.
  // Destroy value_type field and deallocate node
  void EraseNode(iterator pos) {
    node_pointer node = static_cast<node_pointer>(pos.GetNode());
    node->Unhook();
    value_traits::destroy(val_alloc_, &(node->value_));
    node_traits::deallocate(node_alloc_, node, 1);
    --size_;
  }
  // Create and add node in the list before another node
  // Receives the iterator on element before which the new one will be created
  // and parameter pack for CreateNode
  template <typename... Args>
  void InsertNodeBefore(iterator pos, Args &&...value) {
    node_pointer new_node = CreateNode(std::forward<Args>(value)...);
    base_node_pointer node_from_pos = pos.GetNode();
    new_node->HookBefore(node_from_pos);
    ++size_;
  }
};

// Deduction guide for initializer list constructor
// It depends on compiler, will it be created by default
template <typename T>
list(std::initializer_list<T>) -> list<T>;

#endif  // CONTAINERS_LIB_LIST_H
