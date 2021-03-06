#include "SocketEtr.h"
#include "MockConHandle.h"
#include "httpserver.hpp"
#include "spdlog/spdlog.h"
#include <thread>

namespace rls
{

	void custom_access_log(const std::string &url)
	{
		//std::cout << "ACCESSING: " << url << std::endl;
		spdlog::get("rlservlib")->debug("ACCESSING {}", url);
	}

	class http_resource : public httpserver::http_resource
	{
		IConHandler *handler;

	public:
		http_resource(IConHandler *handler)
		{
			this->handler = handler;
		}
		const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request &req)
		{
			spdlog::get("rlservlib")->info("incoming {} request", req.get_method());
			spdlog::get("rlservlib")->info("user '{}' pw '{}'", req.get_user(), req.get_pass());
			spdlog::get("rlservlib")->info("content {}", req.get_content());

			if (req.get_method() == "OPTIONS")
			{
				auto presp = std::shared_ptr<httpserver::http_response>(new httpserver::string_response("", 204));
				presp->with_header("Access-Control-Allow-Origin", "*");
				presp->with_header("Access-Control-Allow-Methods", "OPTIONS, GET, POST, PUT, DELETE");
				presp->with_header("Access-Control-Allow-Headers", "Authorization");
				presp->with_header("Access-Control-Max-Age", "86400");
				return presp;
			}

			if (req.get_user() != "myuser" || req.get_pass() != "mypass")
			{
				return std::shared_ptr<httpserver::basic_auth_fail_response>(new httpserver::basic_auth_fail_response("FAIL", ""));
			}

			MockConHandle handle;
			bool captured = false;
			if (req.get_method() == "GET")
			{
				captured = handler->get(&handle, req.get_path());
			}
			else if (req.get_method() == "POST")
			{
				captured = handler->post(&handle, req.get_path(), req.get_content());
			}
			else if (req.get_method() == "PUT")
			{
				captured = handler->put(&handle, req.get_path(), req.get_content());
			}
			else if (req.get_method() == "DELETE")
			{
				captured = handler->del(&handle, req.get_path());
			}
			else
			{
				spdlog::get("rlservlib")->error("unknown method {}", req.get_method());
			}
			std::string header = handle.send_content.substr(0, handle.send_content.find("\r\n\r\n"));
			bool ok = header.find("200") != std::string::npos;
			int respcode = (ok ? 200 : 500);
			std::string content = handle.send_content.substr(handle.send_content.find("\r\n\r\n") + 4);
			auto presp = std::shared_ptr<httpserver::http_response>(new httpserver::string_response(content, respcode, "application/json"));
			presp->with_header("Access-Control-Allow-Origin", "*");
			presp->with_header("Access-Control-Allow-Methods", "OPTIONS, GET, POST, PUT, DELETE");
			presp->with_header("Access-Control-Allow-Headers", "Authorization");
			presp->with_header("Access-Control-Max-Age", "86400");
			return presp;
		}
	};

	void SocketEtr::init(IConHandler *handler, int port)
	{
		this->port = port;
		this->handler = handler;
	}
	void SocketEtr::start()
	{
		if (this->pws != nullptr)
			return;

		std::thread *pth = new std::thread([this]() {
			httpserver::webserver ws = httpserver::create_webserver(this->port)
										   .single_resource()
										   .max_threads(8)
										   .use_ssl()
										   .https_mem_key("../key.pem")
										   .https_mem_cert("../cert.pem");

			http_resource hwr((IConHandler *)this->handler);
			ws.register_resource("/", &hwr, true);

			spdlog::get("rlservlib")->info("Starting eltr on Port {}", this->port);

			this->pws = &ws;

			ws.start(true);

			spdlog::get("rlservlib")->info("Server thread returning");
		});
	}
	void SocketEtr::stop()
	{
		if (this->pws != nullptr)
		{
			httpserver::webserver *mws = (httpserver::webserver *)pws;

			mws->sweet_kill();

			this->pws = nullptr;
		}
	}
	void SocketEtr::close()
	{
		this->stop();
	}
	SocketEtr::SocketEtr()
	{
		this->pws = nullptr;
	}
	SocketEtr::~SocketEtr()
	{
		this->close();
	}

} // namespace rls