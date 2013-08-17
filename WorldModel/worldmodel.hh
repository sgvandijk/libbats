/*
 *  Little Green BATS (2008-2013), AI department, University of Groningen
 *
 *  Authors: 	Sander van Dijk (sgdijk@ai.rug.nl)
 *		Drew Noakes (drew@drewnoakes.com)
 *		Martin Klomp (martin@ai.rug.nl)
 *		Mart van de Sanden (vdsanden@ai.rug.nl)
 *		A. Bram Neijt (bneijt@gmail.com)
 *		Matthijs Platje (mplatje@gmail.com)
 *		Jeroen Kuijpers (jkuypers@gmail.com)
 *
 *  Date: 	August 17, 20013
 *
 *  Website:	https://github.com/sgvandijk/libbats
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
#ifndef __INC_BATS_WORLDMODEL_HH_
#define __INC_BATS_WORLDMODEL_HH_

#include <map>

#include <Eigen/Core>

#include "../Singleton/singleton.hh"
#include "../Types/types.hh"

namespace bats {

  /**
   *  The WorldModel
   */
  class WorldModel {

    friend class Singleton<WorldModel>;

  public:

    /**
     *  @returns the current simulation time.
     */
    float getTime() const;

    /**
     *  Sets the team name, which is used later on to identify which player
     *  is our and which player is theirs.
     */
    void setTeamName(std::string const &name);

    /**
     *  @returns our team name.
     */
    std::string getTeamName() const;

    /**
     * @returns the side of the field our team is playing on.
     */
    Types::Side getSide() const;

    /** Checks which side we are on, and flips if necessary to have neg x to our goal
     */
    Eigen::Vector3d checkFlipSide(Eigen::Vector3d const& v)
    {
      Eigen::Vector3d fv = v;
      if (getSide() == Types::RIGHT)
      {
        fv.x() = -v.x();
        fv.y() = -v.y();
      }
      return fv;
    }
    
    /**
     *  @returns the gamestate
     */
    Types::PlayMode getPlayMode() const;
    
    /**
     *  @returns whether the current play mode is in the provided list
     */
    bool isPlayMode(std::initializer_list<Types::PlayMode> playModes) const;

    /**
     * @returns the gamestate of last timestep
     */
    Types::PlayMode getLastPlayMode() const;

    /**
     * @returns whether our team has scored this timestep
     */
    bool weJustScored() const;

    /**
     * @returns whether the opponent has scored this timestep
     */
    bool theyJustScored() const;

    /**
     *  @returns the current game time.
     */
    double getGameTime() const;

    /**
     *  @returns the game time of the previous step.
     */
    double getPrevGameTime() const;

    /**
     *  @returns the amount of time between the previous and the current game time.
     */
    double getTimeStep() const;

    /**
     * /todo Move to a better place
     * @returns the direction of the center of the opponent's goal in polar coordinates
     */
    Eigen::Vector3d getDirectionOfOpponentGoal() const;

    /**
     * @returns he first time recieved by the server ever
     */
    double getFirstTime() const;

    /**
     * Update the world model. This updates the AgentSocketComm, reads all messages and integrates them into the model.
     */
    void update();

    /**
     * @returns the number of messages received by the agent during the lifetime of its process.
     */
    unsigned getNumMsgReceived() const;

    /**
     * @returns the radius of the ball in meters
     */
    double getBallRadius() const;

    /**
     * @returns the length of the field (in the x dimension)
     */
    double getFieldLength() const;

    /**
     * @returns the width of the field (in the y dimension)
     */
    double getFieldWidth() const;

    /**
     * @returns the width of the goal (in the y dimension)
     */
    double getGoalWidth() const;

    /**
     * @returns the height of the goal (in the z dimension)
     */
    double getGoalHeight() const;

    /**
     * @returns the length of the penalty area in the x-dimension
     */
    double getPenaltyXLength() const;

    /**
     * @returns the length of the penalty area in the y-dimension
     */
    double getPenaltyYLength() const;

    /**
     * @returns the number of players on each team
     */
    unsigned getNumberOfPlayers() const;

    /**
     * @return whether we get to take the next kick off. Only usable in Types::BEFORE_KICKOFF and Types::GOAL_US/GOAL_THEM
     */
    bool weGetKickOff() const;

    /**
     * @returns whether other team mates are closer to the ball than this agent. If @a standingOnly is true (default), only standing agents are considered. If @a returnCount is false (default), the method returns directly after the first team mate closer to the ball is encountered, otherwise it returns the total number of team mates closer by.
     */
    unsigned otherTeamMateCloserToBall(bool standingOnly = true, bool returnCount = false) __attribute__ ((deprecated));

    /**
     * @returns whether other team mates are closer to the given point than this agent. If @a standingOnly is true (default), only standing agents are considered. If @a returnCount is false (default), the method returns directly after the first team mate closer to the given point is encountered, otherwise it returns the total number of team mates closer by.
     */
    unsigned otherTeamMateCloserTo(Eigen::Vector3d const& posLocal, bool standingOnly = true, bool returnCount = false) __attribute__ ((deprecated));

    /**
     * @returns our score
     */
    unsigned getOurScore() const;

    /**
     * @returns the opponents score
     */
    unsigned getTheirScore() const;

    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:

    WorldModel(WorldModel const &); // NI
    WorldModel &operator=(WorldModel const &); // NI

    double d_time;

    std::string d_teamName;

    bool d_halfTimeRestart;
    
    double d_gameTime;
    double d_prevGameTime;
    double d_timeStep;
    Types::PlayMode d_playMode;
    Types::PlayMode d_lastPlayMode;
    Types::Side d_team;
    unsigned d_scoreUs;
    unsigned d_scoreThem;

    std::map<unsigned,unsigned> d_opponentUnums;

    std::map<unsigned,unsigned> d_playerUnums;

    bool d_catchFallEnabled;
    bool d_getUpTheOldFashionedWay;

    double d_firstTime;
    unsigned int d_numMsgReceived;

    bool d_weGetKickOff;

    double d_ballRadius;
    double d_fieldLength;
    double d_fieldWidth;
    double d_goalWidth;
    double d_goalHeight;
    double d_penaltyXLength;
    double d_penaltyYLength;

    unsigned d_numberOfPlayers;
    
    void destroy();

    void updateGameState();

    WorldModel();

  };

  typedef Singleton<WorldModel> SWorldModel;


  // Inline method implementations


  inline float WorldModel::getTime() const
  {
    return d_time;
  }

  inline void WorldModel::setTeamName(std::string const &name)
  {
    d_teamName = name;
  }

  inline std::string WorldModel::getTeamName() const
  {
    return d_teamName;
  }

  inline Types::Side WorldModel::getSide() const
  {
    return d_team;
  }

  inline Types::PlayMode WorldModel::getPlayMode() const
  {
    return d_playMode;
  }
  
  inline bool WorldModel::isPlayMode(std::initializer_list<Types::PlayMode> playModes) const
  {
    for (Types::PlayMode playMode : playModes)
        if (d_playMode == playMode)
          return true;
    return false;
  }

  inline Types::PlayMode WorldModel::getLastPlayMode() const
  {
    return d_lastPlayMode;
  }

  inline bool WorldModel::weJustScored() const
  {
    return (d_team == Types::LEFT  && d_playMode == Types::GOAL_LEFT  && d_lastPlayMode != Types::GOAL_LEFT) ||
           (d_team == Types::RIGHT && d_playMode == Types::GOAL_RIGHT && d_lastPlayMode != Types::GOAL_RIGHT);
  }

  inline bool WorldModel::theyJustScored() const
  {
    return (d_team == Types::RIGHT && d_playMode == Types::GOAL_LEFT  && d_lastPlayMode != Types::GOAL_LEFT) ||
           (d_team == Types::LEFT  && d_playMode == Types::GOAL_RIGHT && d_lastPlayMode != Types::GOAL_RIGHT);
  }

  inline double WorldModel::getGameTime() const
  {
    return d_gameTime;
  }

  inline double WorldModel::getPrevGameTime() const
  {
    return d_prevGameTime;
  }

  inline double WorldModel::getTimeStep() const
  {
    return d_timeStep;
  }

  inline double WorldModel::getFirstTime() const
  {
    return d_firstTime;
  }

  inline unsigned WorldModel::getNumMsgReceived() const
  {
    return d_numMsgReceived;
  }

  inline double WorldModel::getBallRadius() const
  {
    return d_ballRadius;
  }

  inline double WorldModel::getFieldLength() const
  {
    return d_fieldLength;
  }

  inline double WorldModel::getFieldWidth() const
  {
    return d_fieldWidth;
  }

  inline double WorldModel::getGoalWidth() const
  {
    return d_goalWidth;
  }

  inline double WorldModel::getGoalHeight() const
  {
    return d_goalHeight;
  }

  inline double WorldModel::getPenaltyXLength() const
  {
    return d_penaltyXLength;
  }

  inline double WorldModel::getPenaltyYLength() const
  {
    return d_penaltyYLength;
  }

  inline unsigned WorldModel::getNumberOfPlayers() const
  {
    return d_numberOfPlayers;
  }

  inline bool WorldModel::weGetKickOff() const
  {
    return d_weGetKickOff;
  }

  inline unsigned WorldModel::getOurScore() const
  {
      return d_scoreUs;
  }

  inline unsigned WorldModel::getTheirScore() const
  {
      return d_scoreThem;
  }

};

#endif // __INC_BATS_WORLDMODEL_HH_
