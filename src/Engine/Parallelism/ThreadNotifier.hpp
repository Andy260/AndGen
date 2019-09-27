#ifndef THREAD_NOTIFIER_H
#define THREAD_NOTIFIER_H

// STL includes
#include <mutex>
#include <atomic>
#include <thread>

namespace AndGen
{
	/// <summary>
	/// Wrapper class for condition variables
	/// </summary>
	class ThreadNotifier
	{
	public:
		/// <summary>
		/// Constructs a new Thread Notifier
		/// </summary>
		ThreadNotifier() : m_shouldWake(false) {}
		ThreadNotifier(const ThreadNotifier&)					= delete;
		/// <summary>
		/// Destroys this Thread Notifier
		/// </summary>
		~ThreadNotifier()										= default;
		ThreadNotifier& operator=(const ThreadNotifier& other)	= delete;

		/// <summary>
		/// Notifies all waiting threads
		/// </summary>
		void Notify();

		/// <summary>
		/// Blocks the calling thread until another thread notifies to wake up
		/// </summary>
		void Wait();

	private:
		// Used within condition variable for waiting threads
		std::atomic_bool m_shouldWake;
		// Used to notify threads when to wake up
		std::condition_variable m_conditionVariable;
		// Mutex for editing the condition variable
		std::mutex m_mutex;
	};
}

#endif
