#if !defined(CONTAINERS_LIB_QUEUE_H)
#define CONTAINERS_LIB_QUEUE_H

#include "list.h"

namespace dizing {

template <typename T, typename Base = dizing::list<T>>
class queue {
 public:
  using value_type = typename Base::value_type;
  using reference = typename Base::reference;
  using const_reference = typename Base::const_reference;
  using size_type = typename Base::size_type;

  queue() : base_container_(Base()){};
  template <typename Container>
  explicit queue(const Container& cont) : base_container_(cont){};
  template <typename Container>
  explicit queue(Container&& cont) : base_container_(std::move(cont)){};
  queue(std::initializer_list<value_type> const& items)
      : base_container_(Base(items)){};

  // ELEMENT ACCESS
  const_reference front() const { return base_container_.front(); }
  const_reference back() const { return base_container_.back(); }

  // CAPACITY
  bool empty() const { return size() == 0; }
  size_type size() const { return base_container_.size(); }

  // MODIFIERS
  void push(const_reference value) { base_container_.push_back(value); }
  void push(value_type&& value) { base_container_.push_back(std::move(value)); }
  void pop() { base_container_.pop_front(); }
  void swap(queue& other) { base_container_.swap(other.base_container_); }

 private:
  Base base_container_;
};

template <typename Container>
queue(Container) -> queue<typename Container::value_type, Container>;

}  // namespace dizing

#endif  // CONTAINERS_LIB_QUEUE_H
