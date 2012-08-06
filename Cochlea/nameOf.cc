#include "cochlea.ih"

#define _case(name) case name: return #name

std::string Cochlea::nameOf(Cochlea::InfoID const infoId)
{
  switch(infoId) {
    _case(iNone);

    _case(iGameState);
    _case(iUnum);
    _case(iTeam);
    _case(iGameTime);
    _case(iPlayMode);
    
    _case(iTime);
    _case(iNow);
    _case(iStep);
    _case(iVision);
    _case(iVisionFlag1L);
    _case(iVisionFlag2L);
    _case(iVisionFlag1R);
    _case(iVisionFlag2R);
    _case(iVisionGoal1L);
    _case(iVisionGoal2L);
    _case(iVisionGoal1R);
    _case(iVisionGoal2R);
    _case(iVisionTeamMate1);
    _case(iVisionTeamMate2);
    _case(iVisionTeamMate3);
    _case(iVisionTeamMate4);
    _case(iVisionTeamMate5);
    _case(iVisionTeamMate6);
    _case(iVisionTeamMate7);
    _case(iVisionTeamMate8);
    _case(iVisionTeamMate9);
    _case(iVisionTeamMate10);
    _case(iVisionTeamMate11);
    _case(iVisionPlayerNone);
    _case(iVisionOpponent1);
    _case(iVisionOpponent2);
    _case(iVisionOpponent3);
    _case(iVisionOpponent4);
    _case(iVisionOpponent5);
    _case(iVisionOpponent6);
    _case(iVisionOpponent7);
    _case(iVisionOpponent8);
    _case(iVisionOpponent9);
    _case(iVisionOpponent10);
    _case(iVisionOpponent11);
    _case(iVisionOpponentNone);
    _case(iVisionBall);
    _case(iVisionLine);
    _case(iVisionSelfGT);
    _case(iVisionBallGT);
    _case(iUJ);
    _case(iHJ);
    _case(iHJT);
    _case(iName);
    _case(iAxis);
    _case(iRate);
    _case(iTorque);
    _case(iAxis1);
    _case(iRate1);
    _case(iAxis2);
    _case(iRate2);
    
    _case(iHeadJoint1);
    _case(iHeadJoint2);
    
    _case(iLLegJoint1);
    _case(iLLegJoint2);
    _case(iLLegJoint3);
    _case(iLLegJoint4);
    _case(iLLegJoint5);
    _case(iLLegJoint6);
    
    _case(iRLegJoint1);
    _case(iRLegJoint2);
    _case(iRLegJoint3);
    _case(iRLegJoint4);
    _case(iRLegJoint5);
    _case(iRLegJoint6);
    
    _case(iLArmJoint1);
    _case(iLArmJoint2);
    _case(iLArmJoint3);
    _case(iLArmJoint4);
    
    _case(iRArmJoint1);
    _case(iRArmJoint2);
    _case(iRArmJoint3);
    _case(iRArmJoint4);
    
    _case(iGyro);
    _case(iTorsoGyro);
    _case(iTouch);
    _case(iFootLeft);
    _case(iFootLeft2);
    _case(iFootRight);
    _case(iFootRight2);
    _case(iVal);
    
    _case(iForceResistancePerceptor);
    _case(iContact);
    _case(iForce);
    
    _case(iHear);

    _case(iP);

    _case(iBody);

    _case(iBodyHead);
    _case(iBodyRLowerArm);
    _case(iBodyLLowerArm);
    _case(iBodyRFoot);
    _case(iBodyLFoot);
    
    _case(iRLowerArmTeamMate1);
    _case(iRLowerArmTeamMate2);
    _case(iRLowerArmTeamMate3);
    _case(iRLowerArmTeamMate4);
    _case(iRLowerArmTeamMate5);
    _case(iRLowerArmTeamMate6);
    _case(iRLowerArmTeamMate7);
    _case(iRLowerArmTeamMate8);
    _case(iRLowerArmTeamMate9);
    _case(iRLowerArmTeamMate10);
    _case(iRLowerArmTeamMate11);
    
    _case(iLLowerArmTeamMate1);
    _case(iLLowerArmTeamMate2);
    _case(iLLowerArmTeamMate3);
    _case(iLLowerArmTeamMate4);
    _case(iLLowerArmTeamMate5);
    _case(iLLowerArmTeamMate6);
    _case(iLLowerArmTeamMate7);
    _case(iLLowerArmTeamMate8);
    _case(iLLowerArmTeamMate9);
    _case(iLLowerArmTeamMate10);
    _case(iLLowerArmTeamMate11);
    
    _case(iRFootTeamMate1);
    _case(iRFootTeamMate2);
    _case(iRFootTeamMate3);
    _case(iRFootTeamMate4);
    _case(iRFootTeamMate5);
    _case(iRFootTeamMate6);
    _case(iRFootTeamMate7);
    _case(iRFootTeamMate8);
    _case(iRFootTeamMate9);
    _case(iRFootTeamMate10);
    _case(iRFootTeamMate11);
    
    _case(iLFootTeamMate1);
    _case(iLFootTeamMate2);
    _case(iLFootTeamMate3);
    _case(iLFootTeamMate4);
    _case(iLFootTeamMate5);
    _case(iLFootTeamMate6);
    _case(iLFootTeamMate7);
    _case(iLFootTeamMate8);
    _case(iLFootTeamMate9);
    _case(iLFootTeamMate10);
    _case(iLFootTeamMate11);
    
    _case(iRLowerArmOpponent1);
    _case(iRLowerArmOpponent2);
    _case(iRLowerArmOpponent3);
    _case(iRLowerArmOpponent4);
    _case(iRLowerArmOpponent5);
    _case(iRLowerArmOpponent6);
    _case(iRLowerArmOpponent7);
    _case(iRLowerArmOpponent8);
    _case(iRLowerArmOpponent9);
    _case(iRLowerArmOpponent10);
    _case(iRLowerArmOpponent11);
    
    _case(iLLowerArmOpponent1);
    _case(iLLowerArmOpponent2);
    _case(iLLowerArmOpponent3);
    _case(iLLowerArmOpponent4);
    _case(iLLowerArmOpponent5);
    _case(iLLowerArmOpponent6);
    _case(iLLowerArmOpponent7);
    _case(iLLowerArmOpponent8);
    _case(iLLowerArmOpponent9);
    _case(iLLowerArmOpponent10);
    _case(iLLowerArmOpponent11);
    
    _case(iRFootOpponent1);
    _case(iRFootOpponent2);
    _case(iRFootOpponent3);
    _case(iRFootOpponent4);
    _case(iRFootOpponent5);
    _case(iRFootOpponent6);
    _case(iRFootOpponent7);
    _case(iRFootOpponent8);
    _case(iRFootOpponent9);
    _case(iRFootOpponent10);
    _case(iRFootOpponent11);
    
    _case(iLFootOpponent1);
    _case(iLFootOpponent2);
    _case(iLFootOpponent3);
    _case(iLFootOpponent4);
    _case(iLFootOpponent5);
    _case(iLFootOpponent6);
    _case(iLFootOpponent7);
    _case(iLFootOpponent8);
    _case(iLFootOpponent9);
    _case(iLFootOpponent10);
    _case(iLFootOpponent11);
    
    _case(iAcc);
    _case(iAccA);
    _case(iInfoID);
  default:
    return "???";
  }     
}
