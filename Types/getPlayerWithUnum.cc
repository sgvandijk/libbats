#include "types.ih"

Types::Object Types::getTeamMateWithUnum(unsigned unum)
{
  if (unum < 1 || unum > bats::SWorldModel::getInstance().getNumberOfPlayers())
  {
    cerr << "(Types::getTeamMateWithUnum) Invalid unum: " << unum << endl;
    return Types::NOBJECTS;
  }
  return (Types::Object)(Types::TEAMMATE1 + unum - 1);
};

Types::Object Types::getOpponentWithUnum(unsigned unum)
{
  // NOTE we have to allow the full range of unums here as opponents may have any unum
  if (unum < 1 || unum > 11)
  {
    cerr << "(Types::getOpponentWithUnum) Invalid unum: " << unum << endl;
    return Types::NOBJECTS;
  }
  return (Types::Object)(Types::OPPONENT1 + unum - 1);
};

