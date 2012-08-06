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

#ifndef _BATS_BEHAVIOREVENT_HH_
#define _BATS_BEHAVIOREVENT_HH_

#include <string>
#include <sstream>
#include <Eigen/Core>
#include "../RefAble/refable.hh"

namespace bats
{
  /** The basic class for events thrown by libbats */
  class BatsEvent : public RefAble
  {
    public:
      BatsEvent() {}
      virtual ~BatsEvent() {}

      /**
       * @returns the string description of the Event
       */
      virtual std::string toString() const { return "BatsEvent"; }
  };
  
  /** An event fired when the agent beams by the Beam behavior */
  class BeamEvent : public BatsEvent
  {
      Eigen::Vector3d d_where;
    
    public:
      /** Constructor
       * @param where The location where the agent beamed to
       */
      BeamEvent(Eigen::Vector3d const& where)
      : d_where(where)
      {}
      
      /**
       * @returns the location where the agent beamed to
       */
      Eigen::Vector3d getWhere() const { return d_where; }
      
      virtual std::string toString()
      {
        std::ostringstream out;
        out << "Beam: " << d_where;
        return out.str();
      }

    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
  
  /*
   * --------------------- Behavior Events -------------------
   */
   
  /** The basic class for behavior related events */
  class BehaviorEvent : public BatsEvent
  {
    protected:
      std::string d_behType;
      std::string d_behId;
      std::string d_message;
      
    public:
      /** Constructor
       * @param behType Type of the behavior that fires the event.
       * @param behId ID of the behavior that fires the event.
       * @param message Message to attach to the event
       */
      BehaviorEvent(std::string const& behType, std::string const& behId, std::string const& message = "")
      : d_behType(behType),
        d_behId(behId),
        d_message(message)
      {}
      
      /**
       * @returns type of the behavior that fired the event.
       */
      std::string getBehType() const { return d_behType; }
      
      /**
       * @returns ID of the behavior that fires the event.
       */
      std::string getBehId() const { return d_behId; }
      
      virtual std::string toString() const { return d_behType + std::string("::") + d_behId + d_message; }
  };
  
  /** An event fired when a behavior starts its achieveGoal method */
  class AchieveGoalStartEvent : public BehaviorEvent
  {
    public:
      /** Constructor
       * @param behType Type of the behavior that fires the event.
       * @param behId ID of the behavior that fires the event.
       */
      AchieveGoalStartEvent(std::string const behType, std::string const& behId)
      : BehaviorEvent(behType, behId, " - achieveGoal started")
      {}

  };

  class AchieveGoalSuccessEvent : public BehaviorEvent
  {
    public:
      /** Constructor
       * @param behType Type of the behavior that fires the event.
       * @param behId ID of the behavior that fires the event.
       */
      AchieveGoalSuccessEvent(std::string const behType, std::string const& behId)
      : BehaviorEvent(behType, behId, " - achieveGoal successfull")
      {}
  };
  
  /** Event fired when a behavior starts attempt to run the last chosen (committed) subbehaviors) */
  class LastChosenStartEvent : public BehaviorEvent
  {
    public:
      /** Constructor
       * @param behType Type of the behavior that fires the event.
       * @param behId ID of the behavior that fires the event.
       */
      LastChosenStartEvent(std::string const behType, std::string const& behId)
      : BehaviorEvent(behType, behId, " - run last chosen children started")
      {}
  };
  
  /** Event fired when a behavior starts attempt to run the current sequence step */
  class CurrentStepStartEvent : public BehaviorEvent
  {
      unsigned d_step;
      
    public:
      /** Constructor
       * @param behType Type of the behavior that fires the event.
       * @param behId ID of the behavior that fires the event.
       * @param step Number of current sequence step
       */
      CurrentStepStartEvent(std::string const behType, std::string const& behId, unsigned step)
      : BehaviorEvent(behType, behId),
        d_step(step)
      {
        std::ostringstream out;
        out << BehaviorEvent::toString() << " - run current step (" << d_step << ") started";
        d_message = out.str();
      }
      
      /**
       * @returns number of current sequence step
       */
      unsigned getStep() const { return d_step; }
  };
  
  /** Event fired when a behavior starts attempt to run the next sequence step */
  class NextStepStartEvent : public BehaviorEvent
  {
      unsigned d_step;
      
    public:
      /** Constructor
       * @param behType Type of the behavior that fires the event.
       * @param behId ID of the behavior that fires the event.
       * @param step Number of next sequence step
       */
      NextStepStartEvent(std::string const behType, std::string const& behId, unsigned step)
      : BehaviorEvent(behType, behId),
        d_step(step)
      {
        std::ostringstream out;
        out << BehaviorEvent::toString() << " - run next step (" << d_step << ") started";
        d_message = out.str();
      }

      /**
       * @returns number of current sequence step
       */
      unsigned getStep() const { return d_step; }
  };
};

#endif

