#include "rc3dclock.ih"

#include <iostream>

using namespace std;

void RC3DClock::update()
{
  Cochlea& cochlea = SCochlea::getInstance();
  
  if (cochlea.getInfo(Cochlea::iNow)[0] != 0)
  {
    double newTime = cochlea.getInfo(Cochlea::iNow)[0];
    // Round down to make up for time shift
    d_dt = newTime - d_time;
    //d_dt = (int)((newTime - d_time + 0.001) / 0.02) * 0.02;;
    if (d_dt >= 0.04)
      cerr << "Skipping messages! dt: " << d_dt << endl;
    d_time = newTime;
  }
}
