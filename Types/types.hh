/*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors:   Martin Klomp (martin@ai.rug.nl)
 *    Mart van de Sanden (vdsanden@ai.rug.nl)
 *    Sander van Dijk (sgdijk@ai.rug.nl)
 *    A. Bram Neijt (bneijt@gmail.com)
 *    Matthijs Platje (mplatje@gmail.com)
 *
 *  All students of AI at the University of Groningen
 *  at the time of writing. See: http://www.ai.rug.nl/
 *
 *  Date:   November 1, 2008
 *
 *  Website:  http://www.littlegreenbats.nl
 *
 *  Comment:  Please feel free to contact us if you have any
 *    problems or questions about the code.
 *
 *
 *  License:   This program is free software; you can redistribute
 *    it and/or modify it under the terms of the GNU General
 *    Public License as published by the Free Software
 *    Foundation; either version 3 of the License, or (at
 *    your option) any later version.
 *
 *       This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or FITNESS FOR A
 *    PARTICULAR PURPOSE.  See the GNU General Public
 *    License for more details.
 *
 *       You should have received a copy of the GNU General
 *    Public License along with this program; if not, write
 *    to the Free Software Foundation, Inc., 59 Temple Place -
 *    Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef _BATS_TYPES_HH_
#define _BATS_TYPES_HH_

#include <string>

namespace bats
{
  /** A collection of types used at different places */
  class Types
  {
  public:
    /**
     *  The possible play modes.
     */
    enum PlayMode
    {
      UNKNOWN = 0,
      BEFORE_KICKOFF,   // 1

      KICKOFF_LEFT,     // 2
      KICKOFF_RIGHT,
      KICKOFF_US,
      KICKOFF_THEM,

      PLAY_ON,          // 6

      FREEKICK_LEFT,    // 7
      FREEKICK_RIGHT,
      FREEKICK_US,
      FREEKICK_THEM,

      GOAL_LEFT,        // 11
      GOAL_RIGHT,
      GOAL_US,
      GOAL_THEM,

      GOAL_KICK_LEFT,   // 15
      GOAL_KICK_RIGHT,
      GOAL_KICK_US,
      GOAL_KICK_THEM,

      CORNER_KICK_LEFT, // 19
      CORNER_KICK_RIGHT,
      CORNER_KICK_US,
      CORNER_KICK_THEM,

      KICKIN_LEFT,      // 23
      KICKIN_RIGHT,
      KICKIN_US,
      KICKIN_THEM,
      
      GAME_OVER,        // 27
      
      NPLAYMODE         // 28
    };

    /**
     *  Sides of the field/robot/etc.
     */
    enum Side
    {
      LEFT = 0,
      RIGHT,
      UNKNOWN_SIDE
    };

    /// Enumeration of the agent's joints
    enum Joint
    {
      HEAD1 = 0,///< Neck, Z-Axis (-120, 120)
      HEAD2,    ///< Neck, X-Axis (-45, 45)

      LLEG1,    ///< Left hip, XZ-Axis (-90, 1)
      LLEG2,    ///< Left hip, X-Axis (-25, 100)
      LLEG3,    ///< Left hip, Y-Axis (-25, 45)
      LLEG4,    ///< Left knee, X-Axis (-130, 1)
      LLEG5,    ///< Left ankle, X-Axis (-45, 75)
      LLEG6,    ///< Left ankle, Y-Axis (-25, 45)

      RLEG1,    ///< Right hip, XZ-Axis (-90, 1)
      RLEG2,    ///< Right hip, X-Axis (-25, 100)
      RLEG3,    ///< Right hip, Y-Axis (-45, 25)
      RLEG4,    ///< Right knee, X-Axis (-130, 1)
      RLEG5,    ///< Right ankle, X-Axis (-45, 75)
      RLEG6,    ///< Right ankle, Y-Axis (-25, 45)

      LARM1,    ///< Left shoulder, X-Axis (-120, 120)
      LARM2,    ///< Left shoulder, Z-Axis (-1, 95)
      LARM3,    ///< Left shoulder, Y-Axis (-120, 120)
      LARM4,    ///< Left elbow, Z-Axis (-90, 1)

      RARM1,    ///< Right shoulder, X-Axis (-120, 120)
      RARM2,    ///< Right shoulder, Z-Axis (-95, 1)
      RARM3,    ///< Right shoulder, Y-Axis (-120, 120)
      RARM4,    ///< Right elbow, Z-Axis (-1, 90)

      NJOINTS
    };

    enum JointType
    {
      NO_JOINT = 0,

      UNIVERSAL_JOINT, ///< A universal joint.
      HINGE_JOINT,     ///< A hinge joint.
      TORQUE_JOINT,    ///< A hinge torque joint.

      JOINTTYPE,
    };

    /// Enumeration of objects in the world. \todo Check flag/goalpost positions (again...)
    enum Object
    {
      // 0
      BALL = 0,    ///< The big round orange thingy

      // 1
      TEAMMATE1,   ///< Our team's goalie
      TEAMMATE2,   ///< Our team's 2nd player.
      TEAMMATE3,   ///< Our team's 3rd player.
      TEAMMATE4,   ///< Our team's 4th player.
      TEAMMATE5,   ///< Our team's 5th player.
      TEAMMATE6,   ///< Our team's 6th player.
      TEAMMATE7,   ///< Our team's 7th player.
      TEAMMATE8,   ///< Our team's 8th player.
      TEAMMATE9,   ///< Our team's 9th player.
      TEAMMATE10,  ///< Our team's 10th player.
      TEAMMATE11,  ///< Our team's 11th player.

      // 12
      OPPONENT1,   ///< Thier team's keeper.
      OPPONENT2,   ///< Thier team's 2nd player.
      OPPONENT3,   ///< Thier team's 3rd player.
      OPPONENT4,   ///< Thier team's 4th player.
      OPPONENT5,   ///< Thier team's 5th player.
      OPPONENT6,   ///< Thier team's 6th player.
      OPPONENT7,   ///< Thier team's 7th player.
      OPPONENT8,   ///< Thier team's 8th player.
      OPPONENT9,   ///< Thier team's 9th player.
      OPPONENT10,  ///< Thier team's 10th player.
      OPPONENT11,  ///< Thier team's 11th player.

      // 23
      FLAG1US,     ///< Left flag on our side of the field (ie. with your back to our own goal)
      FLAG2US,     ///< Right flag on our side of the field (ie. with your back to our own goal)
      FLAG1THEM,   ///< Left flag on their side of the field (ie. with your back to their goal)
      FLAG2THEM,   ///< Right flag on their side of the field (ie. with your back to their goal)

      // 27
      GOAL1US,     ///< Left goal post on our side of the field (ie. with your back to our own goal)
      GOAL2US,     ///< Right goal post on our side of the field (ie. with your back to our own goal)
      GOAL1THEM,   ///< Left goal post on their side of the field (ie. with your back to their goal)
      GOAL2THEM,   ///< Right goal post on their side of the field (ie. with your back to their goal)

      // 31
      FIELDCENTER, ///< Center point of the field, located at (0,0)
      
      // 32
      NOBJECTS
    };

    /// Enumeration of body parts.
    enum BodyPart
    {
      HEAD = 0,
      NECK,

      TORSO,
      LOWERTORSO,

      LSHOULDER,
      LUPPERARM,
      LELBOW,
      LLOWERARM,
      LHAND1,
      LHAND2,

      RSHOULDER,
      RUPPERARM,
      RELBOW,
      RLOWERARM,
      RHAND1,
      RHAND2,

      LHIP1,
      LHIP2,
      LUPPERLEG,
      LLOWERLEG,
      LANKLE,
      LFOOT,

      RHIP1,
      RHIP2,
      RUPPERLEG,
      RLOWERLEG,
      RANKLE,
      RFOOT,

      NBODYPARTS
    };
       
    
    /**
     * Check whether a joint is part of a universal joint
     */
    static bool isUniversalJoint(Joint joint)
    {
      return joint == LLEG2 ||
             joint == LLEG3 ||
             joint == LLEG5 ||
             joint == LLEG6 ||
             joint == RLEG2 ||
             joint == RLEG3 ||
             joint == RLEG5 ||
             joint == RLEG6 ||
             joint == LARM1 ||
             joint == LARM2 ||
             joint == RARM1 ||
             joint == RARM2;
    }
    
    /**
     * Return the enum name for a given object enum
     * This function is an inverse of Types::objectEnumFor
     */
    static std::string nameOf(Types::PlayMode const obj);
    
    /**
     * Return the enum name for a given object enum
     * This function is an inverse of Types::objectEnumFor
     */
    static std::string nameOf(Types::Side const obj);
    
    /**
     * Return the enum name for a given object enum
     * This function is an inverse of Types::objectEnumFor
     */
    static std::string nameOf(Types::Joint const obj);
    
    /**
     * Return the enum name for a given object enum
     * This function is an inverse of Types::objectEnumFor
     */
    static std::string nameOf(Types::JointType const obj);
    
    /**
     * Return the enum name for a given object enum
     * This function is an inverse of Types::objectEnumFor
     */
    static std::string nameOf(Types::Object const obj);
    
    /**
     * Return the enum name for a given object enum
     * This function is an inverse of Types::objectEnumFor
     */
    static std::string nameOf(Types::BodyPart const obj);

    // TODO remove all of the below methods, using properties of PlayerInfo instead
    
    /** Return the Object enum for a given name, case sensitive
     *
     * @param a Object name
     * @param def Default return value, in case of unknown object
     */
    static Types::PlayMode objectEnumFor(std::string const &a, Types::PlayMode def = NPLAYMODE);
    
    /** Return the Object enum for a given name, case sensitive
     *
     * @param a Object name
     * @param def Default return value, in case of unknown object
     */
    static Types::Object objectEnumFor(std::string const &a, Types::Object def = NOBJECTS);
    
    /** Return the uniform number for a given player, either team mate or opponent
     * 
     * @param player Player or Opponent enum value
     */
    static unsigned unumFor(Types::Object objectId);

    /** Return the Object enum for a given player (team mate)
     * 
     * @param unum The uniform number of the required player
     */
    static Types::Object getTeamMateWithUnum(unsigned unum);
    
    /** Return the Object enum for a given opponent
     * 
     * @param unum The uniform number of the required opponent
     */
    static Types::Object getOpponentWithUnum(unsigned unum);
    
    /** Gets whether the specified Object is a team mate */
    static bool isTeamMate(Types::Object objectId);

    /** Gets whether the specified Object is an opposition player */
    static bool isOpponent(Types::Object objectId);

    /** Gets whether the specified Object is a player (either team mate or opponent) */
    static bool isPlayer(Types::Object objectId);

    /** Gets whether the specified Object is one of the four corner flags */
    static bool isFlag(Types::Object objectId);

    /** Gets whether the specified Object represents the current agent */
    static bool isMe(Types::Object objectId);
    
    /** Gets whether the specified Object represents a keeper from either team */
    static bool isKeeper(Types::Object objectId);
    
    /** Gets the max PLAYER object, given the configured number of players in this game */
    static Types::Object getMaxPlayer();
    
    /** Gets the max OPPONENT object, given the configured number of players in this game */
    static Types::Object getMaxOpponent();
  };
};

#endif
