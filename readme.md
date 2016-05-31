# overload_cast for C++14

This is a small utility which returns the address of an overloaded function.
It does not present a new solution, just a nicer syntax for an existing language mechanism.


## Casting overloaded functions

Normally, if we have a template taking a function,

```c++
template<class F>
void take_function(F f) { /* does something with f */ }
```

it's not possible to pass an overloaded function:

```c++
int foo(int x);
float foo(float x);

take_function(foo); // error: couldn't infer template argument 'F'
```

The name `foo` refers to an overload set so the call is ambiguous.
This can be resolved by casting to a specific overload:

```c++
take_function(static_cast<int (*)(int)>(foo));
```

However, specifying the return type is actually completely unnecessary since this information does not participate in overload resolution.
Together with the star and two sets of parentheses it just looks messy.
This is where `overload_cast` comes in with a cleaned up syntax.
Only the parameter type needs to be mentioned:

```c++
#include "overload_cast.hpp"

take_function(overload_cast<int>(foo));
```

Any number of arguments can be specified. The following casts are equivalent:

```c++
static_cast<int (*)(int, float, bool)>(bar);
overload_cast<int, float, bool>(bar);
```


## Member functions

Given a set of overloaded member functions,

```c++
struct Widget {
    int foo(int x, int y);
    float foo(float x, float y);

    int foo(int x);
    int foo(int x) const;
};
```

the cast works as follows:

```c++
static_cast<int (Widget::*)(int, int)>(&Widget::foo);
overload_cast<int, int>(&Widget::foo);
```

With `overload_cast` there is no need to mention the class name twice.
The selection can also be made based on constness:

```c++
// select non-const overload
static_cast<int (Widget::*)(int)>(&Widget::foo);
overload_cast<int>(&Widget::foo);

// select const overload
static_cast<int (Widget::*)(int) const>(&Widget::foo);
overload_cast<int>(&Widget::foo, const_);
```


## Usage in C++11

C++14 is the primary target.
It can still be used in C++11 with a small modification:

```c++
overload_cast<int>(foo); // C++14
overload_<int>{}(foo);   // C++11
```

Note the `{}` in the C++11 version.
The C++14 implementation uses variable templates to get the nicer syntax.
Take a look at the header file for details.


## Usefulness

Probably none.
Still, it was a personal curiosity.

For something more interesting, take a look at this C++ standard proposal which aims to allow using overload sets as function arguments:  [P0119R1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0119r1.pdf).
