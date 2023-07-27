#if !defined(CONTAINERS_LIB_ARRAY_H)
#define CONTAINERS_LIB_ARRAY_H
#include <stdexcept>

namespace dizing {

// Helper structs.
template <typename T, std::size_t size_>
struct array_traits {
  using array_type = T[size_];
  constexpr static T &get_ref(const array_type arr_, std::size_t i) {
    return const_cast<T &>(arr_[i]);
  }
  constexpr static T &get_ref(const array_type arr_) {
    return const_cast<T &>(arr_[0]);
  }
  constexpr static T *get_pointer(const array_type arr_, std::size_t i) {
    return const_cast<T *>(arr_ + i);
  }
  constexpr static T *get_pointer(const array_type arr_) {
    return const_cast<T *>(arr_);
  }
};

template <typename T>
struct array_traits<T, 0> {
  // Mock type and mock functions for zero-sized array
  struct array_type {};
  constexpr static T &get_ref(const array_type, std::size_t) {
    return *static_cast<T *>("UB");
  }
  constexpr static T &get_ref(const array_type) {
    return *static_cast<T *>("UB");
  }
  constexpr static T *get_pointer(const array_type, std::size_t) {
    return nullptr;
  }
  constexpr static T *get_pointer(const array_type) { return nullptr; }
};

template <typename T, std::size_t size_>
class array {
 public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = value_type *;
  using const_iterator = const value_type *;
  using size_type = std::size_t;

  // Helper type for support zero sized arrays.
  using helper = array_traits<value_type, size_>;
  using array_type = typename helper::array_type;

  // С array. Or empty mock if zero-sized.
  array_type arr_;

  // No explicit construct/copy/destroy for aggregate type.

  // Iterators.
  constexpr iterator begin() noexcept { return helper::get_pointer(arr_); }
  constexpr const_iterator begin() const noexcept {
    return helper::get_pointer(arr_);
  }
  constexpr iterator end() noexcept { return helper::get_pointer(arr_, size_); }
  constexpr const_iterator end() const noexcept {
    return helper::get_pointer(arr_, size_);
  }

  // Element access.
  constexpr reference operator[](size_type pos) noexcept {
    return helper::get_ref(arr_, pos);
  }
  constexpr const_reference operator[](size_type pos) const noexcept {
    return helper::get_ref(arr_, pos);
  }
  constexpr reference at(size_type pos) {
    return (pos < size_) ? helper::get_ref(arr_, pos)
                         : throw std::out_of_range("Position out of range");
  }
  constexpr const_reference at(size_type pos) const {
    return (pos < size_) ? helper::get_ref(arr_, pos)
                         : throw std::out_of_range("Position out of range");
  }
  constexpr const_reference front() const { return helper::get_ref(arr_); }
  constexpr const_reference back() const {
    return helper::get_ref(arr_, size_ - 1);
  }
  constexpr iterator data() { return helper::get_pointer(arr_); }
  constexpr const_iterator data() const { return helper::get_pointer(arr_); }

  // Capacity.
  constexpr size_type size() const noexcept { return size_; }
  constexpr size_type max_size() const noexcept { return size_; }
  constexpr bool empty() const noexcept { return size_ == 0; }

  // Modifiers
  void swap(array &other) {
    static_assert(std::is_swappable_v<value_type>);
    std::swap_ranges(begin(), end(), other.begin());
  }
  void fill(const_reference value) { std::fill(begin(), end(), value); }
};

// Type deduction (Class template user-defined deduction guide)
// For example guide array = {1, 2} to array<int, 2> = {1, 2}
template <typename T, typename... Targs>
array(T, Targs...) -> array<T, sizeof...(Targs) + 1>;

}  // namespace dizing

#endif  // CONTAINERS_LIB_ARRAY_H