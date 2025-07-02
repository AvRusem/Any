# Any - Custom Implementation of std::any

This is a personal academic project that provides a custom implementation of functionality similar to C++17's `std::any`.

## Overview

The `Any` allows storing values of arbitrary types in a type-erased container and retrieving them using safe type casting. It includes:

- A base interface for type-erased storage (`IHolder`)
- A templated class (`AnyHolder<T>`) to hold values of specific types
- A class (`Any`) to manage the value and provide copy, move, and assignment semantics
- A custom exception (`BadAnyCast`) for failed casts
- A `AnyCast<T>` function to retrieve the value safely

## Example Usage

```cpp
#include "any.hpp"
#include <iostream>
#include <string>

int main() {
    Any a = std::string("Hello");
    
    try {
        std::string s = AnyCast<std::string>(a);
        std::cout << s << std::endl; // Output: Hello
    } catch (const BadAnyCast& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
