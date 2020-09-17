#include "SocketYhirose.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../modules/cpp-httplib/httplib.h"
#include "MockConHandle.h"
#include "spdlog/spdlog.h"
namespace rls
{

	void SocketYhirose::init(IConHandler *handler, int port)
	{
		httplib::SSLServer *pserv = new httplib::SSLServer("../cert.pem", "../key.pem");
		if (handler != nullptr)
		{
			//const char *regexstr = R"((/api/)((item/\d+)|(place/\d+)|(user/\d+)|(items)|(places)|(users)|(ping)))";
			const char *regexstr = R"((/api/)(\S+))";
			pserv->Get(regexstr, [handler](const httplib::Request &req, httplib::Response &res) {
				if (req.has_header("Authorization"))
				{
					if (req.get_header_value("Authorization") == "Basic bXl1c2VyOm15cGFzcw==")
					{
						spdlog::get("rlservlib")->debug("authentication {}", req.get_header_value("Authorization"));
						MockConHandle conhandle;
						if (handler->get(&conhandle, req.path))
						{
							auto splitpos = conhandle.send_content.find("\r\n\r\n");
							if (splitpos > 0 && splitpos < 100)
							{
								conhandle.send_content = conhandle.send_content.substr(splitpos + 4);
							}
							res.set_content(conhandle.send_content, "application/json");
						}
						else
						{
							res.set_content("Hello World!", "text/plain");
						}
					}
					else
					{
						res.status = 401;
						res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
						res.set_content("FAIL", "text/plain");
					}
				}
				else
				{
					res.status = 401;
					res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
					res.set_content("FAIL", "text/plain");
				}
			});
			pserv->Put(regexstr, [handler](const httplib::Request &req, httplib::Response &res) {
				if (req.has_header("Authorization"))
				{
					if (req.get_header_value("Authorization") == "Basic bXl1c2VyOm15cGFzcw==")
					{
						spdlog::get("rlservlib")->debug("authentication {}", req.get_header_value("Authorization"));
						MockConHandle conhandle;
						if (handler->put(&conhandle, req.path,req.body))
						{
							auto splitpos = conhandle.send_content.find("\r\n\r\n");
							if (splitpos > 0 && splitpos < 100)
							{
								conhandle.send_content = conhandle.send_content.substr(splitpos + 4);
							}
							res.set_content(conhandle.send_content, "application/json");
						}
						else
						{
							res.set_content("Hello World!", "text/plain");
						}
					}
					else
					{
						res.status = 401;
						res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
						res.set_content("FAIL", "text/plain");
					}
				}
				else
				{
					res.status = 401;
					res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
					res.set_content("FAIL", "text/plain");
				}
			});
			pserv->Post(regexstr, [handler](const httplib::Request &req, httplib::Response &res) {
				if (req.has_header("Authorization"))
				{
					if (req.get_header_value("Authorization") == "Basic bXl1c2VyOm15cGFzcw==")
					{
						spdlog::get("rlservlib")->debug("authentication {}", req.get_header_value("Authorization"));
						MockConHandle conhandle;
						if (handler->post(&conhandle, req.path,req.body))
						{
							auto splitpos = conhandle.send_content.find("\r\n\r\n");
							if (splitpos > 0 && splitpos < 100)
							{
								conhandle.send_content = conhandle.send_content.substr(splitpos + 4);
							}
							res.set_content(conhandle.send_content, "application/json");
						}
						else
						{
							res.set_content("Hello World!", "text/plain");
						}
					}
					else
					{
						res.status = 401;
						res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
						res.set_content("FAIL", "text/plain");
					}
				}
				else
				{
					res.status = 401;
					res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
					res.set_content("FAIL", "text/plain");
				}
			});
			pserv->Delete(regexstr, [handler](const httplib::Request &req, httplib::Response &res) {
				if (req.has_header("Authorization"))
				{
					if (req.get_header_value("Authorization") == "Basic bXl1c2VyOm15cGFzcw==")
					{
						spdlog::get("rlservlib")->debug("authentication {}", req.get_header_value("Authorization"));
						MockConHandle conhandle;
						if (handler->del(&conhandle, req.path))
						{
							auto splitpos = conhandle.send_content.find("\r\n\r\n");
							if (splitpos > 0 && splitpos < 100)
							{
								conhandle.send_content = conhandle.send_content.substr(splitpos + 4);
							}
							res.set_content(conhandle.send_content, "application/json");
						}
						else
						{
							res.set_content("Hello World!", "text/plain");
						}
					}
					else
					{
						res.status = 401;
						res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
						res.set_content("FAIL", "text/plain");
					}
				}
				else
				{
					res.status = 401;
					res.set_header("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
					res.set_content("FAIL", "text/plain");
				}
			});
		}

		this->port = port;
		this->svr = (void *)pserv;
		this->handler = handler;
	}
	void SocketYhirose::start()
	{
		if (this->svr != nullptr)
		{
			auto th = new std::thread([this]() {
				httplib::Server *pserv = (httplib::Server *)this->svr;
				int p2 = this->port;
				spdlog::get("rlservlib")->debug("start yhirose on port {}", p2);
				pserv->listen("0.0.0.0", p2);
				std::this_thread::sleep_for(std::chrono::minutes(1));
			});
		}
	}
	void SocketYhirose::stop()
	{
		if (this->svr != nullptr)
		{
			httplib::Server *pserv = (httplib::Server *)this->svr;
			pserv->stop();
			delete pserv;
			this->svr = nullptr;
		}
	}
	void SocketYhirose::close()
	{
		if (this->svr != nullptr)
		{
			stop();
		}
	}
	SocketYhirose::SocketYhirose()
	{
		this->svr = nullptr;
		this->handler = nullptr;
		port = -1;
	}
	SocketYhirose::~SocketYhirose()
	{
		if (this->svr != nullptr)
		{
			httplib::Server *pserv = (httplib::Server *)this->svr;
			delete pserv;
			this->svr = nullptr;
		}
	}

} // namespace rls