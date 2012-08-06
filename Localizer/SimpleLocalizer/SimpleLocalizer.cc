#include "simplelocalizer.ih"

SimpleLocalizer::SimpleLocalizer()
{
  for (unsigned i = 0; i < Types::NOBJECTS; ++i)
  {
    d_positionsRaw[i] = new NormalDistribution(3);
    d_positionsLocal[i] = new NormalDistribution(3);
    d_positionsGlobal[i] = new NormalDistribution(3);

    d_velocitiesLocal[i] = new NormalDistribution(3);
    d_velocitiesGlobal[i] = new NormalDistribution(3);
  }
}

