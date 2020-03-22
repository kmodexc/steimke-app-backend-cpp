#include "HTTPSocket.h"
#include <libhttp.h>

namespace rls
{

HTTPSocket::HTTPSocket(){
	this->conhandler = nullptr;
	this->ctx = nullptr;
	memset(options, 0, sizeof(options));
	memset(&callbacks, 0, sizeof(callbacks));
	memset(str_port, 0, sizeof(str_port));
}

void HTTPSocket::init(IConHandler *handler, int port)
{
	this->conhandler = handler;
	memset(options, 0, sizeof(options));
	memset(&callbacks, 0, sizeof(callbacks));
	memset(str_port, 0, sizeof(str_port));

	options[0].name = "listening_ports";
	options[0].value = str_port;

	snprintf(str_port,sizeof(str_port),"%d",port);

	callbacks.begin_request = HTTPSocket::begin_request_handler;
}

void HTTPSocket::run()
{
	if(ctx != nullptr){
		std::cerr << "Already started a httpserver" << std::endl;
		return;
	}
	ctx = httplib_start(&callbacks, this, options);
	getchar();
}

void HTTPSocket::close()
{
	httplib_stop(ctx);
}

int HTTPSocket::begin_request_handler(lh_ctx_t *ctx, lh_con_t *con)
{
	const lh_rqi_t *request_info = httplib_get_request_info(con);

	HTTPSocket* socket = (HTTPSocket*)(request_info->user_data);

	/*
	// Prepare the message we're going to send
	int content_length = snprintf(content, sizeof(content),
								  "Hello from civetweb! Remote port: %d",
								  request_info->remote_port);

	// Send HTTP reply to the client
	httplib_printf(ctx, con,
				   "HTTP/1.1 200 OK\r\n"
				   "Content-Type: text/plain\r\n"
				   "Content-Length: %d\r\n" // Always set Content-Length
				   "\r\n"
				   "%s",
				   content_length, content);
	*/	
	ConHandle ich(ctx,con);
	if(socket->conhandler != nullptr){
		return socket->conhandler->get(&ich,request_info->local_uri);
	}

	// Returning non-zero tells civetweb that our function has replied to
	// the client, and civetweb should not send client any more data.
	return 0;
}

} // namespace rls