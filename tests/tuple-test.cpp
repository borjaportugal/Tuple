/*!
\author Borja Portugal Martin
\brief  All tests that make sure a correct implementation of tuple<>

GitHub: https://github.com/borjaportugal

This file is subject to the license terms in the LICENSE file
found in the top-level directory of this distribution.
*/

#include "src/tuple.h"

#include "testing\testing.h"

#include <tuple>

// TODOS:
// 
//	[x] tuple can store any number of arguments
//	[x] make_tuple
//	[x] make_tuple getting references
//	[x] get
//	[x] is_tuple
//	[x] tie
//	[x] tuple_cat
//	[x] tuple_element
//	[x] make_from_tuple
//	[] ignore
//	[] forward_as_tuple

TEST(tuple_can_take_any_type_of_arguments)
{
	tuple<int, float, char> my_tuple;
	(void)my_tuple;
}

TEST(tuple_has_the_expected_size)
{
	struct SizeTest
	{
		int a;
		float f;
		char c;
	};

	TEST_ASSERT(sizeof(tuple<int, float, char>) == sizeof(SizeTest));
}


TEST(tuple_can_take_parameters_in_ctor)
{
	tuple<int, float, char> my_tuple{ 300, 1.f, 'a' };
}

TEST(can_request_values_to_a_tuple)
{
	tuple<int, float, char> my_tuple{ 300, 1.f, 'a' };
	TEST_ASSERT(get<0>(my_tuple) == 300);
	TEST_ASSERT(get<1>(my_tuple) == 1.f);
	TEST_ASSERT(get<2>(my_tuple) == 'a');
}

TEST(can_request_values_to_a_tuple_const_variation)
{
	const tuple<int, float, char> my_tuple{ 300, 1.f, 'a' };
	TEST_ASSERT(get<0>(my_tuple) == 300);
	TEST_ASSERT(get<1>(my_tuple) == 1.f);
	TEST_ASSERT(get<2>(my_tuple) == 'a');
}

TEST(make_tuple_works_as_expected)
{
	const auto my_tuple = make_tuple(300, 1.f, 'a');
	TEST_ASSERT(typeid(my_tuple) == typeid(tuple<int, float, char>));

	TEST_ASSERT(get<0>(my_tuple) == 300);
	TEST_ASSERT(get<1>(my_tuple) == 1.f);
	TEST_ASSERT(get<2>(my_tuple) == 'a');
}

TEST(make_tuple_works_with_references)
{
	char c = 'h';
	auto my_tuple = make_tuple(300, tuple_ref(c));
	TEST_ASSERT(typeid(my_tuple) == typeid(tuple<int, char &>));

	TEST_ASSERT(get<0>(my_tuple) == 300);
	TEST_ASSERT(get<1>(my_tuple) == 'h');
	
	c = '@';
	TEST_ASSERT(get<0>(my_tuple) == 300);
	TEST_ASSERT(get<1>(my_tuple) == '@');

	get<1>(my_tuple) = '#';
	TEST_ASSERT(get<0>(my_tuple) == 300);
	TEST_ASSERT(get<1>(my_tuple) == '#');
}

TEST(make_tuple_works_with_const_references)
{
	char c = 'h';
	const auto my_tuple = make_tuple(300, tuple_ref(c));
	TEST_ASSERT(typeid(my_tuple) == typeid(tuple<int, char &>));

	TEST_ASSERT(get<0>(my_tuple) == 300);
	TEST_ASSERT(get<1>(my_tuple) == 'h');

	c = '@';
	TEST_ASSERT(get<0>(my_tuple) == 300);
	TEST_ASSERT(get<1>(my_tuple) == '@');
}


TEST(tuple_element_t_is_the_type_at_the_provided_index)
{
	using tuple_type = tuple<int, float, char>;
	TEST_ASSERT(typeid(tuple_element_t<0, tuple_type>) == typeid(int));
	TEST_ASSERT(typeid(tuple_element_t<1, tuple_type>) == typeid(float));
	TEST_ASSERT(typeid(tuple_element_t<2, tuple_type>) == typeid(char));
}

TEST(is_tuple_evaluates_to_true_if_the_parameter_is_a_tuple)
{
	using tuple_type = tuple<int, float, char>;
	TEST_ASSERT(is_tuple<tuple_type>::value);

	struct Not_a_tuple {};
	TEST_ASSERT(!is_tuple<Not_a_tuple>::value);
}

TEST(tuple_cat_t_determines_the_tuple_type_that_yields_tuple_concatenation)
{
	using tuple_a = tuple<int, float, char>;
	using tuple_b = tuple<unsigned, double>;

	using concatenated_tuple = tuple_cat_type_t<
		tuple_a, 
		tuple_b, 
		tuple_a
	>;
	using expected_tuple = tuple<
		int, float, char,	// tuple_a
		unsigned, double,	// tuple_b
		int, float, char	// tuple_a
	>;

	TEST_ASSERT(typeid(concatenated_tuple) == typeid(expected_tuple));
}

TEST(operator_equal_equal_works_with_tuples)
{
	const tuple<int, char> a{ 1, 'a' };
	const tuple<int, char> b{ 2, 'b' };
	const tuple<int, char> c = a;

	TEST_ASSERT(a != b);
	TEST_ASSERT(a == c);
	TEST_ASSERT(!(a == b));
	TEST_ASSERT(!(a != c));
}

TEST(tuple_cat_returns_the_correct_type)
{
	const tuple<int, char> a{ 1, 'a' };
	const tuple<bool, float> b{ false, 3.f };

	using expected_tuple_type = tuple<
		int, char,		// a
		bool, float,	// b
		int, char		// a
	>;

	TEST_ASSERT(typeid(tuple_cat(a, b, a)) == typeid(expected_tuple_type));
}

TEST(tuple_cat_returns_the_correct_value)
{
	const tuple<int, char> a{ 1, 'a' };
	const tuple<bool, float> b{ false, 3.f };
	
	using expected_tuple_type = tuple<
		int, char,		// a
		bool, float,	// b
		int, char		// a
	>;

	const expected_tuple_type concatenated_tuple = tuple_cat(a, b, a);
	const expected_tuple_type expected_tuple{ 1, 'a', false, 3.f, 1, 'a' };
	TEST_ASSERT(concatenated_tuple == expected_tuple);
}

TEST(changing_values_on_a_tuple_returned_by_tie_changed_the_original_variables)
{
	int i = 10;
	double d = 4.0;
	char c = '3';

	tuple<double &, int &, char &> t = tie(d, i, c);

	get<0>(t) = 7.0;
	get<2>(t) = 'd';

	TEST_ASSERT(i == 10);
	TEST_ASSERT(d == 7.0);
	TEST_ASSERT(c == 'd');
}
TEST(tie_ca_be_used_as_lhs_in_assignments)
{
	int i = 0;
	double d = 0.0;
	char c = 0;

	tie(d, i, c) = []() { return make_tuple(7.0, 10, 'd'); }();
	
	TEST_ASSERT(i == 10);
	TEST_ASSERT(d == 7.0);
	TEST_ASSERT(c == 'd');
}
TEST(tie_can_take_as_a_parameter_ignore)
{
	double d = 0.0;
	char c = 0;

	tie(d, ignore, c) = []() -> tuple<double, int, char> { return make_tuple(7.0, 10, 'd'); }();

	TEST_ASSERT(d == 7.0);
	TEST_ASSERT(c == 'd');
}


TEST(make_from_tuple_forwards_the_tuple_values_to_the_object_constructor)
{
	struct SomeStructure
	{
		SomeStructure(int ii, char cc, float ff)
			: i{ ii }
			, c{ cc }
			, f{ ff }
		{}

		int i{ 0 };
		char c{ 0 };
		float f{ 0.f };
	};

	const int i = 'g';
	const char c = 5;
	const float f = 8.14f;

	const tuple<int, char, float> args{ i, c, f };
	const SomeStructure result = make_from_tuple<SomeStructure>(args);

	TEST_ASSERT(result.i == i);
	TEST_ASSERT(result.c == c);
	TEST_ASSERT(result.f == f);
}

TEST(make_from_tuple_also_works_for_references)
{
	struct SomeStructure
	{
		SomeStructure(int ii, char & cc, float ff)
			: i{ ii }
			, c{ cc }
			, f{ ff }
		{}

		int i{ 0 };
		char & c;
		float f{ 0.f };
	};

	const int i = 'g';
	char c = 5;
	const float f = 8.14f;

	tuple<int, char &, float> args{ 'g', c, 8.14f };
	const SomeStructure result = make_from_tuple<SomeStructure>(args);

	TEST_ASSERT(result.i == i);
	TEST_ASSERT(result.c == c);
	TEST_ASSERT(result.f == f);

	c = 10;

	TEST_ASSERT(result.i == i);
	TEST_ASSERT(result.c == c);
	TEST_ASSERT(result.f == f);
}









