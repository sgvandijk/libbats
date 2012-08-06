#include "trainer.ih"

void Trainer::update()
{
  Behavior::update();
  
  STrainerSocketComm::getInstance().update();
  
  _debugLevel1("(Trainer::update) stage: " << d_stage);
  
  WorldModel& wm = SWorldModel::getInstance();
  
  _debugLevel1(wm.getTime() << " " << d_runStartTime << " " << d_runTime);

  if (d_runStartTime == -1)
    d_runStartTime = wm.getTime() - d_runTime - 2;
  
  if (d_stage == START2)
    d_stage = START3;
    
  if (d_stage == START1)
    d_stage = START2;
      
  if (d_stage == RESET)
  {
    // Determine ball and agent start pos
    for (unsigned i = 0; i < 3; ++i)
    {
      d_ballStartPos[i] = d_minBallStartPos[i] + (d_maxBallStartPos[i] - d_minBallStartPos[i]) * rand() / (RAND_MAX + 1.0);
      d_ballStartVel[i] = d_minBallStartVel[i] + (d_maxBallStartVel[i] - d_minBallStartVel[i]) * rand() / (RAND_MAX + 1.0);
      d_agentStartPos[i] = d_minAgentStartPos[i] + (d_maxAgentStartPos[i] - d_minAgentStartPos[i]) * rand() / (RAND_MAX + 1.0);
    }
    _debugLevel1("(Trainer::update) Beam pos: " << d_agentStartPos);
    d_stage = START1;
  }
  
  _debugLevel1(wm.getTime() << " " << d_runStartTime << " " << d_runTime);
  
  if (d_stage == START3 && wm.getTime() - d_runStartTime >= d_runTime + 4)
  {
    d_runStartTime = wm.getTime();
    onRunStart();
    d_stage = RUN;
  }
  
  if (d_stage == STOP1)
    d_stage = STOP2;
    
  if (d_stage == RUN && (d_endRun || wm.getTime() - d_runStartTime >= d_runTime))
  {
    d_stage = STOP1;
    onRunEnd();
  }
  
  if (d_stage == STOP2 && wm.getTime() - d_runStartTime >= d_runTime + 2)
    d_stage = RESET;
}


