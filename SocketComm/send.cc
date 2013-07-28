#include "socketcomm.ih"

void SocketComm::send()
{
  string message;// = "";
  
  while (!d_oMessageQueue.empty())
  {
    message += d_oMessageQueue.front()->toString();
    d_oMessageQueue.pop();
  }

  if (!message.empty())// != "")
    sendMessage(message);
}
