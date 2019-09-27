#ifndef POOLEDTHREAD_H
#define POOLEDTHREAD_H

// STL includes
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
// AndGen includes
#include "..\Jobs\JobQueue.hpp"
#include "ThreadNotifier.hpp"
#include <AndGen/Exceptions/NotImplementedException.hpp>

namespace AndGen
{
	// Pre-declarations
	class Job;

	/// <summary>
	/// Thread within a <see cref="ThreadPool"/>
	/// </summary>
	class PooledThread
	{
	public:
		/// <summary>
		/// Constructs a new pooled thread
		/// </summary>
		/// <remarks>
		/// Constructs a new pooled thread, with no queued jobs. <see cref="QueueJob"/> with a valid job, 
		/// and <see cref="Start"/> will need to be called afterwards
		/// in order for the thread to be executing.
		/// </remarks>
		PooledThread() : m_shouldExit(false), m_isRunning(false) {}
		PooledThread(const PooledThread&) = delete;
		/// <summary>
		/// Destroys this pooled thread
		/// </summary>
		/// <remarks>
		/// De-constructs this pooled thread, but will finish executing the current job before doing so.
		/// Calls <see cref="Stop"/> before deconstruction.
		/// </remarks>
		~PooledThread() 
		{
			// Wait for internal thread to complete
			// any currently executing tasks
			Stop(true);
		}

		/// <summary>
		/// The currently executing job queue
		/// </summary>
		/// <returns>Reference to thread's job queue</returns>
		inline const JobQueue& GetQueue() const
		{
			return m_jobQueue;
		}

		/// <summary>
		/// Is this pooled thread currently executing?
		/// </summary>
		inline bool IsRunning() const
		{
			return m_isRunning;
		}

		/// <summary>
		/// Begins thread execution
		/// </summary>
		void Start();
		/// <summary>
		/// Stops thread execution
		/// </summary>
		void Stop(bool waitForThread = false);

		/// <summary>
		/// Waits for current queue to be executed by the thread
		/// </summary>
		void WaitForQueue();

		/// <summary>
		/// Removes all jobs from the execution queue
		/// </summary>
		inline void ClearQueue()
		{
			m_jobQueue.Clear();
		}

		/// <summary>
		/// Adds a job to the queue to be executed by the thread
		/// </summary>
		/// <typeparam name="JobType">Type of job</typeparam>
		template<class JobType>
		void QueueJob(const std::shared_ptr<JobType>& job)
		{
			// Ignore if job is null
			if (job == nullptr)
			{
				return;
			}

			// Add job to queue
			m_jobQueue.AddJob(std::static_pointer_cast<Job>(job));

			// Notify waiting internal thread there are jobs
			m_jobsReadyNotification.Notify();
		}

	private:
		// Queue of jobs for this thread to execute
		AndGen::JobQueue m_jobQueue;

		// Controls execution of the execution loop
		std::atomic_bool m_shouldExit;
		// Allows for checking if the thread is currently running
		std::atomic_bool m_isRunning;

		// Blocks internal thread execution when no jobs are to be completed
		// or application is shutting down, and only currently executing job should be executed
		AndGen::ThreadNotifier m_jobsReadyNotification;

		// Blocks calling threads of WaitForQueue() until all jobs in queue are completed
		AndGen::ThreadNotifier m_jobsCompleteNotification;

		// Execution thread
		std::thread m_thread;

		// Executes all jobs in the queue
		// and waits for a job to be added when none are left
		void ExecutionLoop();
	};
}

#endif
