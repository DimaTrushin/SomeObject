#include "Test.h"
#include "SomeObject.h"
#include "SomePrint.h"

template<class>
class TD;


class R {
public:
  R() {
    std::cout << "R()" << std::endl;
  }
  R(int) {
    std::cout << "R(int)" << std::endl;
  }
  ~R() {
    std::cout << "~R()" << std::endl;
  }
  R(const R&) {
    std::cout << "R(constR&)" << std::endl;

  }
  R(R&&) noexcept {
    std::cout << "R(R&&)" << std::endl;
  }
  R& operator=(const R&) {
    std::cout << "=(constR&)" << std::endl;
    return *this;
  }
  R& operator=(R&&) noexcept {
    std::cout << "=(R&&)" << std::endl;
    return *this;
  }
};


void test1() {
  using MyVariant = std::variant<int, double, char>;

  MyVariant x(std::in_place_type<int>, 1);
  std::cout << "x = " << std::get<int>(x) << std::endl;

  std::cout << "x.index = " << x.index() << std::endl;

  x.emplace<double>(1.1);
  std::cout << "x = " << std::get<double>(x) << std::endl;
  std::cout << "x.index = " << x.index() << std::endl;

  std::variant_alternative_t<2, MyVariant> y('d');
  std::cout << "y = " << y << std::endl;
}


void test2() {
  using MySomeObject = CSomeObject<int, double, char, R>;
  MySomeObject a;
  MySomeObject b(1);
  const MySomeObject c('c');
  MySomeObject d(std::in_place_type<R>, 1);
  std::cout << std::endl;


  a.clear();
  std::cout << "b.isDefined() = " << b.isDefined() << std::endl;
  b.clear();
  std::cout << "b.isDefined() = " << b.isDefined() << std::endl;

  std::cout << std::endl;
}


void test3() {
  using TestSomeObject = CSomeObject<R>;
  TestSomeObject r1(1);
  TestSomeObject r2;

  std::cout << std::endl << "r2 = r1" << std::endl;
  r2 = r1;

  TestSomeObject r3;
  std::cout << std::endl << "r3 = std::move(r1)" << std::endl;
  r3 = std::move(r1);

  TestSomeObject r4;
  std::cout << std::endl << "r4 = R()" << std::endl;
  r4 = R();

  std::cout << std::endl << "RestSomeObject r5(r1)" << std::endl;
  TestSomeObject r5(r1);

  std::cout << std::endl << "RestSomeObject r6(std::move(r1))" << std::endl;
  TestSomeObject r6(std::move(r1));

  std::cout << std::endl;
}


void test4() {
  using MySomeObject = CSomePrint<int, double, char>;
  using MySomeObjectBase = CSomeObject<int, double, char>;
  MySomeObject a;
  MySomeObject b(1);
  const MySomeObject c('c');
  const MySomeObject d(3.4);
  std::cout << std::endl;

  a.print();
  b.print();
  c.print();
  d.print();

  a.clear();
  std::cout << "b.isDefined() = " << b.isDefined() << std::endl;
  b.clear();
  std::cout << "b.isDefined() = " << b.isDefined() << std::endl;

  MySomeObjectBase v(b);

  std::cout << std::endl;
}


void test_all() {
  test1();
  test2();
  test3();
  test4();
}
