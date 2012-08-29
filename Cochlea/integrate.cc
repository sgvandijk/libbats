#include "cochlea.ih"

void Cochlea::integrate(shared_ptr<Predicate> const &pred)
{
//  cout << (pred->toString()) << endl;
  
  Vector4d vect, vect2;

  InfoID HJID(iNone), UJID(iNone), TID(iNone), id2, id3;

  string str;
  unsigned pid;

  unsigned currentPID = 100;
  bool currentTeam = true;

  Vector4d now;

  string hearMessage;

  bool us;

  // Reset all info to 0
  for (unsigned i = 0; i < iInfoID; ++i)
    d_info[i] = Vector4d(0,0,0,0);

  // since UNKNOWN_SIDE is not 0 anymore the team will always be reset to LEFT
  // this is a bit of a dirty hack but it should help
  d_info[iTeam].x() = Types::UNKNOWN_SIDE;

  d_lines.clear();
  d_lineEnds.clear();

  Eigen::Matrix4d mat;
  mat.setZero();
  d_selfTransform = Affine3d(mat);
  
  // Loop through all predicates under the main predicate
  for (Predicate::const_iterator i = pred->begin();
       i != pred->end(); ++i)
  {
    InfoID id = translateInfo(static_pointer_cast<Predicate>(*i)->getStr());

    // Check ID of predicate
    switch (id) {
    // Predicate contains game state information
    case iGameState:
    {
      for (Predicate::const_iterator j = (*i)->begin();
           j != (*i)->end(); ++j) {

        id = translateInfo(static_pointer_cast<Predicate>(*j)->getStr());
        d_dt[id] = now[0] - d_timestamps[id];
        d_timestamps[id] = now[0];

        switch (id) {

        case iUnum:
          d_info[iUnum] = parseScalar(static_pointer_cast<Predicate>(*j));
          break;

        case iTeam:
          if ((*j)->getChild(0))
            d_info[iTeam].x() = (static_pointer_cast<Predicate>((*j)->getChild(0))->getStr() == "left" ? Types::LEFT : Types::RIGHT);
          break;

        case iGameTime:
          d_info[iGameTime] = parseScalar(static_pointer_cast<Predicate>(*j));
          break;

        case iPlayMode:
          if ((*j)->getChild(0))
            d_info[iPlayMode].x() = (d_playModeMap[static_pointer_cast<Predicate>((*j)->getChild(0))->getStr()]);
          break;

        default:
          break;
        }
      }

      break;
    }
    // Predicate contains vision information
    case iVision:
    {
      // Time difference since last time, and timestamp of this data
      d_dt[id] = now[0] - d_timestamps[id];
      d_timestamps[id] = now[0];

      // Loop through all predicates in the vision data
      for (Predicate::const_iterator j = (*i)->begin();
           j != (*i)->end(); ++j)
      {
        id = translateInfo(static_pointer_cast<Predicate>(*j)->getStr());

        // Time difference since last time, and timestamp of this data
        d_dt[id] = now[0] - d_timestamps[id];
        d_timestamps[id] = now[0];

        // Check ID of vision data predicate
        switch (id) {

        // For the ball and landmarks, simply extract and store polar coordinates
        case iVisionBall:
        case iVisionFlag1L:
        case iVisionFlag1R:
        case iVisionFlag2L:
        case iVisionFlag2R:
        case iVisionGoal1L:
        case iVisionGoal1R:
        case iVisionGoal2L:
        case iVisionGoal2R:
          if ((*j)->getChild(0))
          {
            vect = parsePolar(static_pointer_cast<Predicate>((*j)->getChild(0)));
            d_info[id] = vect;
          }
          break;

        // Player information. Example:
        // (P (team teamBlue) (id 3) (rlowerarm (pol 0.18 -33.55 -20.16)) (llowerarm (pol 0.18 34.29 -19.80)))
        case iP:
        {
          // Extract team name
          if ((*j)->getChild(0) && (*j)->getChild(0)->getChild(0))
            str = static_pointer_cast<Predicate>((*j)->getChild(0)->getChild(0))->getStr();

          // Extract uniform number
          pid = 12;
          if ((*j)->getChild(0) && (*j)->getChild(1)->getChild(0))
            pid = static_pointer_cast<Predicate>((*j)->getChild(1)->getChild(0))->getInt();

          // Invalid uniform number, or number not found
          if (pid > 11)
            continue;

          // Check whether player is in our team or not
          us = str == d_teamName;
          if (us) {
            id2 = (InfoID)(iVisionTeamMate1 + pid - 1);
            _debugLevel1("PID/Team: " << currentPID << "/" << currentTeam);
          } else {
            d_opponentTeamName = str;
            id2 = (InfoID)(iVisionOpponent1 + pid - 1);
            _debugLevel1("PID/Team: " << currentPID << "/" << currentTeam);
          }

          // Go through body parts and extract position
          for (Predicate::const_iterator k = (*j)->begin();
             k != (*j)->end(); ++k)
          {
            id = translateInfo(static_pointer_cast<Predicate>(*k)->getStr());
            switch (id)
            {
            case iBodyHead:
            {
              if((*k)->getChild(0))
              {
                vect = parsePolar(static_pointer_cast<Predicate>((*k)->getChild(0)));
                _debugLevel4("vect: " << vect);
                d_info[id2] = vect;
                d_dt[id2] = now[0] - d_timestamps[id2];
                d_timestamps[id2] = now[0];
              }
              break;
            }
            case iBodyRLowerArm:
            {
              if((*k)->getChild(0))
              {
                vect = parsePolar(static_pointer_cast<Predicate>((*k)->getChild(0)));
                _debugLevel4("vect: " << vect);
                id3 = (InfoID)((us ? iRLowerArmTeamMate1 : iRLowerArmOpponent1) + pid - 1);
                d_info[id3] = vect;
                d_dt[id3] = now[0] - d_timestamps[id3];
                d_timestamps[id3] = now[0];
              }
              break;
            }
            case iBodyLLowerArm:
            {
              if((*k)->getChild(0))
              {
                vect = parsePolar(static_pointer_cast<Predicate>((*k)->getChild(0)));
                _debugLevel4("vect: " << vect);
                id3 = (InfoID)((us ? iLLowerArmTeamMate1 : iLLowerArmOpponent1) + pid - 1);
                d_info[id3] = vect;
                d_dt[id3] = now[0] - d_timestamps[id3];
                d_timestamps[id3] = now[0];
              }
              break;
            }
            case iBodyRFoot:
            {
              if((*k)->getChild(0))
              {
                vect = parsePolar(static_pointer_cast<Predicate>((*k)->getChild(0)));
                _debugLevel4("vect: " << vect);
                id3 = (InfoID)((us ? iRFootTeamMate1 : iRFootOpponent1) + pid - 1);
                d_info[id3] = vect;
                d_dt[id3] = now[0] - d_timestamps[id3];
                d_timestamps[id3] = now[0];
              }
              break;
            }
            case iBodyLFoot:
            {
              if((*k)->getChild(0))
              {
                vect = parsePolar(static_pointer_cast<Predicate>((*k)->getChild(0)));
                _debugLevel4("vect: " << vect);
                id3 = (InfoID)((us ? iLFootTeamMate1 : iLFootOpponent1) + pid - 1);
                d_info[id3] = vect;
                d_dt[id3] = now[0] - d_timestamps[id3];
                d_timestamps[id3] = now[0];
              }
              break;
            }

            default:
              break;
            }

          }
          break;
        }

        // Ground truth values: extract and store directly
        case iVisionBallGT:
        {
          vect = parseVect(static_pointer_cast<Predicate>(*j));
          d_info[id] = vect;
          break;
        }
        case iVisionSelfGT:
        {
          d_selfTransform = parseTransform(static_pointer_cast<Predicate>(*j));
          break;
        }

        case iVisionLine:
        {
          if ((*j)->getChild(0) && (*j)->getChild(0))
          {
            vect = parsePolar(static_pointer_cast<Predicate>((*j)->getChild(0)));
            vect2 = parsePolar(static_pointer_cast<Predicate>((*j)->getChild(1)));
            d_lines.push_back(VisibleLine(vect.head<3>(), vect2.head<3>()));
            d_lineEnds.push_back(vect.head<3>());
            d_lineEnds.push_back(vect2.head<3>());
          }

          break;
        }

        default:
          break;

        };

      }
      break;
    }

    case iGyro:
    {
      id = iTorsoGyro;
      d_dt[id] = now[0] - d_timestamps[id];
      d_timestamps[id] = now[0];
      if((*i)->getChild(1))
        d_info[id] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));

      _debugLevel4("d_info[iTorsoGyro]: " << d_info[iTorsoGyro]);

      break;
    }

    case iAcc:
    {
      d_dt[id] = now[0] - d_timestamps[id];
      d_timestamps[id] = now[0];
      if((*i)->getChild(1))
        d_info[id] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
      break;
    }

    case iForceResistancePerceptor:
    {
      TID = iNone;
      _debugLevel4("iForceResistancePerceptor");
      for (Predicate::const_iterator j = (*i)->begin();
           j != (*i)->end(); ++j)
        {
          id = translateInfo(static_pointer_cast<Predicate>(*j)->getStr());

          d_dt[id] = now[0] - d_timestamps[id];
          d_timestamps[id] = now[0];

          switch (id) {
          case iName:
            if ((*j)->getChild(0))
              TID = translateInfo(static_pointer_cast<Predicate>((*j)->getChild(0))->getStr());
            _debugLevel4("TID: " << TID);
            break;

          case iContact:
            vect = parseVect(static_pointer_cast<Predicate>(*j));
            _debugLevel4("vect: " << vect);
            break;

          case iForce:
            vect2 = parseVect(static_pointer_cast<Predicate>(*j));

            if (d_frpFix) {
              if (vect2.norm() != 0 && d_info[iPlayMode].x() != Types::BEFORE_KICKOFF)
              d_frpFix = false;
              else
              vect2.z() = (1.0);
            }

            _debugLevel4("vect2: " << vect2);
            break;

          default:
            break;
          }
        }

      d_dt[TID] = now[0] - d_timestamps[TID];
      d_timestamps[TID] = now[0];
      d_info[TID] = vect;
      d_info[TID + 1] = vect2;
      break;
    }

    case iTime:
    {
      for (Predicate::const_iterator j = (*i)->begin();
           j != (*i)->end(); ++j) {

        id = translateInfo(static_pointer_cast<Predicate>(*j)->getStr());

        d_dt[id] = now[0] - d_timestamps[id];
        d_timestamps[id] = now[0];

        switch (id) {

        case iNow:
          now = parseScalar(static_pointer_cast<Predicate>(*j));
//           cout.precision(6);
          d_info[iNow] = now;
          break;

        case iStep:
          d_info[iStep] = parseScalar(static_pointer_cast<Predicate>(*j));
          break;

        default:
          break;
        };
      }
      break;
    }

    case iUJ:
    {
      vect = Vector4d(0.0,0.0,0.0,0.0);
      vect2 = Vector4d(0.0,0.0,0.0,0.0);

      UJID = iNone;

      for (Predicate::const_iterator j = (*i)->begin();
           j != (*i)->end(); ++j) {

        id = translateInfo(static_pointer_cast<Predicate>(*j)->getStr());

        switch (id) {
        case iName:
          UJID = translateInfo(static_pointer_cast<Predicate>((*j)->getChild(0))->getStr());
          _debugLevel4(static_pointer_cast<Predicate>((*j)->getChild(0))->getStr());
          break;

        case iAxis1:
          vect.x() = (*static_pointer_cast<Predicate>((*j)->getChild(0)));
          _debugLevel4("joint: " << vect.x());
          break;

        case iRate1:
          vect.y() = (*static_pointer_cast<Predicate>((*j)->getChild(0)));
          break;

        case iAxis2:
          vect2.x() = (*static_pointer_cast<Predicate>((*j)->getChild(0)));
          _debugLevel4("joint: " << vect2.x());
          break;

        case iRate2:
          vect2.y() = (*static_pointer_cast<Predicate>((*j)->getChild(0)));
          break;

        default:
          _debugLevel4("Unknown id: " << static_pointer_cast<Predicate>(*j)->getStr());
          break;
        };

      }

      if (UJID) {
        d_dt[UJID] = now[0] - d_timestamps[UJID];
        d_timestamps[UJID] = now[0];
        d_dt[UJID+1] = now[0] - d_timestamps[UJID];
        d_timestamps[UJID+1] = now[0];
        d_info[UJID] = vect;
        d_info[UJID+1] = vect2;
      } else
        _debugLevel4("Unknown UJID!");
      break;
    }

    case iHJ:
    {
      vect = Vector4d(0.0,0.0,0.0,0.0);

      HJID = iNone;

      for (Predicate::const_iterator j = (*i)->begin();
           j != (*i)->end(); ++j) {

        id = translateInfo(static_pointer_cast<Predicate>(*j)->getStr());

        switch (id) {
        case iName:
          HJID = translateInfo(static_pointer_cast<Predicate>((*j)->getChild(0))->getStr());
          break;

        case iAxis:
          vect.x() = (*static_pointer_cast<Predicate>((*j)->getChild(0)));
          _debugLevel4("joint: " << HJID << " " << vect.x());
          break;

        case iRate:
          vect.y() = (*static_pointer_cast<Predicate>((*j)->getChild(0)));
          break;

        case iTorque:
          vect.z() = (*static_pointer_cast<Predicate>((*j)->getChild(0)));
          break;

        default:
          break;
        };

      }

      if (HJID)
        {
          d_dt[HJID] = now[0] - d_timestamps[HJID];
          d_timestamps[HJID] = now[0];
          d_info[HJID] = vect;
        }
      else
        _debugLevel4("Unknown HJID!");
      break;
    }

    case iHJT:
    {
      vect = Vector4d(0.0,0.0,0.0,0.0);

      HJID = iNone;

      for (Predicate::const_iterator j = (*i)->begin();
           j != (*i)->end(); ++j) {

        id = translateInfo(static_pointer_cast<Predicate>(*j)->getStr());

        switch (id) {
        case iName:
          HJID = translateInfo(static_pointer_cast<Predicate>((*j)->getChild(0))->getStr());
          break;

        case iAxis:
          vect = parseVect(static_pointer_cast<Predicate>(*j));
          _debugLevel2("joint: " << HJID << " " << vect);
          break;

        default:
          break;
        };

      }

      if (HJID)
        d_info[HJID] = vect;
      else
        _debugLevel4("Unknown HJID!");
      break;
    }

    case iHear:
    {
      double hearTime;
      if ((*i)->getChild(0))
        hearTime = *static_pointer_cast<Predicate>((*i)->getChild(0));
      else
        break;

      double hearAngle;
      if ((*i)->getChild(1))
      {
        // Ignore messages from self
        if (static_pointer_cast<Predicate>((*i)->getChild(1))->getStr() == "self")
          break;
        hearAngle = *static_pointer_cast<Predicate>((*i)->getChild(1));
        hearAngle *= M_PI / 180;
      }
      else
        break;

      if ((*i)->getChild(0))
        hearMessage = static_pointer_cast<Predicate>((*i)->getChild(2))->getStr();

      d_dt[iHear] = now[0] - d_timestamps[iHear];
      d_timestamps[iHear] = now[0];

      d_hearMessage = HearMessage(hearTime, hearAngle, hearMessage);

      break;
    }

    case iBody:
    {
      _debugLevel1("ERROR!! Wrong Body!! Body (pid="<<currentPID<<")");

      id = translateInfo(static_pointer_cast<Predicate>((*i)->getChild(0))->getStr());

      if (currentPID == 100) {

        if (id < iInfoID)
          d_info[id] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));

      } else {

        if (currentTeam) {

          switch (id) {

          case iBodyRLowerArm:
            d_info[iRLowerArmTeamMate1 + currentPID] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
            break;

          case iBodyLLowerArm:
            d_info[iLLowerArmTeamMate1 + currentPID] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
            break;

          case iBodyRFoot:
            d_info[iRFootTeamMate1 + currentPID] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
            break;

          case iBodyLFoot:
            d_info[iRFootTeamMate1 + currentPID] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
            break;

          default:
            break;
          };

        } else {

          switch (id) {

          case iBodyRLowerArm:
            d_info[iRLowerArmOpponent1 + currentPID] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
            break;

          case iBodyLLowerArm:
            d_info[iLLowerArmOpponent1 + currentPID] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
            break;

          case iBodyRFoot:
            d_info[iRFootOpponent1 + currentPID] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
            break;

          case iBodyLFoot:
            d_info[iRFootOpponent1 + currentPID] = parseVect(static_pointer_cast<Predicate>((*i)->getChild(1)));
            break;

          default:
            break;
          };
        }

      }
      break;
    }

    default:
      break;
    };


  }
  //cout << "hoei1" << endl;
}
