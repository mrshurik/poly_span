# Polymorphic span

A tiny header-only C++ library extending std::span concept for polymorphic object types. 

## Motivation

```C++
void doWork(std::span<const int> myData);
```

Thanks to ```std::span``` the function can take input stored in different containers, "storage" and "access" aspects of the input data are decoupled and we get a clean interface.

C++ is an object-oriented programming language and you might want to use objects, not just primitive types. Imagine you have classes ```Cat``` and ```Dog``` both are derived from ```Animal``` and can be stored as, say, ```std::vector<Cat>``` and ```std::array<Dog, 5>```. Now you want to define a function doing processing common to ```Animal```. Even with C++20 you're forced to use templates and write something super-generic like this:

```C++
template <typename InputIterator>
int countHungryAnimals(InputIterator animalBegin, InputIterator animalEnd);
```

With C++20 it can be done nicer using Ranges and Concepts, but still it requires templates and so the code can't be moved to a .cpp file, which causes drawbacks such as slow compilation.

Can we make it better? Yes, with ```poly_span``` aka polymorphic span:

```C++
#include "span/poly_span.h"

int countHungryAnimals(span::poly_span<const Animal> animals);
```

Similar to ```std::span``` and iterators it's flexible to how the objects are stored, plus it's very explicit and requires no templates. Now the function will take ```std::vector, std::array, gsl::span```, C array, iterators and pointers of types derived from ```Animal```. Cool, isn't?

In comparison to another possible non-templated argument type ```std::span<const Animal* const>``` there is no overhead for construction of an intermediate buffer with pointers required in this case.

## How to build and use

It's a header-only library, it requires C++11, but will work with newer standards as well. Just add ```include``` dir to your project's include search path.

If you want to build and run the tests please do:

```bash
git clone ... your_dir
cd your_dir && mkdir build && cd build
cmake .. && cmake --build .
ctest
```

## License

This project is licensed under the [MIT License](LICENSE.txt)

## Author

Dr. Alexander Bulovyatov.

Please share your opinion and ideas to bulovyatov AT g m a i l . c o m
