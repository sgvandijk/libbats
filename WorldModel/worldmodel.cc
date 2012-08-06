#include "worldmodel.ih"

WorldModel::WorldModel()
  :
  d_time(0),
  d_teamName(""),
  d_gameTime(0),
  d_prevGameTime(0),
  d_timeStep(0.04),
  d_playMode(Types::UNKNOWN),
  d_lastPlayMode(Types::UNKNOWN),
  d_team(Types::UNKNOWN_SIDE),
  d_scoreUs(0),
  d_scoreThem(0),
  d_firstTime(-1),
  d_numMsgReceived(0),
  d_ballRadius(0.042)
{
  Conf& conf = bats::SConf::getInstance();
  d_fieldLength = conf.getParam("/fieldlength", 30.0);
  d_fieldWidth = conf.getParam("/fieldwidth", 20.0);
  d_goalWidth = conf.getParam("/goalwidth", 2.1);
  d_goalHeight = conf.getParam("/goalheight", 0.8);
  d_penaltyXLength = conf.getParam("/penaltyxlength", 1.8);
  d_penaltyYLength = conf.getParam("/penaltyylength", 3.9);
  d_numberOfPlayers = conf.getParam("/numberofplayers", 11);
  d_halfTimeRestart = conf.getParam("/halftimerestart", 1) == 1;
}
