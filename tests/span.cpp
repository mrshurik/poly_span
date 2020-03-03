#include <gtest/gtest.h>

#include <span/poly_span.h>

#include <vector>
#include <array>

using namespace span;

struct Base {
    virtual int value() const = 0;

    virtual bool operator==(const Base& o) const { return value() == o.value(); }
};

struct Derived : Base {
    Derived(int v) : m_value(v) {}

    int value() const override { return m_value; }

    int m_value;
};

struct AnotherBase {
    virtual double another_value() const { return v; };

    double v = 0;
};

struct MultiDerived : AnotherBase, Base {
    MultiDerived(int v) : m_value(v) {}

    int value() const override { return m_value; }

    int m_value;
};

template <typename Span, typename Source>
bool equal(const Span& span, const Source& source) {
    auto sb = std::begin(source);
    return (span.size() == (std::end(source) - sb)) && std::equal(span.begin(), span.end(), sb);
}

TEST(span, defaultConstruct) {
    poly_span<Base> s{};
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(0, s.size());
    EXPECT_EQ(s.begin(), s.end());
}

TEST(span, constructFromVec) {
    std::vector<int> v = {1, 2, 3};

    poly_span<int> s = v;
    EXPECT_EQ(s[0], v[0]);
    EXPECT_EQ(s[1], v[1]);
    EXPECT_EQ(s[2], v[2]);
    EXPECT_EQ(v.size(), s.size());
    EXPECT_EQ(sizeof(v[0]), s.element_size());
    EXPECT_FALSE(s.empty());
}

TEST(span, constructFromVecRespectConst) {
    std::vector<Derived> v;
    const auto& cv = v;
    poly_span<const Base> s1 = v;
    poly_span<const Base>{v};
    poly_span<const Base> s2 = cv;
    poly_span<const Base>{cv};
    poly_span<Derived> s3 = v;
    poly_span<Base>{v};
    //The line below shouldn't work if uncommented
    //poly_span<int>{cv};
}

TEST(span, constructFromCArray) {
    int a[] = {1, 2, 3};
    poly_span<const int> s = a;
    EXPECT_EQ(s[0], a[0]);
    EXPECT_EQ(s[1], a[1]);
    EXPECT_EQ(s[2], a[2]);
    EXPECT_EQ(3, s.size());
    EXPECT_EQ(sizeof(a[0]), s.element_size());
}

TEST(span, constructFromCArrayRespectConst) {
    Derived ar[2] = {1, 2};
    const auto& car = ar;
    poly_span<const Base> s1 = ar;
    poly_span<const Base>{ar};
    poly_span<const Derived> s2 = car;
    poly_span<const Derived>{car};
    poly_span<Base> s3 = ar;
    poly_span<Base>{ar};
    //The line below shouldn't work if uncommented
    //poly_span<int>{car};
}

TEST(span, constructFromArray) {
    std::array<int, 3> a = {1, 2, 3};
    poly_span<int> s = a;
    EXPECT_EQ(s[0], a[0]);
    EXPECT_EQ(s[1], a[1]);
    EXPECT_EQ(s[2], a[2]);
}

TEST(span, constructFromPolySpan) {
    std::array<Derived, 3> a = {1, 2, 3};
    poly_span<Derived> s1 = a;
    poly_span<Base> s2 = s1;
    poly_span<const Base>{s2};
    EXPECT_TRUE(equal(s1, s2));
}

TEST(span, constructFromIterator) {
    std::vector<int> v = {1, 2, 3, 4};
    poly_span<int> s1{v.begin() + 1, v.end()};
    EXPECT_EQ(v.size() - 1, s1.size());
    EXPECT_EQ(s1[0], v[1]);
    EXPECT_EQ(s1[1], v[2]);
    EXPECT_EQ(s1[2], v[3]);

    poly_span<const int> s2{v.data() + 1, v.data() + v.size()};
    EXPECT_TRUE(equal(s1, s2));
    poly_span<const int> s3{v.cbegin(), v.size()};
    EXPECT_TRUE(equal(s3, v));
}

TEST(span, constructFromInitializerList) {
    poly_span<const int> s = {1, 2, 3, 4, 5};
    EXPECT_EQ(5, s.size());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i + 1, s[i]);
    }

    poly_span<const Base> s1 = {Derived(1)};
}

TEST(span, DerivedToBase) {
    std::vector<Derived> v1 = {0, 1, 2};
    Derived a1[] = {0, 1, 2};
    EXPECT_GT(sizeof(Derived), sizeof(Base));
    poly_span<const Base> s1 = v1;
    EXPECT_EQ(sizeof(Derived), s1.element_size());
    poly_span<Base> s2 = a1;

    auto it1 = s1.begin();
    auto it2 = s2.begin();
    for (int i = 0; i < 3; ++i, ++it1, ++it2) {
        EXPECT_EQ(i, s1[i].value());
        EXPECT_EQ(i, s2[i].value());
        EXPECT_EQ(i, it1->value());
        EXPECT_EQ(i, it2->value());
    }
}

TEST(span, MultiDerivedToBase) {
    std::vector<MultiDerived> v1 = {0, 1, 2};
    poly_span<Base> s1 = v1;
    EXPECT_NE(static_cast<void*>(v1.data()), static_cast<void*>(&s1.front()));
    EXPECT_TRUE(equal(s1, v1));
}

TEST(span, typeProperties) {
    EXPECT_TRUE(std::is_trivial<poly_span<int>::iterator>::value);
    EXPECT_TRUE(std::is_trivial<poly_span<const Base>::iterator>::value);
    EXPECT_TRUE(std::is_trivial<poly_span<int>::const_iterator>::value);
    EXPECT_TRUE(bool(std::is_base_of<std::random_access_iterator_tag,
                     poly_span<int>::iterator::iterator_category>::value));
    EXPECT_TRUE(std::is_trivially_copy_constructible<poly_span<int>>::value);
    EXPECT_TRUE(std::is_trivially_copy_assignable<poly_span<int>>::value);
}

TEST(span, iterators) {
    std::vector<int> v = {1, 2, 3};
    poly_span<int> s = v;
    using iterator = poly_span<int>::iterator;
    EXPECT_EQ(s.end() - s.begin(), s.size());
    EXPECT_EQ(s.rend() - s.rbegin(), s.size());
    iterator b = s.begin();
    EXPECT_EQ(b + s.size(), s.end());
    EXPECT_EQ(b[0], 1);
    EXPECT_LT(b, s.end());
    EXPECT_NE(b, s.end());
    EXPECT_GE(b + 2, s.end() - 1);
    poly_span<int>::const_iterator bc = b;
    EXPECT_LE(bc, s.end());
    EXPECT_EQ(s.back(), *s.crbegin());
    EXPECT_EQ(s.front(), s.crend()[-1]);
    bc += 1;
    bc -= 1;
    EXPECT_EQ(bc, s.cbegin());
    EXPECT_TRUE(std::is_sorted(s.cbegin(), s.cend()));
}

TEST(span, randomAccessIteratorUse) {
    int array[] = {25, 4, 3, -2, 1};
    poly_span<int> s = array;
    std::sort(s.begin(), s.end());
    EXPECT_TRUE(std::is_sorted(std::begin(array), std::end(array)));
}

TEST(span, subspan) {
    poly_span<const int> s{1, 2, 3, 4, 5};
    auto s1 = s.subspan(1, 2);
    EXPECT_EQ(2, s1.size());
    EXPECT_EQ(s[1], s1[0]);
    EXPECT_EQ(s[2], s1[1]);
    auto s2 = s.subspan(2);
    EXPECT_EQ(3, s2.size());
    EXPECT_EQ(s[2], s2[0]);
    EXPECT_EQ(s[4], s2[2]);
    auto s3 = s.subspan(0, 10);
    EXPECT_EQ(s.size(), s3.size());
    EXPECT_THROW(s.subspan(6, 1), std::out_of_range);
}

TEST(span, first) {
    poly_span<const int> s{1, 2, 3, 4, 5};
    EXPECT_THROW(s.first(6), std::out_of_range);
    EXPECT_EQ(s.first(5).size(), 5);
    EXPECT_TRUE(s.first(0).empty());
    auto s1 = s.first(3);
    EXPECT_EQ(3, s1.size());
    EXPECT_EQ(1, s1[0]);
    EXPECT_EQ(3, s1[2]);
}

TEST(span, last) {
    poly_span<const int> s{1, 2, 3, 4, 5};
    EXPECT_THROW(s.last(6), std::out_of_range);
    EXPECT_EQ(s.last(5).size(), 5);
    auto s2 = s.last(2);
    EXPECT_EQ(2, s2.size());
    EXPECT_EQ(4, s2[0]);
    EXPECT_EQ(5, s2[1]);
}

TEST(span, equal_view) {
    std::vector<double> v = {1.0, 2.0, 3.0};
    poly_span<double> s1 = v;
    poly_span<const double> cs2 = v;

    EXPECT_FALSE(poly_span<const double>{}.equal_view(s1));
    EXPECT_TRUE(s1.equal_view(cs2));
    EXPECT_TRUE(s1.last(2).equal_view(cs2.subspan(1)));
}

TEST(span, at) {
    poly_span<const int> s = {1, 2, 3};
    EXPECT_EQ(s.at(0), 1);
    EXPECT_EQ(s.at(2), 3);
    EXPECT_THROW(s.at(3), std::out_of_range);
    EXPECT_THROW(s.at(-1), std::out_of_range);
}
