#pragma once

#undef min
#undef max

#include <algorithm>
#include <numeric>
#include <ranges>

template<typename T>
struct StreamImpl
{
    T View{};

    /// Transforms each element in the view using the provided function.
    constexpr auto Map(auto Func)
    {
        auto NewView = View | std::views::transform(Func);
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Applies a function to each element of the view but returns the original value.
    constexpr auto Each(auto Func)
    {
        return Map([Func](auto Value)
        {
            Func(Value);
            return Value;
        });
    }

    /// Reduces the view to a single value using the provided function.
    constexpr auto Reduce(auto InitialValue, auto Func)
    {
        auto Result = std::accumulate(View.begin(), View.end(), InitialValue, Func);
        auto NewView = std::views::single(Result);
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Filters elements based on the provided predicate function.
    constexpr auto Filter(auto Func)
    {
        auto NewView = View | std::views::filter(Func);
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Filters out elements that do not satisfy the provided predicate function.
    constexpr auto Reject(auto Func)
    {
        auto NewFunc = [Func](auto Value){ return !Func(Value); };
        auto NewView = View | std::views::filter(NewFunc);
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Takes the first 'Count' elements from the view.
    constexpr auto Take(auto Count)
    {
        auto NewView = View | std::views::take(Count);
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Splits the view by the given token.
    constexpr auto SplitBy(auto Token)
    {
        auto NewView = View | std::views::split(Token);
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Joins nested ranges into a single range.
    constexpr auto Join()
    {
        auto NewView = View | std::views::join;
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Returns the keys of the view.
    constexpr auto Keys()
    {
        auto NewView = View | std::views::keys;
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Returns the values of the view.
    constexpr auto Values()
    {
        auto NewView = View | std::views::values;
        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Checks if all elements satisfy the provided predicate function.
    constexpr auto All(auto Func)
    {
        auto FilteredData = Filter(Func);
        return Count() == FilteredData.Count();
    }

    /// Checks if any element satisfies the provided predicate function.
    constexpr auto Any(auto Func)
    {
        auto FilteredData = Filter(Func);
        return FilteredData.Count() > 0;
    }

    /// Returns unique elements by counting occurrences and filtering out duplicates.
    constexpr auto Uniq()
    {
        return WithIndex()
            .Map([this](auto ValuePair)
            {
                auto [Index, Value] = ValuePair;
                return std::pair{Take(Index).Count(Value), Value};
            })
            .Reject([](auto ValuePair)
            {
                auto [Count, Value] = ValuePair;
                return Count > 0;
            })
            .Map([](auto ValuePair)
            {
                auto [Count, Value] = ValuePair;
                return Value;
            });
    }

    /// Pairs each element with its index.
    constexpr auto WithIndex()
    {
        auto Iota = std::views::iota(0, Count());
        auto NewView = Iota | std::views::transform([this](auto Index)
        {
            return std::pair(Index, *std::next(View.begin(), Index));
        });

        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Returns the minimum element in the view.
    constexpr auto Min()
    {
        return *std::ranges::min_element(View.begin(), View.end());
    }

    /// Returns the maximum element in the view.
    constexpr auto Max()
    {
        return *std::ranges::max_element(View.begin(), View.end());
    }

    /// Returns the sum of all elements in the view.
    constexpr auto Sum()
    {
        return std::accumulate(View.begin(), View.end(), 0);
    }

    /// Checks if the view contains a specific value.
    constexpr auto Contains(auto Value)
    {
        return Count(Value) > 0;
    }

    /// Returns the number of elements in the view.
    constexpr auto Count()
    {
        return std::ranges::distance(View);
    }

    /// Counts the occurrences of a specific value in the view.
    constexpr auto Count(auto Value)
    {
        return std::ranges::count(View, Value);
    }

    /// Chunks the view into sub ranges of a given size.
    constexpr auto ChunkEvery(auto Count)
    {
        // TODO: Replace with std::views::chunk when used with C++23
        auto Iota = std::views::iota(0, (std::ranges::distance(View) + Count - 1) / Count);
        auto NewView = Iota | std::views::transform([this, Count](auto Index)
        {
            auto Begin = std::next(View.begin(), Index * Count);
            auto End = std::next(Begin, std::min(static_cast<size_t>(Count), static_cast<size_t>(std::ranges::distance(Begin, View.end()))));
            
            return std::ranges::subrange(Begin, End);
        });

        return StreamImpl<decltype(NewView)>{NewView};
    }

    /// Collects the view elements into a std::vector.
    auto Collect()
    {
        std::vector<std::ranges::range_value_t<T>> Result;
    
        // Reserve space if the view size is known (e.g., random access or sized ranges)
        if constexpr (std::ranges::sized_range<T>)
        {
            Result.reserve(std::ranges::distance(View));
        }
        
        std::ranges::copy(View, std::back_inserter(Result));
        return Result;
    }

    /// Runs the stream operations.
    auto Run()
    {
        Collect();
    }
};

namespace Stream
{
    static constexpr auto of(auto Value)
    {
        return StreamImpl{Value};
    }

    static constexpr auto range(auto Begin, auto End)
    {
        return StreamImpl{std::views::iota(Begin, End + 1)};
    }
}
