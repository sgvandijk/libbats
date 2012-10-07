#include "ikgaitgenerator.ih"

void IKGaitGenerator::updatePhase()
{
  // Determine phase change (per second) based on step period length
  double dphase = 2.0 * M_PI / d_period;

  cout << dphase << " " << SClock::getInstance().getDt() << endl;

  // Update phase
  d_phase += dphase * SClock::getInstance().getDt();
  d_phase = fmod(d_phase, 2 * M_PI);

  cout << "Phase: " << d_phase << endl;
}
