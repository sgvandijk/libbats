#include "agentsocketcomm.ih"

void AgentSocketComm::update()
{
  // Add sync message to output queue
  syn();
  // Send all queued messages
  send();
  // Start reading
  receive();
}
