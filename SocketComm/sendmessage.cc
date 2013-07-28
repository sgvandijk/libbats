#include "socketcomm.ih"

void SocketComm::sendMessage(std::string const &msg)
{
  // This could be more efficient if we used a member buffer...
  unsigned int len = htonl(msg.length());
  _debugLevel4("msg.length(): " << msg.length());
  
  //  std::cerr << "Size of unsigned int: " << sizeof(unsigned int) << std::endl;

  std::string prefix(reinterpret_cast<const char*>(&len),sizeof(unsigned int));
  std::string data = prefix + msg;

  d_socket->writeExactly(data.c_str(), data.length());
}
