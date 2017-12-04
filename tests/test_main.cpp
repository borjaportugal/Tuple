/*!
\author Borja Portugal Martin
\brief  Enter point for running all the test cases.
On success does nothing, on failure remains the console opened.

GitHub: https://github.com/borjaportugal

This file is subject to the license terms in the LICENSE file
found in the top-level directory of this distribution.
*/

#include "testing\testing.h"

#include <iostream>

int main(int argc, char** argv)
{
	if (testing::run_all_tests() == false)
		std::cin.get();

	return 0;
}
