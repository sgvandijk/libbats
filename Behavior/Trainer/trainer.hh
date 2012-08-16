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

#ifndef _BATS_TRAINER_HH_
#define _BATS_TRAINER_HH_

#include <Eigen/Core>

#include "../PrimitiveBehavior/primitivebehavior.hh"
#include "../../SocketComm/TrainerSocketComm/trainersocketcomm.hh"

namespace bats
{
  /** Behavior that can be used to set up training cycles
   *
   * Use this behavior as the win behavior. It will run successive training cycles consisting of the following steps:
   * - Reset: reset all behaviors
   * - Start1: beam to initial position as specified in the XML configuration and move all joints back to initial angles
   * - Start2: place ball to initial position and set the play mode as specified in the XML configuration
   * - Run: run the behavior tree under de Trainer as regular for the time as specified in the XML configuration
   * - Stop: end run
   *
   * If additional functionality is needed when a run is started or ended you can inherit from this class and implement onRunStart and/or onRunEnd
   *
   * Example XML configuration:
   * <pre>
        <param>
          <settings>
            ballStartPos = 0 0 0.04 - 0 0 0.04
            agentStartPos = -1 0 0 - -1 0 0
            runTime = 8
            runs = 1
            playMode = PlayOn
          </settings>
        </param>
      </pre>
   */
  class Trainer : public Behavior
  {
  protected:
    enum Stage
    {
      RESET,
      START1,
      START2,
      START3,
      RUN,
      STOP1,
      STOP2
    };
    
    Stage d_stage;
    int d_run;
    
    Eigen::Vector3d d_minBallStartPos;
    Eigen::Vector3d d_maxBallStartPos;
    
    Eigen::Vector3d d_minBallStartVel;
    Eigen::Vector3d d_maxBallStartVel;

    Eigen::Vector3d d_minAgentStartPos;
    Eigen::Vector3d d_maxAgentStartPos;
    
    Eigen::Vector3d d_ballStartPos;
    Eigen::Vector3d d_ballStartVel;
    Eigen::Vector3d d_agentStartPos;
    
    double d_runTime;
    int d_runs;
    
    double d_runStartTime;
    
    bool d_endRun;
    
    Types::PlayMode d_playMode;
    
    virtual std::shared_ptr<Goal> generateGoal(unsigned step, unsigned slot);
    virtual std::shared_ptr<State> getCurrentState();
    virtual ConfidenceInterval getCapability(std::shared_ptr<State> s, std::shared_ptr<Goal> g);
  
    /** Called when a run starts */
    virtual void onRunStart() {}
    
    /** Called when a run is ended */
    virtual void onRunEnd() {}
    
    /** Called at every step of a run */
    virtual void onStep() {}
    
    void resetJoints();
    
  public:
    Trainer(std::string const &id, std::string const& behaviorTree);
    
    virtual void update();

    virtual bool achieveGoal();
  };
};

#endif

