#include "IConHandle.h"

namespace rls
{
	class MockConHandle : public IConHandle
	{
	public:
		MockConHandle();
		virtual ~MockConHandle();
		virtual void send(std::string str) override;
		std::string send_content;
	};
} // namespace rls