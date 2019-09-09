#ifndef NOTIMPLEMENTEDEXCEPTION_H
#define	NOTIMPLEMENTEDEXCEPTION_H

#include <stdexcept>

namespace AndGen
{
	/// <summary>
	/// Thrown on functions which aren't implemented yet
	/// </summary>
	class NotImplementedException : public std::logic_error
	{
	public:
		NotImplementedException() : std::logic_error("Function not yet implemented") { }
	};
}

#endif
