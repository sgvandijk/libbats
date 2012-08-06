#include "worldmodel.ih"

void WorldModel::updateGameState()
{
  Cochlea& cochlea = SCochlea::getInstance();
  Clock& clock = SClock::getInstance();

  // Save which team we are
  if (d_team == Types::UNKNOWN_SIDE && (clock.getTime() - cochlea.getTimestamp(Cochlea::iTeam)) < 0.005)
    d_team = (Types::Side)cochlea.getInfo(Cochlea::iTeam).x();

  // Update game time
  if (cochlea.getInfo(Cochlea::iGameTime)[0] != 0)
  {
    d_prevGameTime = d_gameTime;
    d_gameTime = cochlea.getInfo(Cochlea::iGameTime)[0];
    d_timeStep = d_gameTime - d_prevGameTime;
  }

  _debugLevel1("playmode: " << cochlea.getInfo(Cochlea::iPlayMode));

  if (cochlea.getInfo(Cochlea::iPlayMode)[0] != 0)
  {
    d_lastPlayMode = d_playMode;

    Types::PlayMode mode = (Types::PlayMode)cochlea.getInfo(Cochlea::iPlayMode)[0];

    switch (mode)
    {
      case Types::KICKOFF_LEFT:
        d_playMode = d_team == Types::LEFT ? Types::KICKOFF_US : Types::KICKOFF_THEM;
        break;

      case Types::KICKOFF_RIGHT:
        d_playMode = d_team == Types::RIGHT ? Types::KICKOFF_US : Types::KICKOFF_THEM;
        break;

      case Types::FREEKICK_LEFT:
        d_playMode = d_team == Types::LEFT ? Types::FREEKICK_US : Types::FREEKICK_THEM;
        break;

      case Types::FREEKICK_RIGHT:
        d_playMode = d_team == Types::RIGHT ? Types::FREEKICK_US : Types::FREEKICK_THEM;
        break;

      case Types::GOAL_LEFT:
        d_playMode = d_team == Types::LEFT ? Types::GOAL_US : Types::GOAL_THEM;
        if (d_playMode != d_lastPlayMode) {
          if (d_team == Types::LEFT)
            ++d_scoreUs;
          else
            ++d_scoreThem;
		}
        break;

      case Types::GOAL_RIGHT:
        d_playMode = d_team == Types::RIGHT ? Types::GOAL_US : Types::GOAL_THEM;
        if (d_playMode != d_lastPlayMode) {
          if (d_team == Types::RIGHT)
            ++d_scoreUs;
          else
            ++d_scoreThem;
		}
        break;

      case Types::GOAL_KICK_LEFT:
        d_playMode = d_team == Types::LEFT ? Types::GOAL_KICK_US : Types::GOAL_KICK_THEM;
        break;

      case Types::GOAL_KICK_RIGHT:
        d_playMode = d_team == Types::RIGHT ? Types::GOAL_KICK_US : Types::GOAL_KICK_THEM;
        break;

      case Types::CORNER_KICK_LEFT:
        d_playMode = d_team == Types::LEFT ? Types::CORNER_KICK_US : Types::CORNER_KICK_THEM;
        break;

      case Types::CORNER_KICK_RIGHT:
        d_playMode = d_team == Types::RIGHT ? Types::CORNER_KICK_US : Types::CORNER_KICK_THEM;
        break;

      case Types::KICKIN_LEFT:
        d_playMode = d_team == Types::LEFT ? Types::KICKIN_US : Types::KICKIN_THEM;
        break;

      case Types::KICKIN_RIGHT:
        d_playMode = d_team == Types::RIGHT ? Types::KICKIN_US : Types::KICKIN_THEM;
        break;

      default:
        d_playMode = mode;
        break;
    }

  }

  if (cochlea.getInfo(Cochlea::iNow)[0] != 0)
  {
    d_time = SClock::getInstance().getTime();
    if (d_firstTime < 0)
      d_firstTime = d_time;
  }
  
  if (d_playMode == Types::GOAL_THEM)
  {
    d_weGetKickOff = true;
  }
  else if (d_playMode == Types::BEFORE_KICKOFF)
  {
    if (d_halfTimeRestart)
    {
      // half time will be at either 300 seconds or 180 seconds (during extra time)
      d_weGetKickOff = (d_team == Types::LEFT && (d_gameTime < 1 || (d_gameTime >= 300 && d_gameTime <= 301) || (d_gameTime >= 180 && d_gameTime <= 181)));
    }
    else
    {
      d_weGetKickOff = (d_team == Types::LEFT && d_gameTime == 0) || (d_team == Types::RIGHT && d_gameTime > 0);
    }
  }
  _debugLevel1("We get kickoff: " << (d_weGetKickOff ? "yes" : "no"));
}

