#include "simplelocalizer.ih"

SimpleLocalizer::SimpleLocalizer()
{
  for (unsigned i = 0; i < Types::NOBJECTS; ++i)
  {
    d_positionsRaw[i] = make_shared<NormalDistribution>(3);
    d_positionsLocal[i] = make_shared<NormalDistribution>(3);
    d_positionsGlobal[i] = make_shared<NormalDistribution>(3);

    d_velocitiesLocal[i] = make_shared<NormalDistribution>(3);
    d_velocitiesGlobal[i] = make_shared<NormalDistribution>(3);
  }
}

