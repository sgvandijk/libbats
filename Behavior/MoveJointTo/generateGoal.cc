#include "movejointto.ih"

rf<Goal> MoveJointTo::generateGoal(unsigned step, unsigned slot)
{
  //WorldModel& wm = SWorldModel::getInstance();


  rf<Goal> goal = new Goal();
  rf<OrNode> dis = goal->addDisjunct();
  rf<AndNode> con = dis->addConjunct();
  AgentModel& am = SAgentModel::getInstance();

	
  // Get our own goal.
  rf<StateVarNode> angleNode = rf_cast<StateVarNode>(d_goal->findDeep("Angle"));
  rf<StateVarNode> speedNode = rf_cast<StateVarNode>(d_goal->findDeep("Speed"));

  double goalAngle = 0;
  double speed = 0;
	
  if (!angleNode && speedNode)
  {
    speed = speedNode->getVar().second.mean();
  }
  else
  {
    if (angleNode)
      goalAngle = angleNode->getVar().second.mean();
    else
      goalAngle = am.getJoint(d_joint)->angle->getMu()(0);

    rf<StateVarNode> maxSpeedNode = rf_cast<StateVarNode>(d_goal->findDeep("MaxSpeed"));
    double maxSpeed = 0;
	
    if (maxSpeedNode)
    {
      Range maxSpeedRange = maxSpeedNode->getVar().second;
      maxSpeed = maxSpeedRange.mean();
    }

    rf<NormalDistribution> angle = am.getJoint(d_joint)->angle;
    //double da = Math::normalizeRadBalanced(goalAngle - angle.getMu());
    double da = goalAngle - angle->getMu()(0);
	
    rf<StateVarNode> gainNode = rf_cast<StateVarNode>(d_goal->findDeep("Gain"));
    double gain = d_gain;
	
    if (gainNode)
      gain = gainNode->getVar().second.mean();

    gain = 1;

    //#ifdef EXPSPEED	
    //	double speed = (exp(da) - 1) * gain;
    //#else
    //	double speed = gain * da;
//	#endif

    speed = gain * da;
    //double minspeed = 0.005;
    //if(speed < minspeed && speed > -minspeed)
    //	speed = 0.0;
    //cout << speed << "\n";
	
    if (maxSpeed > 0)
    {
      if (speed > maxSpeed)
	speed = maxSpeed;
			
      if (speed < -maxSpeed)
	speed = -maxSpeed;
    }
    else if (maxSpeed < 0)
      speed *= fabs(maxSpeed * 180.0 / M_PI);

  }

  con->addVar("Speed", speed, speed);
  con->addVar("Joint", d_joint, d_joint);
	
  return goal;
}

