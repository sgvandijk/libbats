#include "localizer.ih"

Localizer::PlayerVector Localizer::getFilteredPlayers(std::function<bool(shared_ptr<PlayerInfo>)> pred, bool aliveOnly)
{
  PlayerVector filtered = PlayerVector();
  for (shared_ptr<PlayerInfo> player : d_players)
  {
    if (aliveOnly && !player->isAlive)
      continue;
    if (bool(pred(player)))
      filtered.push_back(player);
  }
  return filtered;
}
