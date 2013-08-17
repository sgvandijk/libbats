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
#ifndef BATS_MOTIONSEQUENCEPLAYER_HH
#define BATS_MOTIONSEQUENCEPLAYER_HH

#include "../jointcontroller.hh"
#include "../../MotionSequence/motionsequence.hh"
#include <list>
#include <Eigen/Core>

namespace bats
{
  /** Motion sequence player
   *
   * Joint Controller that can play a predefined motion sequence. A
   * sequences is defined by a @a MotionSequence object. You can build
   * one in code, but the sequence can also be loaded from
   * configuration. The following is the BNF syntax for the
   * configuration section:
   *
    \verbatim
    <msp>                ::= '<motionsequenceplayer id="' <id> '">
                               <sequence-node>
                               [<symmetric-node>]
                             '</motionsequenceplayer>'
    <sequence-node>      ::= '<sequence>' <sequence-def> '</sequence>'
    <sequence-def>       ::= <joint-sequence-def>+
                             <end-def>
    <joint-sequence-def> ::= <joint-id>: <frame>(, <frame>)*;
    <frame>              ::= <time> <angle>
    <symmetric-node>     ::= '<issymmetric>' (0|1) '</issymetric>'
    <end-def>            ::= &end;: <end-time>
    \endverbatim
   *
   * A @a joint-id is an integer, however XML entities such as
   * @a &amp;larm1; and @a &amp;rleg4; are defined to make things more
   * readible. The times given in the sequence are in seconds and
   * relative to the start of the sequence.
   *
   * When played, joints will move from one angle to the next as
   * defined in the frames. Linear interpolation is used between the
   * time stamps of successive frames. After @a end-def seconds, the
   * sequence player is marked as done, which can be checekd with @a
   * isSequenceDone.
   *
   * If @a issymmetric is set to 1 (default is 0), you only need to
   * define sequences for joints on the left side of the body and the
   * right ones will move symmetrically.
   *
   * The following is example that moves the first joints of both arms
   * (note @a issymmetric) all the way up in one second, then all the
   * way down in another second, while the knee bends 90 degrees
   * within 1.5 seconds and then flex again in 2 seconds. THe sequence
   * is marked as done after 4 seconds.
   *
   * \code{.xml}
   * <motionsequenceplayer id="playerid">
   *   <sequence>
   *     &larm1;: 0 0, 1 90, 2 -90;
   *     &lleg4;: 0 0, 1.5 -90, 3.5 0;
   *     &end;: 4;
   *   </sequence>
   *   <issymmetric>1</issymmetric>
   * </motionsequenceplayer>
   * \endcode
   * 
   */
  class MotionSequencePlayer : public JointController
  {
  public:
    /**
     * @param id Sequence player id. Should be referenced in configuration file when loading sequence from configuration.
     */
    MotionSequencePlayer(std::string const& id);
    
    /** Load motion sequence from configuration
     *
     * @see The detailed description of MotionSequencePlayer for configuration syntax.
     */
    void loadSequenceFromConf();

    /** Set pre-constructed sequence
     */
    void setSequence(MotionSequence const& sequence);

    /** Run sequence player
     *
     * Run this sequence player in this timestep. Calling this method
     * updates the velocities returned by @a getJointVelocities. If
     * this method was not called at the previous timestep, @a reset
     * is called and the player starts at t=0. Otherwise, the sequence
     * is progressed from the previous time.
     */
    virtual void run(JointControlParams* params);

    /** Check whether sequence has endeed
     */
    bool isSequenceDone() const;

    /** Reset player to start it from the beginning
     */
    void reset() { d_lastRanTime = 0; }

  private:
    MotionSequence d_sequence;
    double d_sequenceStartTime;
    Eigen::VectorXd d_startJoingAngles;
    double d_lastRanTime;
    bool d_sequenceDone;

    void checkFirstRun();
    void runCurrentStep();
    void checkSequenceDone();
  };

  // Member implementations
  inline MotionSequencePlayer::MotionSequencePlayer(std::string const& id)
    : JointController(id),
      d_lastRanTime(0),
      d_sequenceStartTime(0)
  {
    setTag("motionsequenceplayer");
  }

  inline void MotionSequencePlayer::setSequence(MotionSequence const& sequence)
  {
    d_sequence = sequence;
  }

  inline bool MotionSequencePlayer::isSequenceDone() const
  {
    return d_sequenceDone;
  }
}

#endif
