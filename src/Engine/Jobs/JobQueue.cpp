#include "JobQueue.hpp"

// STL includes
#include <queue>
// AndGen includes
#include <AndGen/Engine/Jobs/Job.hpp>

// Adds a job to the end of the queue
void AndGen::JobQueue::AddJob(std::shared_ptr<AndGen::Job> job)
{
	// Ensure we are given a valid job
	if (job == nullptr)
	{
		return;
	}

	// Acquire lock on queue
	std::scoped_lock<std::mutex> lock(m_jobQueue_mutex);
	// Add job to queue
	m_jobQueue.push(job);
}

// Executes the next job
void AndGen::JobQueue::ExecuteNextJob()
{
	// Get next job from the queue
	std::shared_ptr<Job> nextJob = GetNextJob();
	// Do nothing if we don't have a job to execute
	if (nextJob == nullptr)
	{
		return;
	}

	// Execute job on main thread
	nextJob->Run();
}

// Gets next job from the queue, if any
std::shared_ptr<AndGen::Job> AndGen::JobQueue::GetNextJob()
{
	// Return null pointer if no jobs are left
	if (m_jobQueue.size() <= 0)
	{
		return nullptr;
	}

	// Acquire lock on queue
	std::scoped_lock<std::mutex> lock(m_jobQueue_mutex);
	// Get pointer to next job and pop it from the queue
	std::shared_ptr<Job> nextJob = m_jobQueue.front();
	m_jobQueue.pop();

	return nextJob;
}

// Empties the queue
void AndGen::JobQueue::Clear()
{
	// Acquire lock on queue
	std::scoped_lock<std::mutex> lock(m_jobQueue_mutex);

	// Empty queue
	std::queue<std::shared_ptr<Job>> emptyQueue;
	m_jobQueue.swap(emptyQueue);
}
