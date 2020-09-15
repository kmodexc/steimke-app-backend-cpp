#pragma once

#include "IHTTPSocket.h"

namespace rls
{

class SocketYhirose : public IHTTPSocket
{
	void* svr;
	IConHandler *handler;
	int port;
public:
	virtual void init(IConHandler *handler, int port) override;
	virtual void start() override;
	virtual void stop() override;
	virtual void close() override;
	SocketYhirose();
	virtual ~SocketYhirose();
};

} // namespace rls