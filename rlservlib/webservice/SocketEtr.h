#pragma once
#include "IHTTPSocket.h"

namespace rls
{

	class SocketEtr : public IHTTPSocket
	{
		void *pws;
		int port;
		IConHandler* handler;
	public:
		virtual void init(IConHandler *handler, int port) override;
		virtual void start() override;
		virtual void stop() override;
		virtual void close() override;
		SocketEtr();
		virtual ~SocketEtr();
	};

} // namespace rls