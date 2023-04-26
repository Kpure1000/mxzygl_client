#ifndef MX_JOBSYSTEM_H
#define MX_JOBSYSTEM_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <functional>
#include <type_traits>
#include <future>
#include <memory>
#include <algorithm>

unsigned int get_core_numbers();

class JobSystem {
public:
    explicit JobSystem(size_t size) : m_is_destroy(false)
    {
        for (size_t i = 0; i < size; i++) {
            m_threads.emplace_back([this] {
                while (true) {
                    std::function<void()> job;
                    {
                        // get lock
                        std::unique_lock<std::mutex> lock(m_mutex);
                        // wait for job
                        m_condition.wait(lock, [this]() {
                            return this->m_is_destroy || !this->m_jobs.empty();
                        });
                        // destroy current thread
                        if (this->m_is_destroy && this->m_jobs.empty()) {
                            return;
                        }
                        // get job
                        job = std::move(this->m_jobs.front());
                        this->m_jobs.pop();
                    }
                    // run job
                    job();
                }
            });
        }
    }

    ~JobSystem() {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_is_destroy = true;
        }
        // notify all
        m_condition.notify_all();
        // join all
        for_each(m_threads.begin(), m_threads.end(), std::mem_fn(&std::thread::join));
    }

    /**
     * f: job function
     * args: args of function
     */
    template<class F, class ... Args>
    decltype(auto) submit(F&& f, Args&&... args);

    inline size_t getThreadSize() const {
        return m_threads.size();
    }

    inline size_t getWaitingCount() const {
        return m_jobs.size();
    }

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_jobs;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;

    bool m_is_destroy;

public:
    static JobSystem* getInstance(size_t maxThreadSize = get_core_numbers()) {
        static JobSystem instance(maxThreadSize);
        return &instance;
    }

};

template<class F, class ...Args>
inline decltype(auto) JobSystem::submit(F&& f, Args && ...args) {
    using return_type = std::invoke_result_t<F, Args...>;

    auto job = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> result = job->get_future();

    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_is_destroy) {
            throw std::runtime_error("Add job on detroyed job system");
        }

        //  add task
        m_jobs.emplace([job]() { (*job)(); });
    }
    m_condition.notify_one();
    return result;
}

#endif // MX_JOBSYSTEM_H
