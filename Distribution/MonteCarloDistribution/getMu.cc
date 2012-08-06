#include "montecarlodistribution.ih"

VectorXd MonteCarloDistribution::getMu() const
{
  // Return weighted sum over all particles
  // Other options are COM (unweighted sum, probably wrongly includes outliers too much),
  // or particle with highest weight (probably better when there are multiple particle clusters, but perhaps more inaccurate)
  return d_particles * d_weights;
}
