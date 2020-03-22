#include "HTTPSocket.h"
#include <libhttp.h>

namespace rls
{

void HTTPSocket::init(IConHandler *handler, int port)
{
	memset(options, 0, sizeof(options));

	options[0].name = "listening_ports";
	options[0].value = "10002"; // **********todo*************

	callbacks.begin_request = HTTPSocket::begin_request_handler;
}

void HTTPSocket::run()
{
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
	char content[100];

	//HTTPSocket* sochet = (HTTPSocket*)(request_info->user_data);

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

	// Returning non-zero tells civetweb that our function has replied to
	// the client, and civetweb should not send client any more data.
	return 1;
}

} // namespace rls