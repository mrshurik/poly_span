# Polymorphic span

A tiny header-only C++ library extending std::span concept for polymorphic object types. Make your interfaces nicer!

## Motivation

```C++
void doWork(std::span<const int> myData);
```

Thanks to ```std::span``` the function takes input stored in different containers, "storage" and "access" aspects of the input data are decoupled and the interface is clean. No performance overhead, the function is not a template.

C++ is an object-oriented programming language and you might have objects with inheritance hierarchy. Let ```Cat``` and ```Dog``` are derived from ```Animal``` and stored as ```std::vector<Cat>``` and ```std::array<Dog, 5>```. How to define argument for a function taking ```Animal```s stored in a buffer? You're forced to use templates and write something super-generic like this:

```C++
template <typename InputIterator>
int countHungryAnimals(InputIterator animalBegin, InputIterator animalEnd);
```

C++20 makes it nicer with Ranges, but still it needs templates and so the code can't be moved to a .cpp file, what causes drawbacks such as slow compilation. 

Can we make it better? Yes, let's use ```poly_span```:

```C++
#include "span/poly_span.h"

int countHungryAnimals(span::poly_span<const Animal> animals);
```

Similar to ```std::span``` and iterators it's flexible to how the objects are stored, the interface is clean and the function is not a template. poly_span will take ```std::vector, std::array, gsl::span```, C array, iterators and pointers of types derived from ```Animal```. Cool, isn't?

poly_span is lightweight wrapper, no dynamic allocation, no virtual functions, just pointer arithmetic.

## How to build and use

It's a header-only library, no third-party dependencies, works with C++11 and above. Just add ```include``` dir to your project's include search path.

To build and run the tests please do:

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
