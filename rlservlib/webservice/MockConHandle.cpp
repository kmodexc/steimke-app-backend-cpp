#include "MockConHandle.h"

namespace rls
{

	MockConHandle::MockConHandle()
	{
	}

	MockConHandle::~MockConHandle()
	{
	}

	void MockConHandle::send(std::string str)
	{
		send_content.append(str);
	}

} // namespace rls