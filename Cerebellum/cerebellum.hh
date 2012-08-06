/*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors:     Martin Klomp (martin@ai.rug.nl)
 *        Mart van de Sanden (vdsanden@ai.rug.nl)
 *        Sander van Dijk (sgdijk@ai.rug.nl)
 *        A. Bram Neijt (bneijt@gmail.com)
 *        Matthijs Platje (mplatje@gmail.com)
 *
 *    All students of AI at the University of Groningen
 *  at the time of writing. See: http://www.ai.rug.nl/
 *
 *  Date:     November 1, 2008
 *
 *  Website:    http://www.littlegreenbats.nl
 *
 *  Comment:    Please feel free to contact us if you have any 
 *        problems or questions about the code.
 *
 *
 *  License:     This program is free software; you can redistribute 
 *        it and/or modify it under the terms of the GNU General
 *        Public License as published by the Free Software 
 *        Foundation; either version 3 of the License, or (at 
 *        your option) any later version.
 *
 *           This program is distributed in the hope that it will
 *        be useful, but WITHOUT ANY WARRANTY; without even the
 *        implied warranty of MERCHANTABILITY or FITNESS FOR A
 *        PARTICULAR PURPOSE.  See the GNU General Public
 *        License for more details.
 *
 *           You should have received a copy of the GNU General
 *        Public License along with this program; if not, write
 *        to the Free Software Foundation, Inc., 59 Temple Place - 
 *        Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef _BATS_CEREBELLUM_HH_
#define _BATS_CEREBELLUM_HH_

#include "../Singleton/singleton.hh"
#include "../Types/types.hh"
#include "../Action/RC3DAction/rc3daction.hh"
#include "../SocketComm/AgentSocketComm/agentsocketcomm.hh"
#include <Eigen/Core>
#include <vector>

namespace bats
{
  /** Cerebellum
   *
   * The cerebellum integrates different actions. Useful when different behaviors that control the same joints run in parallel
   */
  class Cerebellum
  {
    friend class Singleton<Cerebellum>;
      
    std::vector<rf<Action> > d_actions;
        
    double d_prevJointValues[Types::NJOINTS];

    Cerebellum(Cerebellum const &); // NI
    Cerebellum &operator=(Cerebellum const &); // NI

    Cerebellum();
  public:
        
    /** Add an action into the cerebellum
     *
     * @param a The action to perform
     */
    void addAction(rf<Action> a);
        
    /** Output action commands
     *
     * Integrates the stored actions and outputs the resulting commands to the given AgentSocketComm. After sending the list of stored actions is cleared.
     * @param comm The AgentSocketComm used to communicate with the server
     */
    void outputCommands(AgentSocketComm& comm);
        
  };
    
  typedef Singleton<Cerebellum> SCerebellum;
}

#endif
