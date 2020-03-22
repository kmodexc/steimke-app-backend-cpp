#include "ConHandle.h"

namespace rls
{

ConHandle::ConHandle(lh_ctx_t *ctx, lh_con_t *con)
{
	this->int_ctx = ctx;
	this->int_con = con;
}
void ConHandle::send(std::string str)
{
	httplib_printf(int_ctx, int_con,str.c_str());
}

} // namespace rls