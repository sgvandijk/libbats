#include "types.ih"

double Types::dualOf(Types::Joint& dualJoint, Types::Joint joint)
{
  double scale = 1.0;
  switch (joint)
  {
    case Types::LARM1:
      dualJoint = Types::RARM1;
      break;
    case Types::LARM2:
      dualJoint = Types::RARM2;
      scale = -1.0;
      break;
    case Types::LARM3:
      dualJoint = Types::RARM3;
      break;
    case Types::LARM4:
      dualJoint = Types::RARM4;
      scale = -1.0;
      break;
    case Types::RARM1:
      dualJoint = Types::LARM1;
      break;
    case Types::RARM2:
      dualJoint = Types::LARM2;
      scale = -1.0;
      break;
    case Types::RARM3:
      dualJoint = Types::LARM3;
      break;
    case Types::RARM4:
      dualJoint = Types::LARM4;
      scale = -1.0;
      break;
    case Types::LLEG1:
      dualJoint = Types::RLEG1;
      break;
    case Types::LLEG2:
      dualJoint = Types::RLEG2;
      break;
    case Types::LLEG3:
      dualJoint = Types::RLEG3;
      scale = -1.0;
      break;
    case Types::LLEG4:
      dualJoint = Types::RLEG4;
      break;
    case Types::LLEG5:
      dualJoint = Types::RLEG5;
      break;
    case Types::LLEG6:
      dualJoint = Types::RLEG6;
      break;
    case Types::RLEG1:
      dualJoint = Types::LLEG1;
      break;
    case Types::RLEG2:
      dualJoint = Types::LLEG2;
      break;
    case Types::RLEG3:
      dualJoint = Types::LLEG3;
      scale = -1.0;
      break;
    case Types::RLEG4:
      dualJoint = Types::LLEG4;
      break;
    case Types::RLEG5:
      dualJoint = Types::LLEG5;
      break;
    case Types::RLEG6:
      dualJoint = Types::LLEG6;
      break;
    default:
      dualJoint = Types::NJOINTS;
  }
  return scale;
}
