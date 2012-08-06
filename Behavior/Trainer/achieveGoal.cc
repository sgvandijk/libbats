#include "trainer.ih"

bool Trainer::achieveGoal()
{
  Cerebellum& cer = SCerebellum::getInstance();
  TrainerSocketComm& comm = STrainerSocketComm::getInstance();

  _debugLevel1("(Trainer::achieveGoal) stage: " << d_stage);
  
  switch (d_stage)
  {
    case RESET:
      ++d_run;
      if (d_runs > 0 && d_run > d_runs)
        HumanoidAgent::die();
      cout << "resetting!" << endl;
      comm.setPlayMode(Types::BEFORE_KICKOFF);
      reset();
      d_endRun = false;
      break;
    
    case START1:
    	cer.addAction(new BeamAction(d_agentStartPos));
      resetJoints();
      break;
      
    case START2:
      _debugLevel1("Setting ball at: " << d_ballStartPos);
      
      // Set ball at start pos
      comm.setBallPosVel(d_ballStartPos, d_ballStartVel);
      
      comm.setPlayMode(d_playMode);
      resetJoints();
      break;

    case START3:
    	cer.addAction(new BeamAction(d_agentStartPos));
      resetJoints();
      break;
      
    case STOP1:
      comm.setPlayMode(Types::GAME_OVER);
    
    case STOP2:
      resetJoints();
      break;
      
    case RUN:
      onStep();
      return Behavior::achieveGoal();
  }
  //assert(false);//Must hit one of the stages, or we have a BIG problem
  return true;
}

