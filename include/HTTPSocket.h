#pragma once
#include "IHTTPSocket.h"
#include "libhttp.h"
#include <string.h>
#include <thread>

namespace rls{

class HTTPSocket : public IHTTPSocket{
private:
	lh_ctx_t *ctx;
	lh_clb_t callbacks;
	lh_opt_t options[2];

public:
	void init(IConHandler *handler, int port);
	static int begin_request_handler(lh_ctx_t *ctx,lh_con_t *con);
	void run();
	void close();
};

}