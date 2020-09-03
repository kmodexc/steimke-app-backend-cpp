#pragma once
#include "IConHandle.h"
#include "libhttp.h"

namespace rls{

class ConHandle : public IConHandle{
private:
	lh_ctx_t *int_ctx;
	lh_con_t *int_con;

public:
	ConHandle(lh_ctx_t*,lh_con_t*);
	void send(std::string str) override;
};

}