#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "AsyncJob.h"

sst::AsyncJob* g_asyncJob = nullptr;
std::atomic<bool> g_stop = false;
std::atomic<int> g_count = 0;


void Init()
{
    if ( !g_asyncJob )
    {
        g_asyncJob = new sst::AsyncJob;
    }
}

void Finish()
{
    if ( g_asyncJob )
    {
        delete g_asyncJob;
        g_asyncJob = nullptr;
    }
}

void Callback()
{
    while ( true )
    {
        if ( g_stop )
        {
            return;
        }

        std::this_thread::sleep_for( std::chrono::milliseconds( 400 ) );

        g_asyncJob->Flush();
    }
}

int main()
{
    Init();

    std::vector<std::thread> workers;

    for ( int i = 0; i < 5; ++i )
    {
        workers.push_back( std::thread( Callback ) );

    }

    for ( auto& worker : workers )
    {
        worker.joinable();
    }

    while ( 1 )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        g_asyncJob->Push( [ ] () {
                if ( g_count == 30 )
                {
                    g_stop = true;
                    return;
                }

                std::cout << std::this_thread::get_id() << " : " << g_count++ << std::endl;
        } );

        if ( g_stop )
        {
            break;
        }
    }

    for ( auto& worker : workers )
    {
        worker.detech();
    }
}
