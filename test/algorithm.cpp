#include <gtest/gtest.h>

#include <std/array.h>
#include <std/list.h>
#include <std/fill.h>
#include <std/algorithm.h>
#include <std/sparse_range.h>

using namespace core;

TEST(algorithm, collapse)
{
	array<int> x;

	x = range<int>(0, 10);
	EXPECT_EQ(x, range<int>(0, 10));
	collapse_inplace(x);
	EXPECT_EQ(x, range<int>(0, 10));

	x = fill<int>(10, 4);
	EXPECT_EQ(x, fill<int>(10, 4));
	collapse_inplace(x);
	EXPECT_EQ(x, fill<int>(1, 4));

	x = array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5);
	EXPECT_EQ(x, array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5));
	collapse_inplace(x);
	EXPECT_EQ(x, array_t<int>(7, 1, 2, 3, 4, 3, 4, 5));

	array<int> y;

	x = range<int>(0, 10);
	EXPECT_EQ(x, range<int>(0, 10));
	y = collapse(x);
	EXPECT_EQ(x, range<int>(0, 10));
	EXPECT_EQ(y, range<int>(0, 10));

	x = fill<int>(10, 4);
	EXPECT_EQ(x, fill<int>(10, 4));
	y = collapse(x);
	EXPECT_EQ(x, fill<int>(10, 4));
	EXPECT_EQ(y, fill<int>(1, 4));

	x = array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5);
	EXPECT_EQ(x, array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5));
	y = collapse(x);
	EXPECT_EQ(x, array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5));
	EXPECT_EQ(y, array_t<int>(7, 1, 2, 3, 4, 3, 4, 5));
}

TEST(algorithm, unique)
{
	array<int> x;

	x = range<int>(0, 10);
	EXPECT_EQ(x, range<int>(0, 10));
	unique_inplace(x);
	EXPECT_EQ(x, range<int>(0, 10));

	x = fill<int>(10, 4);
	EXPECT_EQ(x, fill<int>(10, 4));
	unique_inplace(x);
	EXPECT_EQ(x, fill<int>(1, 4));

	x = array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5);
	EXPECT_EQ(x, array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5));
	unique_inplace(x);
	EXPECT_EQ(x, array_t<int>(5, 1, 2, 3, 4, 5));

	array<int> y;

	x = range<int>(0, 10);
	EXPECT_EQ(x, range<int>(0, 10));
	y = unique(x);
	EXPECT_EQ(x, range<int>(0, 10));
	EXPECT_EQ(y, range<int>(0, 10));

	x = fill<int>(10, 4);
	EXPECT_EQ(x, fill<int>(10, 4));
	y = unique(x);
	EXPECT_EQ(x, fill<int>(10, 4));
	EXPECT_EQ(y, fill<int>(1, 4));

	x = array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5);
	EXPECT_EQ(x, array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5));
	y = unique(x);
	EXPECT_EQ(x, array_t<int>(10, 1, 1, 2, 3, 3, 4, 3, 4, 5, 5));
	EXPECT_EQ(y, array_t<int>(5, 1, 2, 3, 4, 5));
}

TEST(algorithm, intersection)
{
	array<int> x = array_t<int>(5, 1, 3, 5, 7, 9);
	array<int> y = array_t<int>(5, 2, 4, 6, 7, 8);
	array<int> z = array_t<int>(5, 1, 4, 7, 9, 10);
	array<int> w = array_t<int>(3, 11, 12, 13);

	EXPECT_EQ(intersection(x, y), array_t<int>(1, 7));
	EXPECT_EQ(intersection(y, z), array_t<int>(2, 4, 7));
	EXPECT_EQ(intersection(x, z), array_t<int>(3, 1, 7, 9));
	EXPECT_EQ(intersection(x, y, z), array_t<int>(1, 7));
	EXPECT_EQ(intersection(x, w), array<int>());

	array<array<int> > q;
	q.push_back(x);
	q.push_back(y);
	q.push_back(z);
	EXPECT_EQ(intersection(q), array_t<int>(1, 7));
}

TEST(algorithm, intersection_size)
{
	array<int> x = array_t<int>(5, 1, 3, 5, 7, 9);
	array<int> y = array_t<int>(5, 2, 4, 6, 7, 8);
	array<int> z = array_t<int>(5, 1, 4, 7, 9, 10);
	array<int> w = array_t<int>(3, 11, 12, 13);

	EXPECT_EQ(intersection_size(x, y), 1);
	EXPECT_EQ(intersection_size(y, z), 2);
	EXPECT_EQ(intersection_size(x, z), 3);
	EXPECT_EQ(intersection_size(x, y, z), 1);
	EXPECT_EQ(intersection_size(x, w), 0);

	array<array<int> > q;
	q.push_back(x);
	q.push_back(y);
	q.push_back(z);
	EXPECT_EQ(intersection_size(q), 1);
}

TEST(algorithm, intersects)
{
	array<int> x = array_t<int>(5, 1, 3, 5, 7, 9);
	array<int> y = array_t<int>(5, 2, 4, 6, 7, 8);
	array<int> z = array_t<int>(5, 1, 4, 7, 9, 10);
	array<int> w = array_t<int>(3, 11, 12, 13);

	EXPECT_TRUE(intersects(x, y));
	EXPECT_TRUE(intersects(y, z));
	EXPECT_TRUE(intersects(x, z));
	EXPECT_TRUE(intersects(x, y, z));
	EXPECT_FALSE(intersects(x, w));

	array<array<int> > q;
	q.push_back(x);
	q.push_back(y);
	q.push_back(z);
	EXPECT_TRUE(intersects(q));
}

TEST(algorithm, symmetric_compliment)
{
	array<int> y = array_t<int>(5, 2, 4, 6, 7, 8);
	array<int> z = array_t<int>(5, 1, 4, 7, 9, 10);

	symmetric_compliment(y, z);
	EXPECT_EQ(y, array_t<int>(3, 2, 6, 8));
	EXPECT_EQ(z, array_t<int>(3, 1, 9, 10));

	y = array_t<int>(5, 1, 2, 3, 4, 5);
	z = array_t<int>(5, 6, 7, 8, 9, 10);

	symmetric_compliment(y, z);
	EXPECT_EQ(y, array_t<int>(5, 1, 2, 3, 4, 5));
	EXPECT_EQ(z, array_t<int>(5, 6, 7, 8, 9, 10));
}

TEST(algorithm, difference)
{
	array<int> y = array_t<int>(5, 2, 4, 6, 7, 8);
	array<int> z = array_t<int>(5, 1, 4, 7, 9, 10);

	EXPECT_EQ(difference(y, z), array_t<int>(3, 2, 6, 8));
	EXPECT_EQ(difference(z, y), array_t<int>(3, 1, 9, 10));

	y = array_t<int>(5, 1, 2, 3, 4, 5);
	z = array_t<int>(5, 6, 7, 8, 9, 10);

	EXPECT_EQ(difference(y, z), array_t<int>(5, 1, 2, 3, 4, 5));
	EXPECT_EQ(difference(z, y), array_t<int>(5, 6, 7, 8, 9, 10));
}

TEST(algorithm, remove)
{
	array<int> y = array_t<int>(7, 2, 4, 6, 2, 7, 8, 8);
	EXPECT_EQ(y, array_t<int>(7, 2, 4, 6, 2, 7, 8, 8));
	remove(y, 2);
	EXPECT_EQ(y, array_t<int>(5, 4, 6, 7, 8, 8));
	remove(y, 7);
	EXPECT_EQ(y, array_t<int>(4, 4, 6, 8, 8));
	remove(y, 8);
	EXPECT_EQ(y, array_t<int>(2, 4, 6));
	remove(y, 4);
	EXPECT_EQ(y, array_t<int>(1, 6));
	remove(y, 6);
	EXPECT_EQ(y, array<int>());
}
