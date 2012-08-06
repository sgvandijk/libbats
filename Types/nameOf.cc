#include "types.ih"

#define _case(name) case name: return #name

std::string Types::nameOf(Types::PlayMode const playMode)
{
  switch(playMode) {
      _case(BEFORE_KICKOFF);
      _case(KICKOFF_LEFT);
      _case(KICKOFF_RIGHT);
      _case(KICKOFF_US);
      _case(KICKOFF_THEM);
      _case(PLAY_ON);
      _case(FREEKICK_LEFT);
      _case(FREEKICK_RIGHT);
      _case(FREEKICK_US);
      _case(FREEKICK_THEM);
      _case(GOAL_LEFT);
      _case(GOAL_RIGHT);
      _case(GOAL_US);
      _case(GOAL_THEM);
      _case(GOAL_KICK_LEFT);
      _case(GOAL_KICK_RIGHT);
      _case(GOAL_KICK_US);
      _case(GOAL_KICK_THEM);
      _case(CORNER_KICK_LEFT);
      _case(CORNER_KICK_RIGHT);
      _case(CORNER_KICK_US);
      _case(CORNER_KICK_THEM);
      _case(KICKIN_LEFT);
      _case(KICKIN_RIGHT);
      _case(KICKIN_US);
      _case(KICKIN_THEM);
      _case(GAME_OVER);
      default:
		return "???";
  }     
}

std::string Types::nameOf(Types::Side const side)
{
  switch(side) {
	_case(LEFT);
	_case(RIGHT);
	default:
		return "???";
  }
}

std::string Types::nameOf(Types::Joint const joint)
{
  switch(joint) {
	_case(HEAD1);
	_case(HEAD2);
	_case(LLEG1);
	_case(LLEG2);
	_case(LLEG3);
	_case(LLEG4);
	_case(LLEG5);
	_case(LLEG6);
	_case(RLEG1);
	_case(RLEG2);
	_case(RLEG3);
	_case(RLEG4);
	_case(RLEG5);
	_case(RLEG6);
	_case(LARM1);
	_case(LARM2);
	_case(LARM3);
	_case(LARM4);
	_case(RARM1);
	_case(RARM2);
	_case(RARM3);
	_case(RARM4);
	_case(NJOINTS);
	default:
		return "???";
  }
}

std::string Types::nameOf(Types::JointType const jointType)
{
  switch(jointType) {
	_case(UNIVERSAL_JOINT);
	_case(HINGE_JOINT);
	_case(TORQUE_JOINT);
	_case(JOINTTYPE);	
	default:
		return "???";
  }
}



std::string Types::nameOf(Types::Object const object)
{
 switch(object) {
	_case(BALL);
	_case(TEAMMATE1);
	_case(TEAMMATE2);
	_case(TEAMMATE3);
	_case(TEAMMATE4);
  _case(TEAMMATE5);
  _case(TEAMMATE6);
  _case(TEAMMATE7);
  _case(TEAMMATE8);
  _case(TEAMMATE9);
  _case(TEAMMATE10);
  _case(TEAMMATE11);
	_case(OPPONENT1);
	_case(OPPONENT2);
	_case(OPPONENT3);
	_case(OPPONENT4);
  _case(OPPONENT5);
  _case(OPPONENT6);
  _case(OPPONENT7);
  _case(OPPONENT8);
  _case(OPPONENT9);
  _case(OPPONENT10);
  _case(OPPONENT11);
	_case(FLAG1US);
	_case(FLAG2US);
	_case(FLAG1THEM);
	_case(FLAG2THEM);
	_case(GOAL1US);
	_case(GOAL2US);
	_case(GOAL1THEM);
	_case(GOAL2THEM);
	_case(NOBJECTS);
	default:
		return "???";
}
}

std::string Types::nameOf(Types::BodyPart bodyPart)
{
 switch(bodyPart) {
	_case(HEAD);
	_case(NECK);
	_case(TORSO);
	_case(LOWERTORSO);
	_case(LSHOULDER);
	_case(LUPPERARM);
	_case(LELBOW);
	_case(LLOWERARM);
	_case(LHAND1);
	_case(LHAND2);
	_case(RSHOULDER);
	_case(RUPPERARM);
	_case(RELBOW);
	_case(RLOWERARM);
	_case(RHAND1);
	_case(RHAND2);
	_case(LHIP1);
	_case(LHIP2);
	_case(LUPPERLEG);
	_case(LLOWERLEG);
	_case(LANKLE);
	_case(LFOOT);
	_case(RHIP1);
	_case(RHIP2);
	_case(RUPPERLEG);
	_case(RLOWERLEG);
	_case(RANKLE);
	_case(RFOOT);
	_case(NBODYPARTS);
	default:
		return "???";
 } 
}

