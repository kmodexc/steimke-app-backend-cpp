#pragma once
#include "IHTTPSocket.h"
#include "libhttp.h"
#include <string.h>
#include <thread>
#include <iostream>
#include "ConHandle.h"

namespace rls{

class HTTPSocket : public IHTTPSocket{
	
	// local variables for intern socket
private:
	lh_ctx_t *ctx;
	lh_clb_t callbacks;
	lh_opt_t options[5];
	char str_port[50];

	// local variables for interface to ui
private:
	IConHandler* conhandler;
	
	// intern functions
private:
	static int begin_request_handler(lh_ctx_t *ctx,lh_con_t *con);
protected:
	static std::string getContent(lh_ctx_t *ctx,lh_con_t* con);
	// public interface
public:
	HTTPSocket();
	~HTTPSocket() override;
	void init(IConHandler *handler, int port) override;
	void run() override;
	void close() override;
};

}