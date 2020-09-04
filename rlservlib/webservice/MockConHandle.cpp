#include "MockConHandle.h"

namespace rls
{

	void MockConHandle::send(std::string str)
	{
		send_content.append(str);
	}

} // namespace rls