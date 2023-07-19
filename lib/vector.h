#if !defined(CONTAINERS_LIB_VECTOR_H)
#define CONTAINERS_LIB_VECTOR_H

#include <iterator>
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

  template <typename Iter>
  vector(Iter beg, Iter end) : vector() {
    size_type input_size = static_cast<size_type>(std::distance(beg, end));
    reserve(input_size);
    size_ = input_size;
    for (size_type i = 0; i < input_size; ++i) {
      CreateElement(&data_[i], *beg);
      ++beg;
    }
  }

  vector(std::initializer_list<value_type> const &items)
      : vector(items.begin(), items.end()) {}

  vector(const vector &other) : vector(other.begin(), other.end()) {}

  ~vector() { freeDataArray(data_, capacity_, size_); }

  vector &operator=(const vector &other) {
    if (this != &other) {
      vector(other).swap(*this);
    }
    return *this;
  }

  vector &operator=(vector &&other) {
    if (this != &other) {
      vector(std::move(other)).swap(*this);
    }
    return *this;
  }

  void push_back(T &&value) {
    AutomaticReserveLogic();
    CreateElement(&data_[size_], std::move(value));
    ++size_;
  }

  void push_back(const T &value) {
    AutomaticReserveLogic();
    CreateElement(&data_[size_], value);
    ++size_;
  };

  iterator insert(iterator pos, const_reference value) {
    AutomaticReserveLogic();
    alloc_traits::construct(alloc_, data_ + size_);
    for(iterator i = end(); i > pos; --i) {
      *i = *(i - 1);
    }
    CreateElement(pos, value);
    ++size_;
    return pos;
  }

  void erase(iterator pos) {
    // alloc_traits::destroy(alloc_, pos);
    for(iterator i = pos; i < end() - 1; ++i) {
      *i = *(i + 1);
    }
    --size_;
  }

  void clear() {
    if (data_ != nullptr) {
      for (size_type i = 0; i < size_; ++i) {
        alloc_traits::destroy(alloc_, data_ + i);
      }
    }
    size_ = 0;
  }

  // ELEMENT ACCESS
  reference at(size_type pos) {
    if (!(pos < size_)) {
      throw std::out_of_range(std::to_string(pos) + "not less then" +
                              std::to_string(size_));
    }
    return data_[pos];
  }
  const_reference at(size_type pos) const {
    if (!(pos < size_)) {
      throw std::out_of_range(std::to_string(pos) + "not less then" +
                              std::to_string(size_));
    }
    return data_[pos];
  }
  reference operator[](size_type pos) { return data_[pos]; }
  const_reference operator[](size_type pos) const { return data_[pos]; }
  const_reference front() const { return data_[0]; }
  const_reference back() const { return data_[size_ - 1]; }
  pointer data() { return data_; }

  // ITERATORS
  iterator begin() { return data_; }

  const_iterator begin() const { return data_; };

  iterator end() { return data_ + size_; }

  const_iterator end() const { return data_ + size_; };

  // CAPACITY
  bool empty() { return size_ == 0; };

  size_type size() const { return size_; }

  size_type max_size() { return alloc_traits::max_size(alloc_); }

  void reserve(size_type new_capacity) {
    if (new_capacity > capacity_) {
      pointer new_data = MoveToNewDataArray(new_capacity, size_, data_);
      freeDataArray(data_, capacity_, size_);
      data_ = new_data;
      capacity_ = new_capacity;
    }
  }

  size_type capacity() const { return capacity_; }

  void shrink_to_fit() {
    pointer new_data = MoveToNewDataArray(size_, size_, data_);
    freeDataArray(data_, capacity_, size_);
    data_ = new_data;
    capacity_ = size_;
  };

  void swap(vector &other) {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(alloc_, other.alloc_);
    std::swap(size_, other.size_);
  }

 private:
  pointer data_;
  size_type capacity_;
  size_type size_;
  Allocator alloc_;

  template <typename... Args>
  void CreateElement(pointer pos, Args &&...value) {
    alloc_traits::construct(alloc_, pos, std::forward<Args>(value)...);
  }

  // if new_capacity < element_recreating_count -- UB
  pointer MoveToNewDataArray(size_type new_capacity,
                             size_type element_recreating_count,
                             pointer data_for_moving) {
    pointer new_data = alloc_traits::allocate(alloc_, new_capacity);
    try {
      ConstructElementsFromAnotherData(0, element_recreating_count,
                                       data_for_moving, new_data);
    } catch (...) {
      alloc_traits::deallocate(alloc_, new_data, new_capacity);
      throw;
    }
    return new_data;
  }

  void ConstructElementsFromAnotherData(size_type start_pos, size_type count,
                                        pointer data_from, pointer data_to) {
    for (size_type i = start_pos; i < count; ++i) {
      try {
        alloc_traits::construct(alloc_, data_to + i, data_from[i]);
      } catch (...) {
        for (size_type j = 0; j < i; ++j) {
          alloc_traits::destroy(alloc_, data_to + j);
        }
        throw;
      }
    }
  }

  void freeDataArray(pointer data_array, size_type capacity, size_type size) {
    if (data_array != nullptr) {
      for (size_type i = 0; i < size; ++i) {
        alloc_traits::destroy(alloc_, data_array + i);
      }
      alloc_traits::deallocate(alloc_, data_array, capacity);
    }
  }

  void AutomaticReserveLogic() {
    if (size_ >= capacity_) {
      if (capacity_ == 0) {
        reserve(1);
      } else {
        reserve(capacity_ * 2);
      }
    }
  }
};

// Deduction guide for iterators constructor
template <typename Iter>
vector(Iter beg, Iter end)
    -> vector<typename std::iterator_traits<Iter>::value_type>;

#endif  // CONTAINERS_LIB_VECTOR_H