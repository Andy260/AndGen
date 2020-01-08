#ifndef TIMED_JOB
#define TIMED_JOB

// AndGen Engine includes
#include <AndGen/Engine/Jobs/Job.hpp>

namespace AndGen::Tests
{
	class TimedJob : public AndGen::Job
	{
	public:
		TimedJob() : canExecute(false) {}
		TimedJob(const TimedJob&) = default;
		~TimedJob() = default;

		std::atomic_bool canExecute;

		std::atomic<std::thread::id> threadID;

		std::chrono::high_resolution_clock::time_point startOfExecution;
		std::chrono::high_resolution_clock::time_point endOfExecution;

	private:
		virtual void Execute() override
		{
			// Record executing thread ID
			threadID = std::this_thread::get_id();

			// Record execution start time
			startOfExecution = std::chrono::high_resolution_clock::now();

			// Wait until we can execute
			while (!canExecute) 
			{
				// Do nothing
			}

			// Record execution finish time
			endOfExecution = std::chrono::high_resolution_clock::now();
		}
	};
}

#endif
