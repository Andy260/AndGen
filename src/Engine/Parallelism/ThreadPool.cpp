#include "ThreadPool.hpp"

// Constructs a new thread pool with a specified amount of threads
AndGen::ThreadPool::ThreadPool(unsigned int threadCount)
{
	m_threads.reserve(threadCount);

	// Create threads and start them
	for (size_t i = 0; i < threadCount; i++)
	{
		m_threads.push_back(std::make_unique<PooledThread>());
		m_threads[i]->Start();
	}
}

// De-constructor of the Thread Pool
AndGen::ThreadPool::~ThreadPool()
{
	// Wait for threads to finish executing
	for (size_t i = 0; i < m_threads.size(); i++)
	{
		m_threads[i]->Stop(true);
	}
}

// Waits for threads to complete their current tasks and become idle
void AndGen::ThreadPool::WaitForThreads()
{
	// Wait for each thread to complete their execution queue
	for (size_t i = 0; i < m_threads.size(); i++)
	{
		m_threads[i]->WaitForQueue();
	}
}
