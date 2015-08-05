#pragma once

#include <concurrent_queue.h>
#include <functional>

namespace sst
{
    class AsyncJob
    {
    public:
        typedef Concurrency::concurrent_queue<std::function<void()>> AsyncJobQueue;

        template<class Func>
        void Push(Func& func)
        {
            m_queue.push(func);
        }
        void Flush()
        {
            std::function<void()> job;
            while( m_queue.try_pop(job) )
            {
                job();
            }
        }
    private:
        AsyncJobQueue m_queue;        
    };
}
