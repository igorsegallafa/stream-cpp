#include <gtest/gtest.h>
#include "Utils/Stream.hpp"

TEST(Stream, CreateStreamRangeInt)
{
    const auto Result = Stream::range(1, 10).Collect();
    const auto Expected = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    EXPECT_EQ(Result, Expected);
}

TEST(Stream, CreateStreamFromIterable)
{
    const auto Vector = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto Result = Stream::of(Vector).Collect();

    EXPECT_EQ(Result, Vector);
}

TEST(Stream, Map)
{
    const auto Expected = std::vector{4, 16, 36, 64, 100};
    const auto Result = Stream::range(1, 5)
        .Map([](int Value){ return Value * 2; })
        .Map([](int Value){ return Value * Value; })
        .Collect();
    
    EXPECT_EQ(Result, Expected);
}

TEST(Stream, Reduce)
{
    const auto Result = Stream::range(1, 5)
        .Reduce(0, [](auto Acc, int Value){ return Acc + Value; })
        .Collect();

    EXPECT_EQ(Result[0], 15);
}

TEST(Stream, Each)
{
    auto Result = 0;
    Stream::range(1, 5)
        .Each([&Result](int Value){ Result += Value; })
        .Run();
    
    EXPECT_EQ(Result, 15);
}

TEST(Stream, Filter)
{
    const auto Expected = std::vector{2, 4};
    const auto Result = Stream::range(1, 5)
        .Map([](int Value){ return Value * 2; })
        .Filter([](int Value){ return Value < 5; })
        .Collect();
    
    EXPECT_EQ(Result, Expected);
}

TEST(Stream, Reject)
{
    const auto Expected = std::vector{6, 8, 10};
    const auto Result = Stream::range(1, 5)
        .Map([](int Value){ return Value * 2; })
        .Reject([](int Value){ return Value < 5; })
        .Collect();
    
    EXPECT_EQ(Result, Expected);
}

TEST(Stream, Take)
{
    const auto Expected = std::vector{1, 2};
    const auto Result = Stream::range(1, 5)
        .Take(2)
        .Collect();
    
    EXPECT_EQ(Result, Expected);
}

TEST(Stream, SplitBy)
{
    const auto Vector = std::vector{1, 2, 1, 3, 4, 5, 1, 6, 7};
    const auto Expected = std::vector{1, 2};
    const auto Result = Stream::of(Vector)
        .SplitBy(1)
        .Collect();
}

TEST(Stream, Join)
{
    const auto Expected = std::vector{1, 2, 2, 3, 3, 4};
    const auto Result = Stream::range(1, 3)
        .Map([](int Value){ return std::vector{Value, Value + 1}; })
        .Join()
        .Collect();
    
    EXPECT_EQ(Result, Expected);
}

TEST(Stream, All)
{
    const auto Result = Stream::range(1, 5)
        .All([](int Value){ return Value == 5; });
    
    EXPECT_EQ(Result, false);
}

TEST(Stream, Any)
{
    const auto Result = Stream::range(1, 5)
        .Any([](int Value){ return Value == 5; });
    
    EXPECT_EQ(Result, true);
}

TEST(Stream, Uniq)
{
    const auto Vector = std::vector{1, 2, 1, 3, 4, 5, 1, 6, 7};
    const auto Expected = std::vector{1, 2, 3, 4, 5, 6, 7};
    const auto Result = Stream::of(Vector)
        .Uniq()
        .Collect();

    EXPECT_EQ(Result, Expected);
}

TEST(Stream, WithIndex)
{
    const auto Result = Stream::range(1, 3)
        .WithIndex()
        .Collect();
    
    const auto Expected = std::vector{std::pair(0, 1), std::pair(1, 2), std::pair(2, 3)};
    EXPECT_EQ(Result, Expected);
}

TEST(Stream, Min)
{
    const auto Result = Stream::range(1, 5).Min();
    EXPECT_EQ(Result, 1);
}

TEST(Stream, Max)
{
    const auto Result = Stream::range(1, 5).Max();
    EXPECT_EQ(Result, 5);
}

TEST(Stream, Sum)
{
    const auto Result = Stream::range(1, 5).Sum();
    EXPECT_EQ(Result, 15);
}

TEST(Stream, Contains)
{
    const auto Result = Stream::range(1, 5).Contains(1);
    EXPECT_EQ(Result, true);
}

TEST(Stream, Count)
{
    const auto Result = Stream::range(1, 5).Count();
    EXPECT_EQ(Result, 5);
}

TEST(Stream, ChunkEvery)
{
    const auto Result = Stream::range(1, 6)
        .ChunkEvery(2)
        .Collect();

    EXPECT_EQ(Result[0][0], 1);
    EXPECT_EQ(Result[0][1], 2);
    EXPECT_EQ(Result[1][0], 3);
    EXPECT_EQ(Result[1][1], 4);
    EXPECT_EQ(Result[2][0], 5);
    EXPECT_EQ(Result[2][1], 6);
}

TEST(Stream, Keys)
{
    const auto KeyMap = std::vector<std::pair<char,int>>{{'b',3},{'a',4},{'z',2},{'k',9}};
    const auto Expected = std::vector{'b', 'a', 'z', 'k'};
    const auto Result = Stream::of(KeyMap)
        .Keys()
        .Collect();
    
    EXPECT_EQ(Result, Expected);
}

TEST(Stream, Values)
{
    const auto KeyMap = std::vector<std::pair<char,int>>{{'b',3},{'a',4},{'z',2},{'k',9}};
    const auto Expected = std::vector{3, 4, 2, 9};
    const auto Result = Stream::of(KeyMap)
        .Values()
        .Collect();
    
    EXPECT_EQ(Result, Expected);
}
