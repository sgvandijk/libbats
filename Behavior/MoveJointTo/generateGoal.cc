#include "movejointto.ih"

shared_ptr<Goal> MoveJointTo::generateGoal(unsigned step, unsigned slot)
{
  //WorldModel& wm = SWorldModel::getInstance();


  shared_ptr<Goal> goal = make_shared<Goal>();
  shared_ptr<OrNode> dis = goal->addDisjunct();
  shared_ptr<AndNode> con = dis->addConjunct();
  AgentModel& am = SAgentModel::getInstance();

	
  // Get our own goal.
  shared_ptr<StateVarNode> angleNode = static_pointer_cast<StateVarNode>(d_goal->findDeep("Angle"));
  shared_ptr<StateVarNode> speedNode = static_pointer_cast<StateVarNode>(d_goal->findDeep("Speed"));

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

    shared_ptr<StateVarNode> maxSpeedNode = static_pointer_cast<StateVarNode>(d_goal->findDeep("MaxSpeed"));
    double maxSpeed = 0;
	
    if (maxSpeedNode)
    {
      Range maxSpeedRange = maxSpeedNode->getVar().second;
      maxSpeed = maxSpeedRange.mean();
    }

    shared_ptr<NormalDistribution> angle = am.getJoint(d_joint)->angle;
    //double da = Math::normalizeRadBalanced(goalAngle - angle.getMu());
    double da = goalAngle - angle->getMu()(0);
	
    shared_ptr<StateVarNode> gainNode = static_pointer_cast<StateVarNode>(d_goal->findDeep("Gain"));
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

