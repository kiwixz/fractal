#include "thread_pool.h"
#include <doctest/doctest.h>
#include <memory>
#include <thread>

namespace fractal::test {

TEST_SUITE("thread_pool")
{
    TEST_CASE("submit")
    {
        ThreadPool pool{std::thread::hardware_concurrency()};

        {
            bool pass = false;
            pool.submit([&] {
                    pass = true;
                })
                    .get();
            CHECK(pass);
        }

        {
            bool pass = false;
            pool.submit([](bool* b) {
                    *b = true;
                },
                        &pass)
                    .get();
            CHECK(pass);
        }
    }

    TEST_CASE("special_submit")
    {
        ThreadPool pool{std::thread::hardware_concurrency()};

        pool.submit([ptr = std::make_unique<int>()] {});
        pool.submit([ptr = std::make_shared<int>()]() mutable {
            ptr.reset();
        });
    }
}

}  // namespace fractal::test
