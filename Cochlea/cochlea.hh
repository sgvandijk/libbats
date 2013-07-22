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

#ifndef BATS_COCHLEA_HH
#define BATS_COCHLEA_HH

#include "../Singleton/singleton.hh"
#include "../Types/types.hh"
#include "../Math/math.hh"
#include <memory>
#include <unordered_map>
#include <map>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace bats
{
  class Predicate;

  /** Visible line
   */
  class VisibleLine
  {
  public:
    VisibleLine() {}

    VisibleLine(const Eigen::Vector3d end1Polar, const Eigen::Vector3d end2Polar)
    : end1Polar(end1Polar),
      end2Polar(end2Polar),
      end1Cartesian(Math::polarToCartesian(end1Polar)),
      end2Cartesian(Math::polarToCartesian(end2Polar)),
      length((end1Cartesian - end2Cartesian).norm())
    {}
    
    Eigen::Vector3d end1Polar;
    Eigen::Vector3d end2Polar;
    Eigen::Vector3d end1Cartesian;
    Eigen::Vector3d end2Cartesian;
    double length;
    
    Eigen::Vector3d toVector3d() { return end1Cartesian - end2Cartesian; };
  };

  /** Visible line junction
   */
  class VisibleLineJunction
  {
  public:
    VisibleLineJunction(const VisibleLine line1, const VisibleLine line2, const Eigen::Vector3d line1EndPolar, const Eigen::Vector3d line2EndPolar)
    : line1(line1),
      line2(line2),
      line1EndPolar(line1EndPolar),
      line2EndPolar(line2EndPolar),
      line1EndCartesian(Math::polarToCartesian(line1EndPolar)),
      line2EndCartesian(Math::polarToCartesian(line2EndPolar))
    {}
    
    VisibleLine line1;
    VisibleLine line2;
    Eigen::Vector3d line1EndPolar;
    Eigen::Vector3d line2EndPolar;
    Eigen::Vector3d line1EndCartesian;
    Eigen::Vector3d line2EndCartesian;
  };
  
  /** Sensory data pre-processor
   *
   * The Cochlea forms the initial entry point for sensory data. It
   * takes information from the predicate data received from the
   * simulation server, and offers it in an indexed binary form.
   */
  class Cochlea
  {
    friend class Singleton<Cochlea>;
    
  public:
    /**
     * Enumeration of all predicate types
     */
    enum InfoID {

      iNone = 0,
      
      iGameState,
      iUnum,
      iTeam,
      iGameTime,
      iPlayMode,
      
      iTime,         
      iNow,          ///< The current playing time.
      iStep,         ///< The current time step (dt?)
      iVision,
      iVisionFlag1L, ///< First flag on the left side.
      iVisionFlag2L, ///< Second flag on the left side.
      iVisionFlag1R, ///< First flag on the right side.
      iVisionFlag2R, ///< Second flag on the right side.
      iVisionGoal1L, 
      iVisionGoal2L, 
      iVisionGoal1R, 
      iVisionGoal2R, 
      iVisionFieldCenter,
      iVisionTeamMate1,
      iVisionTeamMate2,
      iVisionTeamMate3,
      iVisionTeamMate4,
      iVisionTeamMate5,
      iVisionTeamMate6,
      iVisionTeamMate7,
      iVisionTeamMate8,
      iVisionTeamMate9,
      iVisionTeamMate10,
      iVisionTeamMate11,
      iVisionPlayerNone,
      iVisionOpponent1,
      iVisionOpponent2,
      iVisionOpponent3,
      iVisionOpponent4,
      iVisionOpponent5,
      iVisionOpponent6,
      iVisionOpponent7,
      iVisionOpponent8,
      iVisionOpponent9,
      iVisionOpponent10,
      iVisionOpponent11,
      iVisionOpponentNone,
      iVisionBall,   ///< Position of the ball.
      iVisionLine,   ///< A line segment.
      iVisionSelfGT,   ///< Ground truth of own position.
      iVisionBallGT,   ///< Ground truth of ball position.
      iUJ,
      iHJ,
      iHJT,
      iName,
      iAxis,
      iRate,
      iTorque,
      iAxis1,
      iRate1,
      iAxis2,
      iRate2,

      iHeadJoint1,
      iHeadJoint2,
      
      iLLegJoint1,
      iLLegJoint2,
      iLLegJoint3,
      iLLegJoint4,
      iLLegJoint5,
      iLLegJoint6,

      iRLegJoint1,
      iRLegJoint2,
      iRLegJoint3,
      iRLegJoint4,
      iRLegJoint5,
      iRLegJoint6,

      iLArmJoint1,  ///< 
      iLArmJoint2,  ///< 
      iLArmJoint3,
      iLArmJoint4,

      iRArmJoint1,
      iRArmJoint2,
      iRArmJoint3,
      iRArmJoint4,

      iGyro,
      iTorsoGyro,
      iTouch,
      iFootLeft,
      iFootLeft2,  /// dummy
      iFootRight,
      iFootRight2,
      iVal,
      
      iForceResistancePerceptor,
      iContact,
      iForce,

      iHear,
      
      iP, /// Player vision info.

      iBody,
      iBodyHead,
      iBodyRLowerArm,
      iBodyLLowerArm,
      iBodyRFoot,
      iBodyLFoot,

      iRLowerArmTeamMate1,
      iRLowerArmTeamMate2,
      iRLowerArmTeamMate3,
      iRLowerArmTeamMate4,
      iRLowerArmTeamMate5,
      iRLowerArmTeamMate6,
      iRLowerArmTeamMate7,
      iRLowerArmTeamMate8,
      iRLowerArmTeamMate9,
      iRLowerArmTeamMate10,
      iRLowerArmTeamMate11,
      
      iLLowerArmTeamMate1,
      iLLowerArmTeamMate2,
      iLLowerArmTeamMate3,
      iLLowerArmTeamMate4,
      iLLowerArmTeamMate5,
      iLLowerArmTeamMate6,
      iLLowerArmTeamMate7,
      iLLowerArmTeamMate8,
      iLLowerArmTeamMate9,
      iLLowerArmTeamMate10,
      iLLowerArmTeamMate11,
      
      iRFootTeamMate1,
      iRFootTeamMate2,
      iRFootTeamMate3,
      iRFootTeamMate4,
      iRFootTeamMate5,
      iRFootTeamMate6,
      iRFootTeamMate7,
      iRFootTeamMate8,
      iRFootTeamMate9,
      iRFootTeamMate10,
      iRFootTeamMate11,
      
      iLFootTeamMate1,
      iLFootTeamMate2,
      iLFootTeamMate3,
      iLFootTeamMate4,
      iLFootTeamMate5,
      iLFootTeamMate6,
      iLFootTeamMate7,
      iLFootTeamMate8,
      iLFootTeamMate9,
      iLFootTeamMate10,
      iLFootTeamMate11,
      
      iRLowerArmOpponent1,
      iRLowerArmOpponent2,
      iRLowerArmOpponent3,
      iRLowerArmOpponent4,
      iRLowerArmOpponent5,
      iRLowerArmOpponent6,
      iRLowerArmOpponent7,
      iRLowerArmOpponent8,
      iRLowerArmOpponent9,
      iRLowerArmOpponent10,
      iRLowerArmOpponent11,
      
      iLLowerArmOpponent1,
      iLLowerArmOpponent2,
      iLLowerArmOpponent3,
      iLLowerArmOpponent4,
      iLLowerArmOpponent5,
      iLLowerArmOpponent6,
      iLLowerArmOpponent7,
      iLLowerArmOpponent8,
      iLLowerArmOpponent9,
      iLLowerArmOpponent10,
      iLLowerArmOpponent11,
      
      iRFootOpponent1,
      iRFootOpponent2,
      iRFootOpponent3,
      iRFootOpponent4,
      iRFootOpponent5,
      iRFootOpponent6,
      iRFootOpponent7,
      iRFootOpponent8,
      iRFootOpponent9,
      iRFootOpponent10,
      iRFootOpponent11,
      
      iLFootOpponent1,
      iLFootOpponent2,
      iLFootOpponent3,
      iLFootOpponent4,
      iLFootOpponent5,
      iLFootOpponent6,
      iLFootOpponent7,
      iLFootOpponent8,
      iLFootOpponent9,
      iLFootOpponent10,
      iLFootOpponent11,

      iAcc,
      iAccA,
      
      // Should always have the highest numeric value.
      iInfoID,
    };

    /** Auditory message
     */
    struct HearMessage
    {
      /// Time at which message is received
      double time;

      /// Angle relative to torso of the direction the message came from
      double angle; 

      /// Message content
      std::string message;
      
      HearMessage(double t, double a, std::string const& m)
      : time(t), angle(a), message(m)
      {}
    };
    
  public:
    /** Get a printable name of an info ID
     */
    static std::string nameOf(Cochlea::InfoID const infoId);
    
    /** Get the translation of @a mode to a Types::PlayMode value
     */
    Types::PlayMode getPlayMode(std::string const& mode)
    {
      if (d_playModeMap.find(mode) != d_playModeMap.end())
        return d_playModeMap[mode];
      else
        return Types::UNKNOWN;
    }
    
    /** Set the name of your own team
     *
     * Used to recognize team mates and opponents
     */
    void setTeamName(std::string const& teamName) { d_teamName = teamName; }
    
    /** Get the name of the opponent team
     */
    std::string getOpponentTeamName() const { return d_opponentTeamName; }
    
    /** Define name translation
     * 
     * This is primarily used to map joint pereptor names to Cochlea's
     * internal names.
     */
    void setTranslation(std::string const& from, std::string const& to)
    {
      d_infoMap[from] = d_infoMap[to];
    }
    
    /** Translate an information name
     * 
     * @returns the translation of @a name to an InfoID value
     */
    InfoID translateInfo(std::string const &name);

    /** Update cochlea
     *
     * Updates all values by integrating the latest predicate received
     * by AgentSocketComm. This should be called every think cycle.
     */
    void update();

    /** Get info value
     *
     * @returns the value of the information with id @a id in the form
     * of a 4-dimensional vector
     */	
    Eigen::Vector4d getInfo(InfoID id) const { return d_info[id]; }

    /** Get latest heard message
     */
    HearMessage getHearMessage() const { return d_hearMessage; }

    /** Get visible lines
     *
     * @returns all visible line segments seen in the current cycle in
     * polar form (from the camera's perspective)
     */
    std::vector<VisibleLine> getLines() const { return d_lines; }
    
    /** Get filtered visible lines
     *
     * @returns all visible line segments seen in the current cycle in
     * polar form (from the camera's perspective), for which @a
     * predicate returns true.
     */
    std::vector<VisibleLine> findLines(std::function<bool(VisibleLine)> predicate)
    {
      std::vector<VisibleLine> results(d_lines.size());
      auto endit = std::copy_if(d_lines.begin(), d_lines.end(),
                                results.begin(),
                                predicate);
      results.resize(std::distance(results.begin(), endit));
      return results;
    }
    
    /** Get visible line ends
     *
     * @returns all line ends seen in the current cycle in polar form
     * (from the camera's perspective)
     */
    std::vector<Eigen::Vector3d> getLineEnds() const { return d_lineEnds; }
    
    /** Get visible line junctions
     *
     * @param lines set of lines to check for junctions
     * @param thresholdDistance maximum distance at which lines are considered joined
     * @returns junctions within the provided set of lines
     */
    std::vector<VisibleLineJunction> getLineJunctions(std::vector<VisibleLine> lines,
                                                      const double thresholdDistance = 0.4) const;
    
    /** Get latest measurement time
     *
     * @returns the absolute time of the last measurement of this value
     */
    double getTimestamp(InfoID id) const { return d_timestamps[id]; }

    /** Get time since latest measurement
     *
     * @returns the difference between the current time and the time
     * of the last measurement of this value
     */
    double getDelay(InfoID id) const { return d_timestamps[id] - d_info[iNow](0); }

    /** Get time between last measurements
     *
     * @returns the difference between time between the last and the
     * penultimate measurements of this value
     */
    double getDt(InfoID id) const { return d_dt[id]; }
    
    /** Get ground truth transformation
     *
     * Gets a Affine3d with ground truth data about the agent's global position.
     * This value is not available during competitions, and will only be available
     * during development if the server is configured to sent it:
     * 
     * Edit file
     * /usr/local/share/rcssserver3d/rsg/agent/nao/naoneckhead.rsg and
     * make sure (setSenseMyPos true) is present
     * 
     * The server may provide only x/y/z translation, in which case the three axis
     * vectors in this transformation matrix will be zero.
     * 
     * Note that this matrix represents the frame of the agent's camera, which is
     * translated relative to the torso, and includes rotation of the neck joints.
     */
    Eigen::Affine3d getSelfTransform() const { return d_selfTransform; }
    
    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    std::unordered_map<std::string, InfoID> d_infoMap;
    std::unordered_map<std::string, Types::PlayMode> d_playModeMap;

    std::map<Types::Object, InfoID> d_objectTypesMap;
    
    Eigen::Vector4d d_info[iInfoID];
    double d_timestamps[iInfoID];
    double d_dt[iInfoID];
    std::vector<VisibleLine> d_lines;
    std::vector<Eigen::Vector3d> d_lineEnds;
    
    Eigen::Affine3d d_selfTransform;
    HearMessage d_hearMessage;

    std::string d_teamName;
    std::string d_opponentTeamName;

    bool d_frpFix;

    Cochlea();
    
    Eigen::Vector4d parseVect(std::shared_ptr<Predicate> const &pred);

    Eigen::Vector4d parsePolar(std::shared_ptr<Predicate> const &pred);

    Eigen::Vector4d parseScalar(std::shared_ptr<Predicate> const &pred);

    Eigen::Affine3d parseTransform(std::shared_ptr<Predicate> const &pred);

    void integrate(std::shared_ptr<Predicate> const &pred);
    
    void synthesizeDerivedData();
  };
  
  typedef Singleton<Cochlea> SCochlea;
}

#endif

