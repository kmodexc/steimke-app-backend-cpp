#include "HTTPSocket.h"
#include <libhttp.h>

using namespace std;

namespace rls
{

HTTPSocket::HTTPSocket(){
	this->conhandler = nullptr;
	this->ctx = nullptr;
	memset(options, 0, sizeof(options));
	memset(&callbacks, 0, sizeof(callbacks));
	memset(str_port, 0, sizeof(str_port));
}

HTTPSocket::~HTTPSocket(){
	if(ctx != nullptr)
		close();
}

void HTTPSocket::init(IConHandler *handler, int port)
{
	this->conhandler = handler;
	memset(options, 0, sizeof(options));
	memset(&callbacks, 0, sizeof(callbacks));
	memset(str_port, 0, sizeof(str_port));

	options[0].name = "listening_ports";
	options[0].value = str_port;
	options[1].name = "ssl_certificate";
	options[1].value = "server.pem";
	options[2].name = "ssl_protocol_version";
	options[2].value = "3";

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
	ctx = nullptr;
}

int HTTPSocket::begin_request_handler(lh_ctx_t *ctx, lh_con_t *con)
{
	const lh_rqi_t *request_info = httplib_get_request_info(con);

	HTTPSocket* socket = (HTTPSocket*)(request_info->user_data);
	ConHandle ich(ctx,con);

	std::string req_method = request_info->request_method;

	cout << "Incoming " << req_method << " request" << endl;
	for(int cnt=0;cnt < request_info->num_headers ; cnt++)
		cout << "Header " << request_info->http_headers[cnt].name << "\t" << request_info->http_headers[cnt].value << endl;

	if(req_method == "GET"){
		if(socket->conhandler != nullptr){
			return socket->conhandler->get(&ich,request_info->local_uri);
		}
	}

	else if(req_method == "POST"){
		std::string content = getContent(ctx,con);
		if(socket->conhandler != nullptr){
			return socket->conhandler->post(&ich,request_info->local_uri,content);
		}
	}

	else if(req_method == "PUT"){
		std::string content  = getContent(ctx,con);
		if(socket->conhandler != nullptr){
			return socket->conhandler->put(&ich,request_info->local_uri,content);
		}
	}

	else if(req_method == "DELETE"){
		if(socket->conhandler != nullptr){
			return socket->conhandler->del(&ich,request_info->local_uri);
		}
	}

	// Returning non-zero tells civetweb that our function has replied to
	// the client, and civetweb should not send client any more data.
	return 0;
}

std::string HTTPSocket::getContent(lh_ctx_t *ctx,lh_con_t* con)
{
	std::string content;
	if(httplib_get_request_info(con)->content_length > 0){
		cout << "found data with len " << httplib_get_request_info(con)->content_length << endl;
		char buffer[100];
		memset(buffer,0,sizeof(buffer));
		while(httplib_read(ctx,con,buffer,sizeof(buffer)-2)){
			content += buffer;
			memset(buffer,0,sizeof(buffer));
		}
		cout << content << endl;
	}
	else cout << "no content" << endl;
	return content;
}

} // namespace rls