#include "socketcomm.ih"

void SocketComm::initSocket(std::string host, int port)
{
  d_socket.reset(new Socket(PF_INET,SOCK_STREAM, 0));
  d_socketAddress.reset(new SocketAddress(PF_INET, port, host));
}
