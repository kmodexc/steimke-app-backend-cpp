#include "SocketEtr.h"
#include "httpserver.hpp"

namespace rls
{
	class hello_world_resource : public httpserver::http_resource
	{
	public:
		const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request &)
		{
			return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Hello, World!"));
		}
	};

	void SocketEtr::init(IConHandler *handler, int port)
	{
		this->port = port;
		this->handler = handler;
	}
	void SocketEtr::start()
	{
		if(this->pws != nullptr) return;

		httpserver::webserver ws = httpserver::create_webserver(this->port);

    	hello_world_resource *hwr = new hello_world_resource();
    	ws.register_resource("/api/ping", hwr);
		// ws.register_resource("/api/items", hwr);
		// ws.register_resource("/api/places", hwr);
		// ws.register_resource("/api/users", hwr);
		// ws.register_resource("/api/item/{argid}", hwr);
		// ws.register_resource("/api/place/{argid}", hwr);
		// ws.register_resource("/api/user/{argid}", hwr);
    	ws.start(false);

		this->pws = &ws;
	}
	void SocketEtr::stop()
	{
		if(this->pws != nullptr){
			httpserver::webserver* mws = (httpserver::webserver*)pws;

			mws->stop();

			delete mws;
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