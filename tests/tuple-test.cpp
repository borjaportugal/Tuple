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
//	[] make_tuple
//	[x] get
//	[] tie
//	[] tuple_cat
//	[] tuple_element
//	[] make_from_tuple
//	[] forward_as_tuple

TEST(tuple_can_take_any_type_of_arguments)
{
	tuple<int, float, char> my_tuple;
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






