#ifndef THREADPOOL_H
#define THREADPOOL_H

// STL includes
#include <algorithm>
#include <exception>
#include <limits>
#include <memory>
#include <vector>
// AndGen includes
#include <AndGen/Engine/Jobs/Job.hpp>
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
		ThreadPool(unsigned int threadCount = GetIdealThreadCount());
		ThreadPool(const ThreadPool&)				= delete;
		ThreadPool& operator=(const ThreadPool&)	= delete;

		/// <summary>
		/// De-constructor of the Thread Pool. 
		/// Will wait for all threads to complete their current tasks before de-constructing.
		/// </summary>
		~ThreadPool();

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

		/// <summary>
		/// Adds a job to the thread pool to execute
		/// </summary>
		/// <param name="job">Job to enqueue</param>
		/// <typeparam name="JobType">Type of job</typeparam>
		template<class JobType>
		void QueueJob(const std::shared_ptr<JobType> job)
		{
			// Find value of thread with least amount of jobs queued
			size_t lowestQueueCount = std::numeric_limits<size_t>::max();
			for (size_t i = 0; i < m_threads.size(); i++)
			{
				if (m_threads[i]->GetQueue().Count() < lowestQueueCount)
				{
					lowestQueueCount = m_threads[i]->GetQueue().Count();
				}
			}

			// Queue with a thread which has the least amount of jobs queued
			for (size_t i = 0; i < m_threads.size(); i++)
			{
				if (m_threads[i]->GetQueue().Count() <= lowestQueueCount)
				{
					m_threads[i]->QueueJob(job);
					return;
				}
			}

			throw std::logic_error("Unknown logic error - unable to enqueue job");
		}
		
		/// <summary>
		/// Adds multiple jobs from a collection to the thread pool to execute
		/// </summary>
		/// <param name="beginItr">Begin iterator for collection</param>
		/// <param name="endItr">End iterator for collection</param>
		/// <typeparam name="JobType">Type of jobs</typeparam>
		template<class Iterator>
		void QueueJobs(const Iterator beginItr, const Iterator endItr)
		{
			for (Iterator i = beginItr; i != endItr; i++)
			{
				QueueJob(*i);
			}
		}

		/// <summary>
		/// Waits for threads to complete their current tasks and become idle
		/// </summary>
		void WaitForThreads();

		/// <summary>
		/// The amount of threads within the pool
		/// </summary>
		inline unsigned int Size()
		{
			return static_cast<unsigned int>(m_threads.size());
		}

		/// <summary>
		/// The amount of Jobs currently queued to be processed
		/// </summary>
		inline unsigned int PendingJobsCount() const
		{
			unsigned int pendingJobsCount = 0;
			for (size_t i = 0; i < m_threads.size(); i++)
			{
				pendingJobsCount += static_cast<unsigned int>(m_threads[i]->GetQueue().Count());
			}

			return pendingJobsCount;
		}

		/// <summary>
		/// Returns the amount of threads currently executing a task
		/// </summary>
		inline unsigned int RunningCount() const
		{
			unsigned int runningCount = 0;
			for (size_t i = 0; i < m_threads.size(); i++)
			{
				if (m_threads[i]->GetStatus() == PooledThread::Status::ExecutingJobs)
				{
					runningCount++;
				}
			}

			return runningCount;
		}

		/// <summary>
		/// Amount of threads currently idle and not executing a task
		/// </summary>
		inline unsigned int IdleCount() const
		{
			unsigned int idleCount = 0;
			for (size_t i = 0; i < m_threads.size(); i++)
			{
				if (m_threads[i]->GetStatus() == PooledThread::Status::Idle)
				{
					idleCount++;
				}
			}

			return idleCount;
		}

	private:
		std::vector<std::unique_ptr<PooledThread>> m_threads;
	};
}

#endif
