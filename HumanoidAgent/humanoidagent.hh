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
#ifndef _BATS_HUMANOIDAGENT_HH_
#define _BATS_HUMANOIDAGENT_HH_

#include "../SocketComm/AgentSocketComm/agentsocketcomm.hh"
#include "../WorldModel/worldmodel.hh"
#include "../Cochlea/cochlea.hh"
#include "../PlayerClass/playerclass.hh"
#include <sigc++/sigc++.h>

namespace bats
{
  /** Humanoid agent base class
   *
   * This class forms the base of your humanoid 3D soccer simulation
   * agent. All you need to do to create a working agent is derive
   * from this class. For instance:
   *
   * \code
   * class MyAgent : public bats::HumanoidAgent
   * {
   *   public:
   *     MyAgent(std::string teamName) : bats::HumanoidAgent(teamName) {}
   * };
   * \endcode
   *
   * To start your agent, simply create an object of your agent class
   * and call its run method:
   *
   * \code
   * int main()
   * {
   *   MyAgent agent("MyTeam");
   *   agent.run();
   * }
   * \endcode
   *
   * The HumanoidAgent class handles all the communication with the
   * server and parsing of input by using SocketComm and
   * WorldModel. You can implement the behavior for your agent by
   * overloading the think method. If your agent needs initialization
   * code that should be run once at start up, overload the init
   * method.
   */
  class HumanoidAgent
  {
  public:
    HumanoidAgent(std::string teamName, std::string confFile = "",
                  std::string const host = "localhost", unsigned port = 3100,
                  unsigned unum = 0)
      : d_teamName(teamName),
        d_confFile(confFile),
        d_host(host),
        d_port(port),
        d_unum(unum),
        d_minThinkTime(5000)
    {
    }
    
    virtual ~HumanoidAgent() {}

    /** Update before think
     *
     * Called after all modules are updated, but before joint angle
     * prediction is done.
     */
    virtual void update() {}

    /** Run the agent
     *
     * Start an infinite loop that updates the communication with the
     * server and the WorldModel and calls think() at every step.
     */
    void run();
    
    static void die() { s_die = true; }
    
    static sigc::signal<void> think_start_signal;
    static sigc::signal<void> think_end_signal;
    
  protected:
    std::string d_teamName;
    std::string d_confFile;
    std::string d_host;
    unsigned d_port;
    unsigned d_unum;
    useconds_t d_minThinkTime;
    
    /// Kill agent next cycle
    static bool s_die;
    
    /** Startup agent
     *
     * Performs the communication with the server necesary to start up
     * the agent
     */
    void startUp();
    
    /** Initialize agent
     *
     * This method is run when the agent is started up. At this point
     * the uniform number and all object properties are known
     */
    virtual void init() {}
    
    /** Perform think cycle
     */
    virtual void think() {}
    
  };
  
};

#endif
