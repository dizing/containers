#if !defined(CONTAINERS_LIB_LIST_H)
#define CONTAINERS_LIB_LIST_H

#include <iostream>
#include <memory>

namespace dizing {

namespace list_internal {

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
class ListIterator {
 public:
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

  ListIterator(const ListIterator &other) : node_(other.node_) {}

  // Non const to const
  template <
      bool otherIsConst,
      std::enable_if_t<isConst == true && otherIsConst == false, bool> = true>
  ListIterator(const ListIterator<T, otherIsConst> &other)
      : node_(other.node_) {}

  ListIterator<T, isConst> &operator++() {
    node_ = node_->next_;
    return *this;
  }
  ListIterator<T, isConst> operator++(int) {
    ListIterator<T, isConst> temp(*this);
    ++(*this);
    return temp;
  }
  ListIterator<T, isConst> &operator--() {
    node_ = node_->prev_;
    return *this;
  }
  ListIterator<T, isConst> operator--(int) {
    ListIterator<T, isConst> temp(*this);
    --(*this);
    return temp;
  }
  ListIterator &operator=(const ListIterator &other) {
    node_ = other.node_;
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

}  // namespace list_internal

template <typename T, typename Allocator = std::allocator<T>>
class list {
 public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = list_internal::ListIterator<value_type, false>;
  using const_iterator = list_internal::ListIterator<value_type, true>;
  using size_type = std::size_t;
  using base_node = list_internal::BaseListNode;
  using node = list_internal::ListNode<value_type>;
  using value_traits = std::allocator_traits<Allocator>;
  using node_alloc = typename value_traits::rebind_alloc<node>;
  using node_traits = typename value_traits::rebind_traits<node>;

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

  list &operator=(const list &l) {
    if (this != &l) {
      list(l).swap(*this);
    }
    return *this;
  }

  list &operator=(list &&l) {
    list temp(std::move(l));
    swap(temp);
    return *this;
  }

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
  iterator insert(const_iterator pos, const_reference value) {
    InsertNodeBefore(IteratorConstCast(pos), value);
    return --IteratorConstCast(pos);
  }
  iterator insert(const_iterator pos, value_type &&value) {
    InsertNodeBefore(IteratorConstCast(pos), std::move(value));
    return --IteratorConstCast(pos);
  }
  iterator erase(const_iterator pos) {
    return erase(pos, const_iterator(pos.GetNode()->next_));
  }
  iterator erase(const_iterator first, const_iterator last) {
    while (first != last) {
      EraseNode(IteratorConstCast(first++));
    }
    return IteratorConstCast(last);
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

  void merge(list &other) { merge(std::move(other)); }

  void merge(list &&other) {
    auto comparator = [](const T &a, const T &b) -> bool { return a < b; };
    merge<decltype(comparator)>(std::move(other), comparator);
  }

  template <class Compare>
  void merge(list &other, Compare comp) {
    merge<Compare>(std::move(other), comp);
  }
  // Merge two lists.
  // No elements are copied
  // Basic exception safety(not all other list elements will be merged)
  template <class Compare>
  void merge(list &&other, Compare comp) {
    if (&other == this) {
      return;
    }
    iterator first = begin();
    iterator second = other.begin();
    while (first != end() && second != other.end()) {
      if (comp(*second, *first)) {
        TransferNodeBefore(first.GetNode(), second++);
        ++size_;
        --other.size_;
      } else {
        ++first;
      }
    }
    while (second != other.end()) {
      TransferNodeBefore(first.GetNode()->next_, second++);
      ++size_;
      --other.size_;
    }
  }

  void splice(const_iterator pos, list &other) {
    splice(pos, std::move(other));
  }

  void splice(const_iterator pos, list &&other) {
    base_node_pointer pos_node = (IteratorConstCast(pos).GetNode());
    base_node_pointer other_first_node = other.begin().GetNode();
    base_node_pointer other_last_node = (--other.end()).GetNode();
    other_first_node->prev_ = pos_node->prev_;
    pos_node->prev_->next_ = other_first_node;
    other_last_node->next_ = pos_node;
    pos_node->prev_ = other_last_node;
    size_ += other.size_;
    other.fakeNode_ = {&other.fakeNode_, &other.fakeNode_};
    other.size_ = 0;
  }

  void reverse() {
    iterator it = begin();
    while (it != end()) {
      base_node_pointer node = (it++).GetNode();
      base_node_pointer tmp = node->next_;
      node->next_ = node->prev_;
      node->prev_ = tmp;
    }
    base_node_pointer tmp = fakeNode_.next_;
    fakeNode_.next_ = fakeNode_.prev_;
    fakeNode_.prev_ = tmp;
  }

  void clear() { erase(begin(), end()); }

  void sort() {
    auto comparator = [](const T &a, const T &b) -> bool { return a < b; };
    sort<decltype(comparator)>(comparator);
  }

  // Sorting by selection sort O(n^2)
  template <typename Compare>
  void sort(Compare comp) {
    SelectionSort<Compare>(comp);
  }

  void unique() {
    auto predicate = [](const T &a, const T &b) -> bool { return a == b; };
    unique<decltype(predicate)>(predicate);
  }

  template <class BinaryPredicate>
  void unique(BinaryPredicate p) {
    iterator first = begin();
    iterator second = ++begin();
    while (second != end())
      if (p(*first, *second)) {
        EraseNode(second++);
      } else {
        first = second++;
      }
  }

  template <class... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    static_assert(
        (std::is_same_v<value_type, std::remove_reference_t<decltype(args)>> &&
         ...));
    (insert(pos, std::forward<Args>(args)), ...);
    return IteratorConstCast(pos);
  }
  template <class... Args>
  void insert_many_back(Args &&...args) {
    insert_many(end(), args...);
  }
  template <class... Args>
  void insert_many_front(Args &&...args) {
    insert_many(begin(), args...);
  }

 private:
  using node_pointer = node *;
  using base_node_pointer = base_node *;

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
  // Unhook node and place it before pos
  void TransferNodeBefore(iterator pos, iterator node_for_steal) {
    base_node_pointer tmp = node_for_steal.GetNode();
    tmp->Unhook();
    tmp->HookBefore(pos.GetNode());
  }

  // Const cast from const iterator to non-const iterator
  iterator IteratorConstCast(const_iterator pos) const {
    iterator temp(
        const_cast<typename iterator::base_node_pointer>(pos.GetNode()));
    return temp;
  }

  template <class Compare>
  void SelectionSort(Compare comp) {
    list temp;
    while (size_ > 0) {
      iterator min_element(begin());
      for (auto it = begin(); it != end(); ++it) {
        min_element = (comp(*it, *min_element)) ? it : min_element;
      }
      temp.TransferNodeBefore(temp.end(), min_element);
      --size_;
      ++temp.size_;
    }
    swap(temp);
  }
};

// Deduction guide for initializer list constructor
// It depends on compiler, will it be created by default
template <typename T>
list(std::initializer_list<T>) -> list<T>;

}  // namespace dizing

#endif  // CONTAINERS_LIB_LIST_H
