#include "HTTPSocket.h"
#include <libhttp.h>
#include <chrono>
#include <thread>
#include <limits>
#include "spdlog/spdlog.h"
#include <iostream>

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
	//options[1].name = "ssl_certificate";
	//options[1].value = "server.pem";
	// options[2].name = "ssl_protocol_version";
	// options[2].value = "3";

	// configure port for http and port+1 for https
	//snprintf(str_port,sizeof(str_port),"%d, %ds",port,(port+1));
	// alternativ without https
	snprintf(str_port,sizeof(str_port),"%d",port);

	callbacks.begin_request = HTTPSocket::begin_request_handler;

	spdlog::get("rlservlib")->info("Initialized Socket on port {}",str_port);
}

void HTTPSocket::run()
{
	if(ctx != nullptr){
		spdlog::get("rlservlib")->error("Already started a httpserver");
		return;
	}
	ctx = httplib_start(&callbacks, this, options);
	std::string line;
	std::getline(std::cin,line);
	while(line.find("quit rlserv") != 0){
		std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::minutes(1));
	}
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

	spdlog::get("rlservlib")->debug("Incoming {} request",req_method);
	for(int cnt=0;cnt < request_info->num_headers ; cnt++){
		auto header_prop = request_info->http_headers[cnt];
		spdlog::get("rlservlib")->debug("HEADER {}   VALUE: {}",header_prop.name,header_prop.value);
	}		

	// if path has apk in it its probably getting the file
	std::string path = request_info->local_uri;
	if(path.find(".apk")!=std::string::npos){
		// set 0 so libhttp sends file
		return 0;
	}

	if(req_method == "GET"){
		if(socket->conhandler != nullptr){
			socket->conhandler->get(&ich,request_info->local_uri);
		}
	}

	else if(req_method == "POST"){
		std::string content = getContent(ctx,con);
		if(socket->conhandler != nullptr){
			socket->conhandler->post(&ich,request_info->local_uri,content);
		}
	}

	else if(req_method == "PUT"){
		std::string content  = getContent(ctx,con);
		if(socket->conhandler != nullptr){
			socket->conhandler->put(&ich,request_info->local_uri,content);
		}
	}

	else if(req_method == "DELETE"){
		if(socket->conhandler != nullptr){
			socket->conhandler->del(&ich,request_info->local_uri);
		}
	}	

	return 1;	// make shure that httpserver doesent try to load something
}

std::string HTTPSocket::getContent(lh_ctx_t *ctx,lh_con_t* con)
{
	std::string content;
	if(httplib_get_request_info(con)->content_length > 0){
		spdlog::get("rlservlib")->debug("found data with len {}",httplib_get_request_info(con)->content_length);
		char buffer[100];
		memset(buffer,0,sizeof(buffer));
		while(httplib_read(ctx,con,buffer,sizeof(buffer)-2)){
			content += buffer;
			memset(buffer,0,sizeof(buffer));
		}
		spdlog::get("rlservlib")->debug("content: {}",content);
	}
	else spdlog::get("rlservlib")->debug("no content");
	return content;
}

} // namespace rls
