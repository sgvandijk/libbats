/*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors:  Martin Klomp (martin@ai.rug.nl)
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
 *  License:  This program is free software; you can redistribute 
 *    it and/or modify it under the terms of the GNU General
 *    Public License as published by the Free Software 
 *    Foundation; either version 3 of the License, or (at 
 *    your option) any later version.
 *
 *      This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or FITNESS FOR A
 *    PARTICULAR PURPOSE.  See the GNU General Public
 *    License for more details.
 *
 *      You should have received a copy of the GNU General
 *    Public License along with this program; if not, write
 *    to the Free Software Foundation, Inc., 59 Temple Place - 
 *    Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef BATS_PLAYERINFO_HH
#define BATS_PLAYERINFO_HH

#include "../../../Types/types.hh"
#include "../../objectinfo.hh"
#include "../dynamicobjectinfo.hh"

#include <Eigen/Core>

namespace bats
{
  /** Player information
   *
   * Keep track of a player's information, most notably the position
   * of its limbs, and its orientation derived from that.
   */
  struct PlayerInfo : DynamicObjectInfo
  {
    /** The position of the left arm, relative to the player's torso,
     * in the local frame of the observer. */
    Eigen::Vector3d posLArmLocal;
    /** The position of the right arm, relative to the player's torso,
     * in the local frame of the observer. */
    Eigen::Vector3d posRArmLocal;
    /** The position of the left foot, relative to the player's torso,
     * in the local frame of the observer. */
    Eigen::Vector3d posLFootLocal;
    /** The position of the right foot, relative to the player's
     * torso, in the local frame of the observer. */
    Eigen::Vector3d posRFootLocal;

    /** The position of the left arm, relative to the player's torso,
     * in the global frame. */
    Eigen::Vector3d posLArmGlobal;
    /** The position of the right arm, relative to the player's torso,
     * in the global frame. */
    Eigen::Vector3d posRArmGlobal;
    /** The position of the left foot, relative to the player's torso,
     * in the global frame. */
    Eigen::Vector3d posLFootGlobal;
    /** The position of the right foot, relative to the player's
     * torso, in the global frame. */
    Eigen::Vector3d posRFootGlobal;
    
    /** Whether the left arm is visible */
    bool lArmVisible;
    /** Whether the right arm is visible */
    bool rArmVisible;
    /** Whether the left foot is visible */
    bool lFootVisible;
    /** Whether the right foor is visible */
    bool rFootVisible;
    
    /** Gets the direction this player is believed to be facing, in
     * the agent's local frame. */
    Eigen::Vector3d faceDirLocal;
    /** Gets the direction this player is believed to be facing, in
     * the global local frame. */
    Eigen::Vector3d faceDirGlobal;
    
    /** Gets whether this player is believed to be lying due to a
     * fall. */
    bool fallen;
    
    /** Gets whether this player is on the same team as the running
     * agent.  Note that this value is true for the PlayerInfo object
     * which represents self. */
    const bool isTeamMate;
    /** Gets whether this player is on the opposing team. */
    const bool isOpponent;
    /** Gets whether this PlayerInfo object represents myself (the
     * running agent). */
    const bool isMe;
    /** Gets whether this player is a goal keeper of either team
     * (using convention that the keeper has uniform number 1. */
    const bool isKeeper;
    /** Gets the uniform number of this player. */
    const unsigned unum;
    
    PlayerInfo(Types::Object objectId, double radius);
  };
}

#endif
