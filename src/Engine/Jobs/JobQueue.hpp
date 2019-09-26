#ifndef JOBQUEUE_H
#define JOBQUEUE_H

// STL includes
#include <memory>
#include <mutex>
#include <deque>

namespace AndGen
{
	// Pre-declarations
	class Job;

	/// <summary>
	/// Queue of jobs to be executed on a thread
	/// </summary>
	class JobQueue
	{
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		JobQueue() = default;
		/// <summary>
		/// Copy constructor
		/// </summary>
		JobQueue(const JobQueue& other)
		{
			m_jobQueue = other.m_jobQueue;
		}
		/// <summary>
		/// Default de-constructor
		/// </summary>
		~JobQueue()	= default;

		/// <summary>
		/// Adds a job to the end of the queue
		/// </summary>
		/// <param name="job">Pointer to job, which will be added to the queue</param>
		void AddJob(std::shared_ptr<Job> job);
		/// <summary>
		/// Adds all jobs for another queue into this queue
		/// </summary>
		/// <param name="jobQueue">Job Queue to add jobs from to this queue</param>
		void AddJobQueue(const JobQueue& jobQueue);

		/// <summary>
		/// Executes the next job in the queue
		/// </summary>
		void ExecuteNextJob();
		/// <summary>
		/// Amount of jobs left in the queue
		/// </summary>
		inline int Count() const
		{
			return static_cast<int>(m_jobQueue.size());
		}
		/// <summary>
		/// Has the queue got no jobs?
		/// </summary>
		inline bool IsEmpty() const
		{
			return m_jobQueue.empty();
		}

		/// <summary>
		/// Empties the queue
		/// </summary>
		void Clear();

	private:
		// Queue of jobs to execute
		std::deque<std::shared_ptr<Job>> m_jobQueue;
		// Mutex to ensure thread safety when accessing m_jobQueue
		std::mutex m_jobQueue_mutex;

		/// <summary>
		/// Gets next job from the queue, if any
		/// </summary>
		/// <returns>
		/// Next job in queue, or null if no jobs left
		/// </returns>
		std::shared_ptr<Job> GetNextJob();
	};
}

#endif
