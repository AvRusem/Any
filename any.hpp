// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#ifndef MY_ANY_HPP
#define MY_ANY_HPP

#include <stdexcept>
#include <memory>

class BadAnyCast : public std::bad_cast {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "BadAnyCast error";
  }
};

class IHolder {
 public:
  virtual IHolder* Clone() = 0;

  virtual const std::type_info& Type() = 0;

  virtual ~IHolder() = default;
};

template <class T>
class AnyHolder : public IHolder {
 public:
  explicit AnyHolder(const T& value) : value_(value) {
  }

  explicit AnyHolder(T&& value) noexcept : value_(std::move(value)) {
  }

  IHolder* Clone() override {
    return new AnyHolder(value_);
  }

  const std::type_info& Type() override {
    return typeid(value_);
  }

  const T& GetValue() {
    return value_;
  }

 private:
  T value_;
};

class Any {
 public:
  Any() : holder_(nullptr) {
  }

  Any(const Any& other) : holder_(other.holder_->Clone()) {
  }

  template <class T>
  explicit Any(const T& value) : holder_(new AnyHolder<T>(value)) {
  }

  Any(Any&& other) noexcept : Any() {
    Swap(other);
  }

  Any& operator=(const Any& other) {
    if (this != &other) {
      holder_.reset(other.holder_->Clone());
    }

    return *this;
  }

  template <class T>
  Any& operator=(const T& value) {
    holder_.reset(new AnyHolder<T>(value));

    return *this;
  }

  Any& operator=(Any&& other) noexcept {
    Swap(other);
    other.holder_ = nullptr;

    return *this;
  }

  void Swap(Any& other) noexcept {
    holder_.swap(other.holder_);
  }

  void Reset() {
    holder_.reset();
  }

  bool HasValue() const {
    return holder_ != nullptr;
  }

  ~Any() {
    holder_.reset();
  }

  template <class T>
  friend T AnyCast(const Any& value);

 private:
  std::unique_ptr<IHolder> holder_ = nullptr;
};

template <class T>
T AnyCast(const Any& value) {
  if (!value.holder_) {
    throw BadAnyCast();
  }

  auto* holder = dynamic_cast<AnyHolder<T>*>(value.holder_.get());
  if (!holder) {
    throw BadAnyCast();
  }

  return holder->GetValue();
}

#endif  // MY_ANY_HPP
