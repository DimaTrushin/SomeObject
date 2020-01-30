#ifndef SOMEPRINT_H
#define SOMEPRINT_H

#include "SomeObject.h"
#include <iostream>

template<class... TObjects>
class CSomePrint : public CSomeObject<TObjects...> {
  using CBase = CSomeObject<TObjects...>;
public:
  using CBase::CBase;

  void print() const {
    CBase::call([](auto&& arg) {
      if constexpr(CBase::isOneOf<decltype(arg), int, char>())
        std::cout << "print int or char() = " << arg << std::endl;
      else
        std::cout << "print other() = " << arg << std::endl;
    },[](auto&&) {
      std::cout << "empty" << std::endl;
    });
  }
};

#endif // SOMEPRINT_H
