#include "dribbleagent.ih"

DribbleAgent::DribbleAgent()
  : HumanoidAgent(string("Dribble"), "conf.xml"),
    d_beamed(false)
{
}
