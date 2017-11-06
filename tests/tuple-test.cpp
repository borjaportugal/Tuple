/*!
\author Borja Portugal Martin
\brief  All tests that make sure a correct implementation of tuple<>
*/

#include "src/tuple.h"

#include "testing\testing.h"

#include <tuple>

// TODOS:
// 
//	[x] tuple can store any number of arguments
//	[x] make_tuple
//	[] make_tuple getting references
//	[x] get
//	[x] is_tuple
//	[] tie
//	[] tuple_cat
//	[] tuple_element
//	[] make_from_tuple
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







