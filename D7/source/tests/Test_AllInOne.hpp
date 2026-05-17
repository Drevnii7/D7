#pragma once

#include "Test_ETokenType.hpp"
#include "Test_FToken.hpp"
#include "Test_CLexer.hpp"
#include "Test_CPreprocessor.hpp"

namespace d7
{
    namespace tests
    {
        bool Test_AllInOne()
        {
            int l_fails = 0;

            if (!Test_ETokenType())
                l_fails++;

            if (!Test_FToken())
                l_fails++;

            if (!Test_CLexer())
                l_fails++;

            if (!Test_CPreprocessor())
                l_fails++;

            return l_fails == 0;
        }
    }
}
