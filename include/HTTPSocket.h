#pragma once
#include "IHTTPSocket.h"
#include "libhttp.h"
#include <string.h>
#include <thread>
#include <iostream>

namespace rls{

class HTTPSocket : public IHTTPSocket{
	
	// local variables for intern socket
private:
	lh_ctx_t *ctx;
	lh_clb_t callbacks;
	lh_opt_t options[2];
	char str_port[10];

	// local variables for interface to ui
private:
	IConHandler* conhandler;
	
	// intern functions
private:
	static int begin_request_handler(lh_ctx_t *ctx,lh_con_t *con);

	// public interface
public:
	HTTPSocket();
	void init(IConHandler *handler, int port) override;
	void run() override;
	void send(std::string str) override;
	void close() override;
};

}