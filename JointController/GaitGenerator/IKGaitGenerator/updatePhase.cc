#include "ikgaitgenerator.ih"

void IKGaitGenerator::updatePhase()
{
  // Determine phase change (per second) based on step period length
  double dphase = 2.0 * M_PI / d_period;

  // Update phase
  double dt = SClock::getInstance().getDt();
  cout << "phi: ----- " << d_phase << " " << dt << " " << d_period << " " << dphase << endl;
  d_phase += dphase * dt;

  cout << "phi: " << d_phase << endl;
  d_phase = fmod(d_phase, 2 * M_PI);
  cout << "phi: " << d_phase << endl;

}
