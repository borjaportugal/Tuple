/*!
\author Borja Portugal Martin
\brief  Enter point for running all the test cases.
On success does nothing, on failure remains the console opened.
*/

#include "testing\testing.h"

#include <iostream>

int main(int argc, char** argv)
{
	if (testing::run_all_tests() == false)
		std::cin.get();

	return 0;
}
