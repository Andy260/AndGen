// STL includes
#include <sstream>
#include <vector>
// Google Test includes
#include <gtest/gtest.h>
// AndGen includes
#include "AndGen/Engine/CommandLineArguments.hpp"

namespace AndGen::Tests
{
	// Test Constructor with command line arguments
	TEST(CommandLineArgumentsTests, Constructor)
	{
		char* args[] = { "-firstArgument", "-secondArgument" };

		// Create Command Line Arguments object
		CommandLineArguments arguments = CommandLineArguments(2, args);

		// Ensure created object has expected values
		for (size_t i = 0; i < 2; i++)
		{
			ASSERT_STREQ(args[i], arguments[i].c_str());
		}
	}

	// Constructor with no command line arguments
	TEST(CommandLineArgumentsTests, ConstructorNoArguments)
	{
		// Create object
		CommandLineArguments arguments = CommandLineArguments(0, nullptr);

		// Ensure created object has expected values
		ASSERT_EQ(arguments.Count(), 0);
	}

	// Constructor with invalid arguments
	TEST(CommandLineArgumentsTests, ConstructorInvalidArguments)
	{
		char* args[] = { "-firstArgument", "-secondArgument" };

		// Negative argument count
		ASSERT_THROW(CommandLineArguments(-1, nullptr), std::invalid_argument);

		// Null argument value pointer
		ASSERT_THROW(CommandLineArguments(2, nullptr), std::invalid_argument);

		// Too small argument count
		CommandLineArguments argument = CommandLineArguments(1, args);
		ASSERT_EQ(argument.Count(), 1);
		ASSERT_STREQ(argument[0].c_str(), args[0]);

		// Argument value contains a NULL value
		char* argsWithNull[] = { nullptr, "-secondArgument", nullptr, "-fourthArgument", nullptr };
		CommandLineArguments argumentWithNullValue = CommandLineArguments(5, argsWithNull);
		ASSERT_EQ(argumentWithNullValue.Count(), 2);
		ASSERT_STREQ(argumentWithNullValue[0].c_str(), argsWithNull[1]);
		ASSERT_STREQ(argumentWithNullValue[1].c_str(), argsWithNull[3]);
	}

	// GetAllArguments() with command line arguments
	TEST(CommandLineArgumentsTests, GetAllArguments)
	{
		char* args[] = { "-firstArgument", "-secondArgument" };

		// Create Command Line Arguments object
		CommandLineArguments arguments = CommandLineArguments(2, args);

		// Ensure AllArguments() returns expected value
		const std::vector<std::string>& allArguments = arguments.GetAllArguments();

		// Ensure created object has expected values
		ASSERT_EQ(allArguments.size(), 2);
		for (size_t i = 0; i < allArguments.size(); i++)
		{
			ASSERT_STREQ(allArguments[i].c_str(), args[i]);
		}
	}

	// Count() with command line arguments
	TEST(CommandLineArgumentsTests, Count)
	{
		char* args[] = { "-firstArgument", "-secondArgument" };

		// Create Command Line Arguments object
		CommandLineArguments arguments = CommandLineArguments(2, args);

		// Ensure created object has expected values
		ASSERT_EQ(arguments.Count(), 2);
	}

	// HasArgument() with valid argument
	TEST(CommandLineArgumentsTests, HasArgument)
	{
		char* args[] = { "-firstArgument", "-secondArgument" };

		// Create Command Line Arguments object
		CommandLineArguments arguments = CommandLineArguments(2, args);

		// Ensure created object has expected values
		ASSERT_TRUE(arguments.HasArgument(std::string(args[0])));
		ASSERT_TRUE(arguments.HasArgument(std::string(args[1])));
	}

	// HasArgument() with an invalid argument
	TEST(CommandLineArgumentsTests, HasArgumentInvalid)
	{
		char* args[] = { "-firstArgument", "-secondArgument" };

		// Create Command Line Arguments object
		CommandLineArguments arguments = CommandLineArguments(2, args);

		// Ensure created object has expected values
		ASSERT_FALSE(arguments.HasArgument(std::string("-thirdArgument")));
		ASSERT_FALSE(arguments.HasArgument(std::string()));
	}

	// operator[] with a valid index
	TEST(CommandLineArgumentsTests, SubscriptOperator)
	{
		char* args[] = { "-firstArgument", "-secondArgument" };

		// Create Command Line Arguments object
		CommandLineArguments arguments = CommandLineArguments(2, args);

		// Ensure created object has expected values
		ASSERT_STREQ(arguments[0].c_str(), args[0]);
		ASSERT_STREQ(arguments[1].c_str(), args[1]);
	}

	// operator[] with an index out of bounds
	TEST(CommandLineArgumentsTests, SubscriptOperatorOutofBounds)
	{
		char* args[] = { "-firstArgument", "-secondArgument" };

		// Create Command Line Arguments object
		CommandLineArguments arguments = CommandLineArguments(2, args);

		// Ensure an exception is thrown for values out of range
		ASSERT_THROW(arguments[-1], std::out_of_range);
		ASSERT_THROW(arguments[2], std::out_of_range);
	}
}
