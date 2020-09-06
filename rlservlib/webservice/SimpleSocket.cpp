#include "SimpleSocket.h"
#include "spdlog/spdlog.h"

using namespace std;

namespace rls
{

SimpleSocket::SimpleSocket()
{
	sock = -1;
	mthread = nullptr;
	run_thread = false;
	handler = nullptr;
}

void SimpleSocket::init(IConHandler *handler, int port)
{
	this->handler = handler;
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("10.250.100.159");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1)
	{
		spdlog::get("rlservlib")->error("Socket creation error");
		return;
	}

	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		spdlog::get("rlservlib")->error("Bind error");
		::close(sock);
		return;
	}
}
void SimpleSocket::start()
{
	run_thread = true;
	mthread = new std::thread(main_thread, this);
	getchar();
	run_thread = false;
}
void SimpleSocket::stop()
{
	run_thread = false;
	if(mthread != nullptr && mthread->joinable())
		mthread->join();
}
void SimpleSocket::main_thread(SimpleSocket *mthis)
{
	if (listen(mthis->sock, 1 /*length of connections queue*/) == -1)
	{
		spdlog::get("rlservlib")->error("Listen error");
		::close(mthis->sock);
		return;
	}
	while (mthis->run_thread)
	{
		int client_sock = accept(mthis->sock, 0, 0);
		if (client_sock == -1)
		{
			spdlog::get("rlservlib")->error("Accept error");
			break;
		}

		spdlog::get("rlservlib")->info("Accepted client");

		SimpleConHandle con(client_sock);

		std::string str_header = con.recv();

		spdlog::get("rlservlib")->debug("Header: {}",str_header);

		HTTPHeader header = decodeHeader(str_header);

		if (header.valid)
		{
			std::string content;
			if (header.contentLength > 0)
			{
				content = con.recv(header.contentLength);
				spdlog::get("rlservlib")->debug("content: {}",content);
			}

			if (mthis->handler != nullptr && header.valid)
			{
				switch (header.method)
				{
				case HTTPMethod::GET:
					mthis->handler->get(&con, header.path);
					break;
				case HTTPMethod::POST:
					mthis->handler->post(&con, header.path, content);
					break;
				case HTTPMethod::PUT:
					mthis->handler->put(&con, header.path, content);
					break;
				case HTTPMethod::DELETE:
					mthis->handler->del(&con, header.path);
					break;
				}
			}
		}else{
			con.send("HTTP/1.1 400 Bad Header\r\n\r\n");
			spdlog::get("rlservlib")->error("invalid header");
		}

		::close(client_sock);
	}
}

HTTPHeader SimpleSocket::decodeHeader(std::string str_header)
{
	HTTPHeader header;
	header.valid = true;
	std::stringstream ss(str_header);

	std::string method;
	ss >> method >> header.path;
	if (method == "GET")
	{
		header.method = HTTPMethod::GET;
	}
	else if (method == "POST")
	{
		header.method = HTTPMethod::POST;
	}
	else if (method == "PUT")
	{
		header.method = HTTPMethod::PUT;
	}
	else if (method == "DELETE")
	{
		header.method = HTTPMethod::DELETE;
	}
	else
	{
		header.valid = false;
	}

	header.contentLength = -1;
	while (header.contentLength < 0 && ss.good())
	{
		ss >> method;
		if (method == "Content-Length:")
		{
			ss >> header.contentLength;
		}
	}
	if(!ss){
		header.contentLength = 0;
	}
	return header;
}
void SimpleSocket::close()
{
	if (run_thread)
	{
		run_thread = false;
		if (mthread->joinable())
			mthread->join();
		mthread = nullptr;
	}

	if (sock >= 0)
		::close(sock);
}

} // namespace rls
