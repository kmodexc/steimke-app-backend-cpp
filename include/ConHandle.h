#pragma once
#include "IConHandle.h"
#include "libhttp.h"

namespace rls{

class ConHandle : public IConHandle{
public:
	lh_ctx_t *int_ctx;
	lh_con_t *int_con;
};

}