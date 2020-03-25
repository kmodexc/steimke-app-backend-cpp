#include "SimpleConHandle.h"

namespace rls
{

SimpleConHandle::SimpleConHandle(int socfd)
{
	this->sockfd = sockfd;
}
void SimpleConHandle::send(std::string str)
{
	write(sockfd, str.c_str(), str.length());
}
std::string SimpleConHandle::recv(int maxlen)
{
	char buffer[200];
	char *it = buffer;
	memset(buffer, 0, sizeof(buffer));
	bool cr = false;
	read(sockfd, it, 1);
	while (((it - buffer) < maxlen) && ((*it >= 32 && *it < 127) || (*it == '\n' || *it == '\n')))
	{
		if (cr && *it == '\n')
			break;
		if (*it == '\n' || (cr && *it == '\r'))
			cr = true;
		else
			cr = false;
		read(sockfd, ++it, 1);
	}
	std::string ret = buffer;
	return ret;
}
std::string SimpleConHandle::recv()
{
	return recv(200);
}

} // namespace rls