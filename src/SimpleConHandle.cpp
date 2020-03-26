#include "SimpleConHandle.h"
#include <iostream>

using namespace std;

namespace rls
{

SimpleConHandle::SimpleConHandle(int msocfd)
{
	this->sockfd = msocfd;
}
void SimpleConHandle::send(std::string str)
{
	::send(sockfd, str.c_str(), str.length(),0);
}
std::string SimpleConHandle::recv(int maxlen)
{
	char buffer[200];
	char *it = buffer;
	memset(buffer, 0, sizeof(buffer));
	bool cr = false;
	::recv(sockfd, it, 1,0);
	while (((it - buffer) < (maxlen - 1)) && ((*it >= 32 && *it < 127) || (*it == '\n' || *it == '\r')))
	{
		if (cr && *it == '\n')
			break;
		if (*it == '\n' || (cr && *it == '\r'))
			cr = true;
		else
			cr = false;
		::recv(sockfd, ++it, 1,0);
	}
	std::string ret = buffer;
	return ret;
}
std::string SimpleConHandle::recv()
{
	return recv(200);
}

} // namespace rls