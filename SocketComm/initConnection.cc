#include "socketcomm.ih"

void SocketComm::initConnection()
{
  d_socket->setBlocking(true);
  d_socket->setsockopt(IPPROTO_TCP, TCP_NODELAY, true);
}
