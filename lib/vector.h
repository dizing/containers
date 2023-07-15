#if !defined(CONTAINERS_LIB_VECTOR_H)
#define CONTAINERS_LIB_VECTOR_H

#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using alloc_traits = std::allocator_traits<Allocator>;

  vector() : data_(nullptr), capacity_(0), size_(0), alloc_(Allocator()) {}

  void reserve(size_type new_capacity) {
    if (new_capacity > capacity_) {
      pointer new_data = alloc_traits::allocate(alloc_, new_capacity);
      for (size_type i = 0; i < size_; ++i) {
        try {
          alloc_traits::construct(alloc_, new_data + i, std::move(data_[i]));
        } catch (...) {
          for (size_type j = 0; j < i; ++j) {
            alloc_traits::destroy(alloc_, new_data + j);
          }
          alloc_traits::deallocate(alloc_, new_data, new_capacity);
          throw;
        }
      }
      for (size_type i = 0; i < size_; ++i) {
        alloc_traits::destroy(alloc_, data_ + i);
      }
      alloc_traits::deallocate(alloc_, data_, capacity_);
      data_ = new_data;
      capacity_ = new_capacity;
    }
  }

 private:
  pointer data_;
  size_type capacity_;
  size_type size_;
  Allocator alloc_;
};

#endif  // CONTAINERS_LIB_VECTOR_H