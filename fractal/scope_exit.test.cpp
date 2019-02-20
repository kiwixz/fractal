#include "scope_exit.h"
#include <doctest/doctest.h>

namespace fractal::test {

TEST_SUITE("scope_exit")
{
    TEST_CASE("generic")
    {
        bool pass = false;
        {
            ScopeExitGeneric scope_exit{[&] {
                pass = true;
            }};
            CHECK(!pass);
        }
        CHECK(pass);
    }

    TEST_CASE("false")
    {
        ScopeExitGeneric{};
        ScopeExit<>{};
    }
}

}  // namespace fractal::test
