/*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors: 	Martin Klomp (martin@ai.rug.nl)
 *		Mart van de Sanden (vdsanden@ai.rug.nl)
 *		Sander van Dijk (sgdijk@ai.rug.nl)
 *		A. Bram Neijt (bneijt@gmail.com)
 *		Matthijs Platje (mplatje@gmail.com)
 *
 *	All students of AI at the University of Groningen
 *  at the time of writing. See: http://www.ai.rug.nl/
 *
 *  Date: 	November 1, 2008
 *
 *  Website:	http://www.littlegreenbats.nl
 *
 *  Comment:	Please feel free to contact us if you have any 
 *		problems or questions about the code.
 *
 *
 *  License: 	This program is free software; you can redistribute 
 *		it and/or modify it under the terms of the GNU General
 *		Public License as published by the Free Software 
 *		Foundation; either version 3 of the License, or (at 
 *		your option) any later version.
 *
 *   		This program is distributed in the hope that it will
 *		be useful, but WITHOUT ANY WARRANTY; without even the
 *		implied warranty of MERCHANTABILITY or FITNESS FOR A
 *		PARTICULAR PURPOSE.  See the GNU General Public
 *		License for more details.
 *
 *   		You should have received a copy of the GNU General
 *		Public License along with this program; if not, write
 *		to the Free Software Foundation, Inc., 59 Temple Place - 
 *		Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef __INC_BATS_NORMALDISTRIBUTION_HH_
#define __INC_BATS_NORMALDISTRIBUTION_HH_

#include "../distribution.hh"
#include <Eigen/Core>
#include <Eigen/LU>

namespace bats {

  /// Variable size normal distribution
  class NormalDistribution : public Distribution
  {
    std::size_t d_size;
    Eigen::VectorXd d_mu;
    Eigen::MatrixXd d_sigma;
    Eigen::MatrixXd d_invSigma;
    Eigen::MatrixXd d_I;

  public:
    /** Construct normal distribution with d dimensions
      * sets /mu and /sigma to 0
      * @param size Number of dimensions
    */
    NormalDistribution(int size) 
    : d_size(size),
      d_mu(size),
      d_sigma(size,size),
      d_invSigma(size, size),
      d_I(size,size)
    {
      d_mu.setZero();
      d_sigma.setZero();
      d_invSigma.setZero();
      d_I.setIdentity();
    }
    

    virtual ~NormalDistribution() {}
      
    /** Initialize distribution
     *
     * Initialize the distribution given a normal distribution \f$ N(\mu, \Sigma) \f$
     * @param mu Mean vector of the distribution
     * @param sigma Covariance matrix of the distribution
     */
    virtual void init(Eigen::VectorXd const &mu, Eigen::MatrixXd const &sigma);

    /** Get Mu
     *
     * Retrieve \f$ \mu \f$: the mean/average/expected value of the distribution. \f$ \mu \f$ is the point with the highest probability
     * @returns a 3D Vector that \f$ \mu \f$ will be placed into
     */
    virtual Eigen::VectorXd getMu() const { return d_mu; }
    
    virtual void setMu(Eigen::VectorXd const& mu) { d_mu = mu; }
    
    /** Get Sigma
     *
     * Retrieve \f$ \Sigma \f$: the covariance matrix/expected deviationof the distribution. This will be an estimation when the distribution is non-normal
     * @returns a 3x3 Matrix that Sigma will be placed into
     */
    virtual Eigen::MatrixXd getSigma() const { return d_sigma; }
        
    virtual void setSigma(Eigen::MatrixXd const& sigma) { d_sigma = sigma; d_invSigma = sigma.inverse(); }
    
    virtual Eigen::VectorXd draw() const;
    
    virtual double getP(Eigen::VectorXd const& x) const;
    
    /** Predict distribution.
     *
     * Predict/move the distribution given a specific movement model
     * \f$x_{t+1} = F x_t + B u \f$
     * @param F State transition model. Relates current state to next state
     * @param controlModel Control-input model. Expected state change given control u
     */
    virtual void predict(Eigen::MatrixXd const& F, std::shared_ptr<Distribution> controlModel);
      
    /** Update distribution.
     *
     * Update the distribution given a measurement and the measurement model for that measurement
     * 
     * @param obsModel Observation model. Relates state to a measurement
     */
    virtual void update(std::shared_ptr<Distribution> obsModel);

  private:
    NormalDistribution(NormalDistribution const &other); //NI    
    NormalDistribution &operator=(NormalDistribution const &other); // NI

  };

};

#endif // __INC_BATS_NORMALDISTRIBUTION_HH_
