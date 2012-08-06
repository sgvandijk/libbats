#include "trainer.ih"

Trainer::Trainer(string const &id, string const& behaviorTree)
  : Behavior("Trainer", id, behaviorTree),
    d_stage(STOP2),
    d_run(0),
    d_minBallStartPos(0,0,0.4),
    d_maxBallStartPos(0,0,0.4),
    d_minBallStartVel(0, 0, 0),
    d_maxBallStartVel(0, 0, 0),
    d_minAgentStartPos(-1,0,0),
    d_maxAgentStartPos(-1,0,0),
    d_ballStartPos(0,0,0.4),
    d_agentStartPos(-1,0,0),
    d_runTime(10.0),
    d_runs(-1),
    d_runStartTime(-1),
    d_endRun(false),
    d_playMode(Types::PLAY_ON)
{
  srand(time(0));
  
  //WorldModel& wm = SWorldModel::getInstance();
  STrainerSocketComm::initialize();
  
  string host = Behavior::getParam("/host", string("127.0.0.1"));
  //cerr << "host: " << host << endl;
  STrainerSocketComm::getInstance().initSocket(host, TrainerSocketComm::port);
  STrainerSocketComm::getInstance().connect();
  STrainerSocketComm::getInstance().parseInput(false);
  
  d_tree = new AST::Node(sequenceType);
  d_tree->addChild(new AST::Node(andType));
  d_tree->getChild(0)->addChild(new AST::Node(orType));

  XMLNodeSet params = Behavior::getParams("/settings");
  istringstream paramReader;
  if (params && !params.empty())
  {
    XMLNode paramNode = params.front();
    paramReader.str(paramNode.getContent());

    while (!paramReader.eof())
    {
      _debugLevel1("Reading new line..");
      string setting;
      char punc;
      
      paramReader >> setting;
      if (setting[setting.size() - 1] != '=')
      {
        paramReader >> punc;
        if (punc != '=')
        {
          _debugLevel1("Wrong format: '" << punc << "', expected '='");
          break;
        }
      }
      
      if (setting == "ballStartPos")
      {
        paramReader >> d_minBallStartPos[0] >> d_minBallStartPos[1] >> d_minBallStartPos[2] >> punc
                    >> d_maxBallStartPos[0] >> d_maxBallStartPos[1] >> d_maxBallStartPos[2];
      }
      else if (setting == "ballStartVel")
        paramReader >> d_minBallStartVel[0] >> d_minBallStartVel[1] >> d_minBallStartVel[2] >> punc
                    >> d_maxBallStartVel[0] >> d_maxBallStartVel[1] >> d_maxBallStartVel[2];
      else if (setting == "agentStartPos")
        paramReader >> d_minAgentStartPos[0] >> d_minAgentStartPos[1] >> d_minAgentStartPos[2] >> punc
                    >> d_maxAgentStartPos[0] >> d_maxAgentStartPos[1] >> d_maxAgentStartPos[2];
      else if (setting == "runTime")
        paramReader >> d_runTime;
      else if (setting == "runs")
        paramReader >> d_runs;
      else if (setting == "playMode")
      {
        string mode;
        paramReader >> mode;
        d_playMode = SCochlea::getInstance().getPlayMode(mode);
      }
    }
  }
  else
  {
    _debugLevel1("no settings parameters found");
  }

}
