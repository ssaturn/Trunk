#pragma once

#include <functional>
#ifdef _WINDOWS
#include <concurrent_queue.h>
#endif
#include <mutex>
#include <boost/lockfree/queue.hpp>
#include <boost/function.hpp>

namespace sst
{
	class AsyncJob
	{
	public:
		AsyncJob()
		{		
		}

		AsyncJob(int size)
			: m_queue(size)
		{
		}

#ifdef _WINDOWS
		typedef Concurrency::concurrent_queue<std::function<void()>> AsyncJobQueue;

		template<typename Func>
		void Push(Func& func)
		{
			m_queue.push(func);
		}
#else
		typedef boost::lockfree::queue<boost::function<void()>*> AsyncJobQueue;

		template<typename Func>
		void Push(Func* func)
		{
			m_queue.push(func);
		}
#endif 

		void Flush()
		{
#ifdef _WINDOWS
			std::function<void()> job;

			while (m_queue.try_pop(job))
			{
				job();
			}
#else
			boost::function<void()>* job;
			while (m_queue.pop(job))
			{
				job->operator()();
				delete job;
			}
#endif 
			
		}
	private:
		AsyncJobQueue m_queue;
		std::mutex m_lock;
	};
}
