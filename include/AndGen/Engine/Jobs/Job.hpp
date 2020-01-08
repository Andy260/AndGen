#ifndef JOB_H
#define JOB_H

// STL includes
#include <atomic>
#include <memory>
#include <list>
#include <vector>

namespace AndGen
{
	/// <summary>
	/// Abstract class representing a job to be executed by worker threads of the 
	/// job system
	/// </summary>
	class Job
	{
	public:
		/// <summary>
		/// De-constructs this job
		/// </summary>
		virtual ~Job() = default;

		/// <summary>
		/// Runs this job and flags this job as completed
		/// </summary>
		inline void Run()
		{
			// Execute the job (if not already completed)
			if (!m_isCompleted)
			{
				Execute();
				m_isCompleted = true;
			}
		}

		template<class JobType>
		void Schedule(std::shared_ptr<JobType> dependsOn);
		template<class JobType>
		void Schedule(std::vector<std::shared_ptr<JobType>> dependsOn);
		template<class JobType>
		void Schedule(std::list<std::shared_ptr<JobType>> dependsOn);

		/// <summary>
		/// Has this job been completed?
		/// </summary>
		inline bool IsCompleted() const
		{
			return m_isCompleted;
		}

	protected:
		/// <summary>
		/// Constructs a new job
		/// </summary>
		Job() : m_isCompleted(false) {}
		Job(const Job&) = delete;

		/// <summary>
		/// Work to be executed on the work threads
		/// </summary>
		virtual void Execute() = 0;

	private:
		// Is this job currently completed?
		std::atomic_bool m_isCompleted;

		// Jobs this job depends on being completed to execute
		std::vector<Job*> m_dependencies;

		void Internal_Schedule(std::shared_ptr<Job> dependsOn);
	};
}

#endif
