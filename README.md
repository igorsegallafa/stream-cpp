# StreamCpp - A Functional Approach to C++ Streams

**StreamCpp** is a C++ header-only library that provides a functional programming approach to working with streams, heavily inspired by the **Stream** and **Enum** modules from the Elixir language. The library aims to simplify the use of C++'s `std::views` while offering a clean and expressive API for data manipulation, making it easier to adopt a functional style in C++ code.

## Features

- **Functional Approach**: Inspired by Elixir’s `Stream` and `Enum` modules, this library brings a more functional approach to C++ streams and ranges.
- **Simpler Usage of `std::views`**: StreamCpp wraps and simplifies `std::views`, making range transformations and filtering more intuitive.
- **Header-Only Library**: No need for additional build steps or dependencies—just include the header and start using it.
- **Work in Progress**: While already functional, this library is still under development. Expect improvements and new features over time.

## Key Operations

- **Map**: Transform each element of the stream using a function.
- **Each**: Perform a side-effect on each element without changing the original value.
- **Filter**: Keep only the elements that match a predicate.
- **Reject**: Remove elements that match a predicate.
- **Take**: Take the first N elements from the stream.
- **SplitBy**: Split a stream into substreams by a token.
- **Join**: Flatten nested ranges into a single range.
- **All/Any**: Check if all or any elements satisfy a predicate.
- **Uniq**: Filter out duplicate values.
- **WithIndex**: Pair each element with its index.
- **Min/Max**: Retrieve the minimum or maximum value in the stream.
- **Contains**: Check if a specific value exists in the stream.
- **Count**: Count the total elements or the occurrences of a specific value.
- **ChunkEvery**: Split the stream into subranges of a given size.
- **Collect**: Gather the elements into a `std::vector`.
- **Run**: Execute the stream pipeline.

## Example Usage

```cpp
#include "Stream.h"
#include <iostream>

int main() {
    auto stream = Stream::range(1, 10)
        .Filter([](int x) { return x % 2 == 0; })
        .Map([](int x) { return x * x; })
        .Collect();

    for (const auto& value : stream) {
        std::cout << value << " "; // Output: 4 16 36 64 100
    }
}
```

## Future Work

This library is a work in progress. Future enhancements may include:

- Full C++23 support, including the use of `std::views::chunk`.
- More advanced stream operations.
- Performance optimizations and further testing.

## Contributions

Contributions and feedback are welcome! Feel free to open issues or submit pull requests to help improve the library.
