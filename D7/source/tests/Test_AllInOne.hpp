#pragma once

#include "Test_ETokenType.hpp"

namespace d7
{
    namespace tests
    {
        bool Test_AllInOne()
        {
            int l_fails = 0;

            if (!Test_ETokenType())
                l_fails++;

            if (l_fails == 0)
            {
                std::cout << "!!!Test_AllInOne: ALL TESTS PASSED!!!" << std::endl;
            }
            else
            {
                std::cout << "!!!Test_AllInOne: FAILED with " << l_fails << " errors!!!" << std::endl;
            }

            return l_fails == 0;
        }

    }
}
