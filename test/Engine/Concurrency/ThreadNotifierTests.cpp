#include <Engine/Concurrency/ThreadNotifier.hpp>

// STL includes
#include <thread>
// Google Test includes
#include <gtest/gtest.h>

namespace AndGen
{
	namespace UnitTests
	{
		// Normal usage of Notify()
		TEST(ThreadNotifierTests, Notify)
		{
			AndGen::ThreadNotifier threadNotifier;

			// Start waiting thread
			bool notified		= false;
			bool waitSuccessful = false;
			std::thread waitingThread([&threadNotifier, &waitSuccessful] 
			{
				threadNotifier.Wait();
				waitSuccessful = true;
			});
			// Start notifying thread
			std::thread notifyingThread([&threadNotifier, &notified] 
			{
				threadNotifier.Notify();
				notified = true;
			});

			// Wait for threads
			if (waitingThread.joinable())
			{
				waitingThread.join();
			}
			if (notifyingThread.joinable())
			{
				notifyingThread.join();
			}

			// Ensure operation completed as expected
			ASSERT_TRUE(notified);
			ASSERT_TRUE(waitSuccessful);
		}

		// Normal usage of Wait()
		TEST(ThreadNotifierTests, Wait)
		{
			AndGen::ThreadNotifier threadNotifier;

			// Start waiting thread
			bool notified = false;
			bool waitSuccessful = false;
			std::thread waitingThread([&threadNotifier, &waitSuccessful]
			{
				threadNotifier.Wait();
				waitSuccessful = true;
			});
			// Start notifying thread
			std::thread notifyingThread([&threadNotifier, &notified]
			{
				threadNotifier.Notify();
				notified = true;
			});

			// Wait for threads
			if (waitingThread.joinable())
			{
				waitingThread.join();
			}
			if (notifyingThread.joinable())
			{
				notifyingThread.join();
			}

			// Ensure operation completed as expected
			ASSERT_TRUE(notified);
			ASSERT_TRUE(waitSuccessful);
		}
	}
}
