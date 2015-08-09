#pragma once

//#include <concurrent_queue.h>
#include <functional>
#include <boost/lockfree/queue.hpp>
#include <boost/function.hpp>

namespace sst
{
    class AsyncJob
    {
    public:
        AysncJob()
            : m_queue(2048)
        {}

        //typedef Concurrency::concurrent_queue<std::function<void()>> AsyncJobQueue;
        typedef boost::lockfree::queue<boost::function<boost::parameter::void_()>> AsyncJobQueue;

        template<typename Func>
        void Push(Func& func)
        {
            m_queue.push(func);
        }
        void Flush()
        {
            std::function<void()> job;
            while( m_queue.pop(job) )
            {
                job();
            }
        }
    private:
        AsyncJobQueue m_queue;        
    };
}
