#include "cerebellum.ih"

void Cerebellum::outputCommands(AgentSocketComm& comm)
{
  rf<Action> actions[Types::NJOINTS];
  double counts[Types::NJOINTS];
  memset(reinterpret_cast<char*>(counts), 0, Types::NJOINTS * sizeof(double));

  rf<MoveJointAction> mja;
  //rf<MoveHingeJointAction> mhja;
  //rf<MoveUniversalJointAction> muja;
  rf<BeamAction> ba;

  string sayMessage = "";
  
  _debugLevel4("output commands");

  for (vector<rf<Action> >::iterator iter = d_actions.begin(); iter != d_actions.end(); ++iter)
    {
      _debugLevel4("action type: " << (*iter)->type);
    
      switch ((*iter)->type)
        {
        case RC3DAction::MOVEJOINT:
          mja = rf_cast<MoveJointAction>(*iter);
          _debugLevel2("joint: " << (int)mja->joint << " " << mja->speed);
          if (!actions[mja->joint])
            {
              actions[mja->joint] = mja;
              counts[mja->joint] = 1;
            }
          else
            {
              _debugLevel2("+ joint: " << (int)mja->joint << " " << mja->speed);
              rf_cast<MoveJointAction>(actions[mja->joint])->speed += mja->speed;
              counts[mja->joint]++;
            }
          break;
        
        case RC3DAction::BEAM:
          ba = rf_cast<BeamAction>(*iter);
          _debugLevel4("beam: " << ba->pos);
          comm.beam(ba->pos);
          break;
      
        case RC3DAction::SAY:
          sayMessage += rf_cast<SayAction>(*iter)->message;
          break;
        
        default:
          break;
        };
    }
  
  ostringstream msg;
  double speed1 = 0, speed2 = 0;
  AgentModel& am = SAgentModel::getInstance();
  
  for (unsigned i = 0; i < Types::NJOINTS; ++i)
    {
      speed1 = 0;
      speed2 = 0;
      Types::Joint joint = (Types::Joint)i;
      rf<Joint> j = am.getJoint(joint);
      _debugLevel4(joint << " " << j);
      if (j)
        {
          switch (j->type) {
          case Types::UNIVERSAL_JOINT:
            //        bool moveIt = false;
            if (actions[i])
              {
                speed1 = rf_cast<MoveJointAction>(actions[i])->speed;// / counts[i];
                //          moveIt = true;
              }
            ++i;
            if (actions[i])
              {
                speed2 = rf_cast<MoveJointAction>(actions[i])->speed;// / counts[i];
                _debugLevel4("speed: " << speed2);
                //          moveIt = true;
              }
            //        if (true || moveIt)
            //        {
            _debugLevel4("speed: " << speed1);
            _debugLevel4("speed: " << speed2);
            comm.moveUniversalJoint(joint, speed1, speed2);
            //        }
            break;

          case Types::HINGE_JOINT:
            if (actions[i]) {
              speed1 = rf_cast<MoveJointAction>(actions[i])->speed;// / counts[i];
              _debugLevel4("speed: " << speed1);
            }

            if (d_prevJointValues[i] != speed1)
            {
              _debugLevel4("moveJoint " << joint << " speed " << speed1);

              // We could probably filter this out if we don't have to do anything!!
              comm.moveHingeJoint(joint, speed1);

              d_prevJointValues[i] = speed1;
            }// else
            //              _debugLevel4("Skipping duplicate hinge");
            break;

          case Types::TORQUE_JOINT:
            if (actions[i])
              {
                speed1 = rf_cast<MoveJointAction>(actions[i])->speed;// / counts[i];
                _debugLevel4("speed: " << speed1);
              }

            _debugLevel4("moveTorqueJoint " << joint << " speed " << speed1);

            // We could probably filter this out if we don't have to do anything!!
            comm.moveTorqueJoint(joint, speed1);
            break;

          default:
            _debugLevel1("(outputCommands) error, unknown joint type for joint " << i << ".");
            break;
          };
        }
    }
  
  // Check if we want to say anything.
  if (sayMessage.size() > 0)
    comm.say(sayMessage);
  
  //comm.sendMessage("(graph (g hoi)(val 75))(graph (g doei)(val 90))");
  
  d_actions.clear();
}
