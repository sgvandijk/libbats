#include "types.ih"

unsigned Types::unumFor(Types::Object objectId)
{
  if (Types::isOpponent(objectId))
    return objectId - Types::OPPONENT1 + 1;
  
  if (Types::isTeamMate(objectId))
    return objectId - Types::TEAMMATE1 + 1;
  
  return -1;
};