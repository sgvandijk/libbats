#include "localizer.ih"

bool Localizer::tryObserveGlobalRotation(Eigen::Matrix3d& transform)
{
  if (!d_haveNewVisionData)
    return false;

  //
  // Build matrixes with global directions and observed directions
  //
  ObjectVector observedLandmarks =
    getFilteredObjects(d_objects, [](shared_ptr<ObjectInfo> o)
                       {
                         return o->isVisible && !o->isDynamic;
                       });
  
  unsigned nM = observedLandmarks.size();

  //
  // If we have less than 3 landmarks, can't determine rotation
  // TODO: if all landmarks are in a line, this won't work either
  //
  if (nM < 3)
    return false;

  MatrixXd dirsGlobal(3, nM * (nM - 1) / 2);
  MatrixXd dirsObserved(3, nM * (nM - 1) / 2);
  
  // Go through all combinations, and determine directions
  unsigned idx = 0;
  for (unsigned i = 0; i < nM; ++i)
    for (unsigned j = i + 1; j < nM; ++j)
    {
      dirsGlobal.col(idx) = observedLandmarks[j]->getPositionGlobal() - observedLandmarks[i]->getPositionGlobal();
      dirsObserved.col(idx) = observedLandmarks[j]->getPositionRaw() - observedLandmarks[i]->getPositionRaw();
      ++idx;
    }
  
  // Determine covariance (proxy)
  MatrixXd covar = dirsGlobal * dirsObserved.transpose();
  
  // Perform singular value decomposition, which provides two rotations and a scaling
  JacobiSVD<MatrixXd> svd(covar, ComputeThinU | ComputeThinV);
  
  // Least square estimate of the rotation matrix is product of the two rotations
  transform = svd.matrixU() * svd.matrixV().transpose();
    return true;
}
