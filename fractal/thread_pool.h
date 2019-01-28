#pragma once

#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace fractal {

class ThreadPool {
public:
    ThreadPool() = default;
    ThreadPool(size_t nr_threads);
    ~ThreadPool();
    ThreadPool(ThreadPool const&) = delete;
    ThreadPool& operator=(ThreadPool const&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    size_t size() const;
    void extend(size_t nr_threads);

    /// Callable only needs to be moveable, but args must be copyable.
    template <typename T, typename... Args>
    std::future<std::invoke_result_t<T, Args...>> submit(T&& callable, Args&&... args);

private:
    /// Wrapper to make tasks copyable in case they are not.
    template <typename T>
    struct Callable {
        Callable(T&& callable);

        template <typename... Args>
        auto operator()(Args&&... args);

    private:
        std::shared_ptr<T> callable_;
    };

    std::vector<std::thread> workers_;

    std::mutex tasks_mutex_;
    std::condition_variable tasks_condvar_;
    std::queue<std::function<void()>> tasks_;
    bool stopping_ = false;
};


template <typename T, typename... Args>
std::future<std::invoke_result_t<T, Args...>> ThreadPool::submit(T&& callable, Args&&... args)
{
    using ReturnType = std::invoke_result_t<T, Args...>;


    std::shared_ptr<std::packaged_task<ReturnType()>> task;

    if constexpr (sizeof...(Args) == 0) {
        if constexpr (std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>)
            task = std::make_shared<std::packaged_task<ReturnType()>>([callable = std::forward<T>(callable)]() mutable {
                callable();
            });
        else
            task = std::make_shared<std::packaged_task<ReturnType()>>([callable = Callable<T>{std::forward<T>(callable)}]() mutable {
                callable();
            });
    }
    else {
        if constexpr (std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>)
            task = std::make_shared<std::packaged_task<ReturnType()>>([callable = std::forward<T>(callable),
                                                                       bound_args_tuple = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                std::apply([&](auto&&... bound_args) {
                    return std::invoke(std::forward<T>(callable), std::forward<Args>(bound_args)...);
                },
                           std::move(bound_args_tuple));
            });
        else
            task = std::make_shared<std::packaged_task<ReturnType()>>([callable = Callable<T>{std::forward<T>(callable)},
                                                                       bound_args_tuple = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                std::apply([&](auto&&... bound_args) {
                    return std::invoke(std::move(callable), std::forward<Args>(bound_args)...);
                },
                           std::move(bound_args_tuple));
            });
    }

    std::future<ReturnType> future = task->get_future();

    {
        std::unique_lock lock(tasks_mutex_);
        if (stopping_)
            throw std::runtime_error{"trying to add work on stopping thread pool"};
        tasks_.emplace([task = std::move(task)] {
            (*task)();
        });
    }

    tasks_condvar_.notify_one();
    return future;
}


template <typename T>
ThreadPool::Callable<T>::Callable(T&& callable) :
    callable_{std::make_unique<T>(std::forward<T>(callable))}
{}

template <typename T>
template <typename... Args>
auto ThreadPool::Callable<T>::operator()(Args&&... args)
{
    return (*callable_)(std::forward<Args>(args)...);
}

}  // namespace fractal
