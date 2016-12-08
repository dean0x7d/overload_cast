// Copyright (c) 2016, Dean Moldovan
// License: simplified BSD, see the license.md file

#include "overload_cast.hpp"
#include "resolve.hpp"

template<class T> constexpr bool expected(T) { return true; }
template<class> constexpr bool expected(...) { return false; }

namespace test_pointer_to_function {
    void foo();
    int foo(float);
    void foo(int);
    float foo(int, float);

    // overload_cast
    static_assert(expected<void (*)()>(overload_cast<>(foo)), "");
    static_assert(expected<int (*)(float)>(overload_cast<float>(foo)), "");
    static_assert(expected<void (*)(int)>(overload_cast<int>(foo)), "");
    static_assert(expected<float (*)(int, float)>(overload_cast<int, float>(foo)), "");

    // resolve
    static_assert(expected<void (*)()>(foo | resolve<>()), "");
    static_assert(expected<int (*)(float)>(foo | resolve<float>()), "");
    static_assert(expected<void (*)(int)>(foo | resolve<int>()), "");
    static_assert(expected<float (*)(int, float)>(foo | resolve<int, float>()), "");
}

namespace test_pointer_to_member_function {
    struct W {
        void bar();
        void bar() const;

        void bar(int);
        void bar(int) const;
        int bar(float);

        float bar(int, float);
        float bar(int, float) const;
    };

    // overload_cast
    static_assert(expected<void (W::*)()>(overload_cast<>(&W::bar)), "");
    static_assert(expected<void (W::*)() const>(overload_cast<>(&W::bar, const_)), "");

    static_assert(expected<void (W::*)(int)>(overload_cast<int>(&W::bar)), "");
    static_assert(expected<void (W::*)(int) const>(overload_cast<int>(&W::bar, const_)), "");
    static_assert(expected<int (W::*)(float)>(overload_cast<float>(&W::bar)), "");

    static_assert(expected<float (W::*)(int, float)>(overload_cast<int, float>(&W::bar)), "");
    static_assert(expected<float (W::*)(int, float) const>(overload_cast<int, float>(&W::bar, const_)), "");

    // resolve
    static_assert(expected<void (W::*)()>(&W::bar | resolve<>()), "");
    static_assert(expected<void (W::*)() const>(&W::bar | resolve_const<>()), "");

    static_assert(expected<void (W::*)(int)>(&W::bar | resolve<int>()), "");
    static_assert(expected<void (W::*)(int) const>(&W::bar | resolve_const<int>()), "");
    static_assert(expected<int (W::*)(float)>(&W::bar | resolve<float>()), "");

    static_assert(expected<float (W::*)(int, float)>(&W::bar | resolve<int, float>()), "");
    static_assert(expected<float (W::*)(int, float) const>(&W::bar | resolve_const<int, float>()), "");
}

namespace test_readme {
    template<class F>
    void take_function(F) { /* does something with f */ }

    int foo(int x) { return x; }
    float foo(float x) { return x; }

    void example1() {
        take_function(static_cast<int (*)(int)>(foo));
        take_function(overload_cast<int>(foo));
    }

    struct Widget {
        int foo(int x, int y) { return x + y; }
        float foo(float x, float y) { return x + y; }

        int foo(int x) { return x; }
        int foo(int x) const { return x; }
    };

    void example2() {
        take_function(static_cast<int (Widget::*)(int, int)>(&Widget::foo));
        take_function(overload_cast<int, int>(&Widget::foo));

        take_function(static_cast<int (Widget::*)(int)>(&Widget::foo));
        take_function(overload_cast<int>(&Widget::foo));

        take_function(static_cast<int (Widget::*)(int) const>(&Widget::foo));
        take_function(overload_cast<int>(&Widget::foo, const_));
    }

    void cpp11() {
        take_function(overload_cast<int>(foo));
        take_function(overload_<int>{}(foo));
    }

    void alternative() {
        take_function(overload_cast<int>(foo));
        take_function(foo | resolve<int>());

        take_function(overload_cast<int>(&Widget::foo));
        take_function(&Widget::foo | resolve<int>());

        take_function(overload_cast<int>(&Widget::foo, const_));
        take_function(&Widget::foo | resolve_const<int>());
    }
}

int main() {}
