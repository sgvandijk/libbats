#include "localizer.ih"

Localizer::PlayerVector Localizer::getFilteredPlayers(std::function<bool(rf<PlayerInfo>)> pred, bool aliveOnly)
{
  PlayerVector filtered = PlayerVector();
  for (rf<PlayerInfo> player : players)
  {
    if (aliveOnly && !player->isAlive)
      continue;
    if (bool(pred(player)))
      filtered.push_back(player);
  }
  return filtered;
}