#include "socketcomm.ih"

shared_ptr<Predicate> SocketComm::nextMessage()
{
  if (!hasNextMessage())
    return 0;

  shared_ptr<Predicate> msg = d_iMessageQueue.front();
  _debugLevel4("Message: " << *msg);
  d_iMessageQueue.pop();
  return msg;
}
