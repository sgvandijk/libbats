#include "socketcomm.ih"

SocketComm::SocketComm()
  : d_parseInput(true)
{
  gettimeofday(t0,0);
  Parser::initialize();
}
    
SocketComm::SocketComm(std::string host, int port)
  : d_parseInput(true),
    d_socket(new Socket(PF_INET, SOCK_STREAM, 0)),
    d_socketAddress(new SocketAddress(PF_INET, port, host))
{
  gettimeofday(t0,0);
  Parser::initialize();
}
    
SocketComm::~SocketComm() {
}
