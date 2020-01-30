#ifndef SOMEOBJECT_H
#define SOMEOBJECT_H

#include <variant>
#include <functional>

//---------------------------------------------------------------------------
// How To Use
//---------------------------------------------------------------------------
//
// Description
// CSomeObject allows you to store an object of a class from a list of
// different classes. It does not allocate any memory on the heap.
// The basic template CSomeObject only stores data without any access
// to the data. You can extend the interface of the class by adding
// new methods to its interface and even control the behaviour depending
// on the type currently stored in the class. CSomeObject can be in an empty
// state.
//
// CSomeObject has value semantics. Since it does not allocate, its move
// operations are as bad as the worst one among the types provided.
//
// Currently CSomeObject uses std::variant inside. Hence it has all
// its restrictions on the stored classses.
//
// 1) Publicly inherit from CSomeObject. Open all its constructors and
// provide public methods in the interface.
//
// template<class... TObjects>
// class CSomePrint : public CSomeObject<TObjects...> {
//   using CBase = CSomeObject<TObjects...>;
// public:
//   using CBase::CBase;
//   void print() const {
//     CBase::call([](auto&& arg) {
//       if constexpr(CBase::isOneOf<decltype(arg), int, char>())
//         std::cout << "print int or char() = " << arg << std::endl;
//       else
//         std::cout << "print other() = " << arg << std::endl;
//     },[](auto&&) {
//       std::cout << "empty" << std::endl;
//     });
//   }
// };
//
// 2) The pattern to implement a method is this
//
//   void method_name const {
//     CBase::call([](auto&& arg) {
//       // implementation for each non empty case
//     },[](auto&&) {
//       // implementation for the empty case
//     });
//   }
//
// If you want doing nothing in the empty case, then just use this version
//
//   void method_name const {
//     CBase::call([](auto&& arg) {
//       // implementation for non empty case
//     });
//   }
//
// You could implement the print method simply by
//
//   void print() const {
//     CBase::call([](auto&& arg) {
//       std::cout << "print() = " << arg << std::endl;
//     });
//   }
//
// In case you need different behaviour for different types you are storing
// use if constexpr, e.g.,
//
// if constexpr (CBase::isOneOf<decltype(arg), int>())
//
// is used to check if the current type is int or you may use several types
//
// if constexpr(CBase::isOneOf<decltype(arg), int, char>())
//
//---------------------------------------------------------------------------

template<class... TObjects>
class CSomeObject {
protected:
  struct CEmpty {};
private:
  // The first type is used in the default constructor of variant
  // it is used as a flag for an empty CSomeObject
  using CStorage = std::variant<CEmpty, TObjects...>;
  template<class T>
  using CEnableIfConvertible = std::enable_if_t<std::is_convertible_v<T, CStorage>>;
public:
  CSomeObject() = default;

  template<class T, class = CEnableIfConvertible<T>>
  CSomeObject(T&& Object) : Object_(std::forward<T>(Object)) {}

  template<class T, class... TArgs>
  CSomeObject(std::in_place_type_t<T>, TArgs&& ... args)
    : Object_(std::in_place_type<T>, std::forward<TArgs>(args)...) {}

  bool isDefined() const {
    return Object_.index() != 0 && Object_.index() != std::variant_npos;
  }

  void clear() {
    Object_.template emplace<0>();
  }

protected:
  template<class T>
  constexpr static bool isOneOf() {
    return false;
  }

  template<class T, class T1, class... Ts>
  constexpr static bool isOneOf() {
    using T_ = std::decay_t<T>;
    return (std::is_same_v<T_, T1> ? true : isOneOf<T, Ts...>());
  }

  template<class TArg, class TFunction>
  static auto filter(TArg&& arg, TFunction&& function) {
    using CArg_ = std::decay_t<TArg>;
    if constexpr(!std::is_same_v<CArg_, CEmpty>) {
      return std::invoke(std::forward<TFunction>(function),
                         std::forward<TArg>(arg));
    } else {
      // Need this to suppress the unused warnings in case
      // there is no code in the function if the type of the argument is CEmpty
      (void)function;
      (void)arg;
    }
  }

  template<class TArg, class TFunction1, class TFunction2>
  static auto filter(TArg&& arg, TFunction1&& function1, TFunction2&& function2) {
    using CArg_ = std::decay_t<TArg>;
    if constexpr(!std::is_same_v<CArg_, CEmpty>) {
      (void)function2;
      return std::invoke(std::forward<TFunction1>(function1),
                         std::forward<TArg>(arg));
    } else {
      return std::invoke(std::forward<TFunction2>(function2),
                         std::forward<TArg>(arg));
      // Need this to suppress the unused warnings in case
      // there is no code in the function if the type of the argument is CEmpty
      (void)function1;
    }
  }

  // There must be a way to provide an interface
  // I am not sure that this is the best solution
  template<class TFunction>
  void call(TFunction&& function) {
    std::visit([&function, this](auto&& arg) {
      return filter(std::forward<decltype(arg)>(arg), // should be the same as static_cast<decltype(arg)&&>
                    std::forward<TFunction>(function));
    }, Object_);
  }

  template<class TFunction1>
  void call(TFunction1&& function1) const {
    std::visit([&function1, this](auto&& arg) {
      return filter(std::forward<decltype(arg)>(arg), // should be the same as static_cast<decltype(arg)&&>
                    std::forward<TFunction1>(function1));
    }, Object_);
  }

  template<class TFunction1, class TFunction2>
  void call(TFunction1&& function1, TFunction2&& function2) {
    std::visit([&function1, &function2, this](auto&& arg) {
      return filter(std::forward<decltype(arg)>(arg), // should be the same as static_cast<decltype(arg)&&>
                    std::forward<TFunction1>(function1),
                    std::forward<TFunction2>(function2));
    }, Object_);
  }

  template<class TFunction1, class TFunction2>
  void call(TFunction1&& function1, TFunction2&& function2) const {
    std::visit([&function1, &function2, this](auto&& arg) {
      return filter(std::forward<decltype(arg)>(arg), // should be the same as static_cast<decltype(arg)&&>
                    std::forward<TFunction1>(function1),
                    std::forward<TFunction2>(function2));
    }, Object_);
  }

private:
  CStorage Object_;
};

#endif // SOMEOBJECT_H
