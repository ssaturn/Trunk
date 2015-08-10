// AsyncJob.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <boost/function.hpp>
#include "AsyncJob.h"

sst::AsyncJob* g_asyncJob = nullptr;
std::atomic<bool> g_stop(false);
std::atomic<int> g_count(0);
std::mutex g_lock;

void Init()
{
	if (!g_asyncJob)
	{
#ifdef _WINDOWS
		g_asyncJob = new sst::AsyncJob;
#else
		g_asyncJob = new sst::AsyncJob(1024);
#endif
	}
}

void Finish()
{
	if (g_asyncJob)
	{
		delete g_asyncJob;
		g_asyncJob = nullptr;
	}
}

void Callback()
{
	while (true)
	{
		if (g_stop)
		{
			return;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(400));

		g_asyncJob->Flush();
	}
}

int main()
{
	Init();

	std::vector<std::thread> workers;

	for (int i = 0; i < 5; ++i)
	{
		workers.push_back(std::thread(Callback));

	}

	for (auto& worker : workers)
	{
		worker.joinable();
	}

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		auto function = []() { 
			if (g_count >= 30)
			{
				g_stop = true;
				return;
			}
			std::lock_guard<std::mutex> lock(g_lock);
			std::cout << std::this_thread::get_id() << " : " << g_count++ << std::endl; };

#ifdef _WINDOWS
		g_asyncJob->Push(function);
#else
		g_asyncJob->Push(new boost::function<void()>(function));
#endif 


		if (g_stop)
		{
			break;
		}
	}

	for (auto& worker : workers)
	{
		worker.detach();
	}
}
