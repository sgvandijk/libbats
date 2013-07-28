#include "socketcomm.ih"
#include "../WorldModel/worldmodel.hh"

void SocketComm::receive()
{
  // Read prefix
  unsigned len = d_socket->readExactly(reinterpret_cast<char*>(d_buffer), 4);
  if (len != 4)
    throw runtime_error("Failed reading prefix");

  int prefix = 0;
  memcpy(reinterpret_cast<char*>(&prefix), d_buffer, 4);
  prefix = ntohl(prefix);
  
  len = d_socket->readExactly(reinterpret_cast<char*>(d_buffer), prefix);
  if (len != prefix)
    throw runtime_error("Failed reading message");

  d_buffer[len] = 0;
  d_parser->reset();
  d_parser->parse(d_buffer, len);
  d_parser->parse('\n');
}
