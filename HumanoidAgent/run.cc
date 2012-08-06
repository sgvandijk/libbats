#include "humanoidagent.ih"

void HumanoidAgent::run()
{
  // Startup agent
  startUp();
  
  // Get our worldmodel and stuff
  Cochlea& cochlea = bats::SCochlea::getInstance();
  WorldModel& wm = SWorldModel::getInstance();
  AgentSocketComm& comm = bats::SAgentSocketComm::getInstance();
  AgentModel& am = bats::SAgentModel::getInstance();
  Clock& clock = bats::SClock::getInstance();
  
  // Get first message
  comm.update();
  cochlea.update();
  clock.update();
  wm.update();
  unsigned i(0);
  
  while(wm.getNumMsgReceived() < 2 && ++i < 200) /// \todo Tweak to a minimum possible number of messages before sending init is ok.
  {
    comm.update();
    cochlea.update();
    clock.update();
    wm.update();
  }
  
  //Send init
  comm.init(d_unum, d_teamName);
  
  //Wait for unum
  while(wm.getNumMsgReceived() < 10 && cochlea.getInfo(Cochlea::iUnum)[0] == 0)//Failsafe max messages
  {
    comm.update();
    cochlea.update();
    clock.update();
    wm.update();
  }
  
  float unum = cochlea.getInfo(Cochlea::iUnum)[0];
  if (unum != 0)
  {
    assert(unum > 0);
    am.setUnum(static_cast<unsigned int>(unum));
  }
  
  cout << "unum: " << unum << endl;

  // At this point we should have a unum
  assert(am.getUnum());
  am.initBody();
  
  //Init agent (Localizer should be initialized in here, and player classes should be created here)
  init();
  
  Localizer& loc = SLocalizer::getInstance();
  loc.init();
  
  am.update();
  loc.update();
  
  timeval start, end;
//   timespec sleeptime;
//   sleeptime.tv_sec = 0;
//   sleeptime.tv_nsec = 10000000;
//   int minThinkTime = 5000;

  int countDown = 50;
  
gettimeofday(&end, 0);

  while(!s_die)
  {
    _debugLevel4("----------------------- New think cycle ------------------------");
    
    try
    {
      comm.update();
    }
    catch (runtime_error err)
    {
      cerr << "(HumanoidAgent::run) Error caught from comm update: " << err.what() << endl;
    }
    catch (...)
    {
      cerr << "(HumanoidAgent::run) Error caught from comm update!" << endl;
    }
    
    gettimeofday(&start, 0);
    double startTime = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;

    _debugLevel4("Comm updated");
    
    cochlea.update();
    _debugLevel4("Cochlea updated");
    
    clock.update();
    _debugLevel4("Clock updated");
    
    wm.update();
    _debugLevel4("WorldModel updated");
    
    am.update();
    _debugLevel4("AgentModel updated");
    
    loc.update();
    _debugLevel4("Localizer updated");
    
    am.predict();

    think_start_signal();
    if (countDown == 0)
    {
      try
      {
        think();
      }
      catch (runtime_error err)
      {
        cerr << "(HumanoidAgent::run) Error caught from think: " << err.what() << endl;
      }
      catch(...)
      {
        cerr << "(HumanoidAgent::run) Error caught from think!" << endl;
      }
    }
    else
      countDown--;

    think_end_signal();
    
    //
    // If 'think' completed too fast, sleep for a while.
    //
    gettimeofday(&end, 0);
    double thinkTime = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    
    if (d_minThinkTime > 0 && thinkTime < d_minThinkTime)
      usleep(d_minThinkTime - thinkTime);

    //cout << "Think: " << startTime << " " << thinkTime << endl;
  }
}
