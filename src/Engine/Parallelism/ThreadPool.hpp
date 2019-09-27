#ifndef THREADPOOL_H
#define THREADPOOL_H

// STL includes
#include <algorithm>
#include <exception>
#include <functional>
#include <vector>
// AndGen includes
#include <AndGen/Exceptions/NotImplementedException.hpp>
#include "..\Parallelism\PooledThread.hpp"

namespace AndGen
{
	/// <summary>
	/// Pool of threads which can be used to execute tasks asynchronously
	/// </summary>
	class ThreadPool
	{
	public:
		/// <summary>
		/// Constructs a new thread pool with a specified amount of threads
		/// </summary>
		/// <param name="threadCount">Amount of threads to construct within the pool</param>
		ThreadPool(int threadCount);
		ThreadPool(const ThreadPool&)				= delete;
		ThreadPool& operator=(const ThreadPool&)	= delete;
		/// <summary>
		/// De-constructor of the Thread Pool. 
		/// Will wait for all threads to complete their current tasks before de-constructing.
		/// </summary>
		~ThreadPool() = default;

		/// <summary>
		/// Returns the ideal amount of threads in the pool for maximum performance
		/// </summary>
		/// <remarks>
		/// This function returns the ideal amount of threads within the thread pool
		/// for the best performance. This is should be the same as the amount of cores
		/// of the executing system minus 1, since the function considers the main thread as well.
		/// On some hardware configurations, this function may return a value of 0, in which case
		/// the thread pool shouldn't be used, but rather just the main thread for executing jobs.
		/// </remarks>
		inline static unsigned int GetIdealThreadCount()
		{
			return std::max<unsigned int>(0, std::thread::hardware_concurrency() - 1);
		}

		template<class JobType>
		void QueueJob(JobType job)
		{
			throw NotImplementedException();
		}
		/// <summary>
		/// Waits for threads to complete their current tasks and become idle
		/// </summary>
		void WaitForThreads();
		/// <summary>
		/// Changes the amount of threads within the pool
		/// </summary>
		void Resize(int threadCount);

		/// <summary>
		/// The amount of threads within the pool
		/// </summary>
		inline int Size()
		{
			throw NotImplementedException();
		}
		/// <summary>
		/// Returns the amount of threads currently executing a task
		/// </summary>
		inline int RunningCount() const
		{
			throw NotImplementedException();
		}
		/// <summary>
		/// Amount of threads currently idle and not executing a task
		/// </summary>
		inline int IdleCount() const
		{
			throw NotImplementedException();
		}

	private:
		std::vector<PooledThread> m_threads;
	};
}

#endif
