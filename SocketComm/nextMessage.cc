#include "socketcomm.ih"

rf<Predicate> SocketComm::nextMessage()
{
  if (!hasNextMessage())
    return 0;

  rf<Predicate> msg = d_iMessageQueue.front();
  _debugLevel4("Message: " << *msg);
  d_iMessageQueue.pop();
  return msg;
}
