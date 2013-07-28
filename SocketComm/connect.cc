#include "socketcomm.ih"

void SocketComm::connect()
{
  // Connect to the server.
  d_socket->connect(*d_socketAddress);
  
  // Init connection.
  if (*d_socket)
    initConnection();
  else
    throw std::runtime_error("error connecting to server");
}
