#include "simplelocalizer.ih"
#include <iostream>

void SimpleLocalizer::update()
{
  Cochlea& cochlea = SCochlea::getInstance();
  WorldModel& wm = SWorldModel::getInstance();
  
  // Polar to Cartesian
  Vector3d ballPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionBall).start<3>());

  Vector3d flag1LPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionFlag1L).start<3>());
  Vector3d flag1RPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionFlag1R).start<3>());
  Vector3d flag2LPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionFlag2L).start<3>());
  Vector3d flag2RPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionFlag2R).start<3>());

  Vector3d goal1LPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionGoal1L).start<3>());
  Vector3d goal1RPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionGoal1R).start<3>());
  Vector3d goal2LPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionGoal2L).start<3>());
  Vector3d goal2RPos = Math::polarToCartesian(cochlea.getInfo(Cochlea::iVisionGoal2R).start<3>());

  Types::Side team = wm.getSide();
  
  Matrix3d zeroMat = Matrix3d::Zero();
  
  // Store positions in local coordinates
  d_positionsRaw[Types::BALL]->init(ballPos, zeroMat);

//   d_positionsRaw[Types::GOAL1L]->init(goal1LPos, zeroMat);
  d_positionsRaw[team ==Types::LEFT ? Types::GOAL1US : Types::GOAL2THEM]->init(goal1LPos, zeroMat);
  
//   d_positionsRaw[Types::GOAL2L]->init(goal2LPos, zeroMat);
  d_positionsRaw[team ==Types::LEFT ? Types::GOAL2US : Types::GOAL1THEM]->init(goal2LPos, zeroMat);
   
//   d_positionsRaw[Types::GOAL1R]->init(goal1RPos, zeroMat);
  d_positionsRaw[team ==Types::LEFT ? Types::GOAL1THEM : Types::GOAL2US]->init(goal1RPos, zeroMat);
   
//   d_positionsRaw[Types::GOAL2R]->init(goal2RPos, zeroMat);
  d_positionsRaw[team ==Types::LEFT ? Types::GOAL2THEM : Types::GOAL1US]->init(goal2RPos, zeroMat);

//   d_positionsRaw[Types::FLAG1L]->init(flag1LPos, zeroMat);
  d_positionsRaw[team == Types::LEFT ? Types::FLAG1US : Types::FLAG2THEM]->init(flag1LPos, zeroMat);
  
//   d_positionsRaw[Types::FLAG2L]->init(flag2LPos, zeroMat);
  d_positionsRaw[team ==Types::LEFT ? Types::FLAG2US : Types::FLAG1THEM]->init(flag2LPos, zeroMat);
   
//   d_positionsRaw[Types::FLAG1R]->init(flag1RPos, zeroMat);
  d_positionsRaw[team ==Types::LEFT ? Types::FLAG1THEM : Types::FLAG2US]->init(flag1RPos, zeroMat);
   
//   d_positionsRaw[Types::FLAG2R]->init(flag2RPos, zeroMat);
  d_positionsRaw[team ==Types::LEFT ? Types::FLAG2THEM : Types::FLAG1US]->init(flag2RPos, zeroMat);

  // for each team mate
  for (unsigned i = 0; i < wm.getNumberOfPlayers(); ++i) {
    Vector3d playerPosPolar = cochlea.getInfo((Cochlea::InfoID)(Cochlea::iVisionTeamMate1 + i)).start<3>();
    if (playerPosPolar[0] != 0)
    {
      Vector3d playerPosCart = Math::polarToCartesian(playerPosPolar);
      d_positionsRaw[Types::TEAMMATE1 + i]->init(playerPosCart, zeroMat);
    }
    else
      d_positionsRaw[Types::TEAMMATE1 + i]->init(Vector3d(0,0,0), zeroMat);
  }
  // for each opponent
  for (unsigned i = 0; i < wm.getNumberOfPlayers(); ++i) {
    Vector3d opponentPosPolar = cochlea.getInfo((Cochlea::InfoID)(Cochlea::iVisionOpponent1 + i)).start<3>();
    if (opponentPosPolar[0] != 0)
    {
      Vector3d opponentPosCart = Math::polarToCartesian(opponentPosPolar);
      d_positionsRaw[Types::OPPONENT1 + i]->init(opponentPosCart, zeroMat);
    }
    else
      d_positionsRaw[Types::OPPONENT1 + i]->init(Vector3d(0,0,0), zeroMat);
  }


  // Determine field orientation
  Vector3d worldRight = (d_positionsRaw[Types::FLAG1THEM]->getMu() - d_positionsRaw[Types::FLAG1US]->getMu());
  Vector3d worldForward = (d_positionsRaw[Types::FLAG1US]->getMu() - d_positionsRaw[Types::FLAG2US]->getMu());
  Vector3d up = worldRight.cross(worldForward);
  
  Vector3d wrNorm = worldRight.normalized();// * wrlInv;
  Vector3d wfNorm = worldForward.normalized();// * wflInv;
  Vector3d uNorm = up.normalized();

  Transform3d invGlobalTransform = Math::makeTransform(wrNorm, wfNorm, uNorm);
  d_globalTransform = invGlobalTransform.inverse();
  
  Vector3d right = up.cross(Vector3d(1, 0, 0).cross(up)).normalized();
  Vector3d forward = up.cross(Vector3d(0, 1, 0).cross(up)).normalized();
  
  Transform3d invLocalTransform = Math::makeTransform(right, forward, uNorm);
  d_localTransform = invLocalTransform.inverse();
  
  // Transform all positions into local and global coordinates
  for (unsigned i = 0; i < Types::NOBJECTS; ++i)
  {
    Vector3d local = d_localTransform * (Vector3d)d_positionsRaw[i]->getMu();
    d_velocitiesLocal[i]->init((local - d_positionsLocal[i]->getMu()) / 0.02, zeroMat);
    d_positionsLocal[i]->init(local, zeroMat);
    
    Vector3d global = d_globalTransform * (Vector3d)d_positionsRaw[i]->getMu();
    d_positionsGlobal[i]->init(global, zeroMat);
  }

  Vector3d flag1UsRef(-6.0, 4.0, 0);
  Vector3d flag1UsDif = flag1UsRef - d_positionsGlobal[Types::FLAG1US]->getMu();
  
  for (unsigned i = 0; i < Types::NOBJECTS; ++i)
  {
    Vector3d global = d_positionsGlobal[i]->getMu() + flag1UsDif;
    d_velocitiesGlobal[i]->init((global - d_positionsGlobal[i]->getMu()) / 0.02, zeroMat);
    d_positionsGlobal[i]->init(global, zeroMat);
  }
}

