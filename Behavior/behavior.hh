/*
 *  Little Green BATS (2007), AI department, University of Groningen
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
 *  Date: 	July 27, 2007
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

#ifndef _BATS_BEHAVIOR_HH_
#define _BATS_BEHAVIOR_HH_

#include "../Ref/rf.hh"
#include "../RefAble/refable.hh"
#include "../AST/ast.hh"
#include "../Predicate/predicate.hh"
#include "../Action/action.hh"
#include "../Conf/conf.hh"
#include "../BehaviorFactory/behaviorfactory.hh"
#include "../ConfidenceInterval/confidenceinterval.hh"
#include "../AST/StateNode/statenode.hh"
#include "../AST/BehaviorNode/behaviornode.hh"
#include "../BatsEvent/batsevent.hh"

#include <ostream>
#include <vector>
#include <set>
#include <algorithm>
#include <utility>
#include <cmath>
#include <map>
#include <sigc++/sigc++.h>
#include <list>

#define MIN_C 0.5

namespace bats
{
  /** Behavior class
   *
   * This is the Behavior class, the building block of the BATS
   * agents. It supplies the basic architecture to set up a hierarchy
   * of behaviors. A behavior can be given a goal to achieve. New
   * behaviors should inherit from this class and implement the
   * following methods:
   *
   * <dl>
   *   <dt>rf<Goal> generateGoal(unsigned step, unsigned slot)</dt>
   *
   *   <dd>This method is called to generate a subgoal for the
   *   subbehaviors in a slot. The goal is a State description tree
   *   containing nodes of type andType and varType.</dd>
   *
   *   <dt>rf<State> getCurrentState()</dt>
   *
   *   <dd>For different behaviors different descriptions of the
   *   current state are important. Implement this method to let the
   *   behavior create it's own description. This description will be
   *   passed to getCapability.</dd>
   *
   *   <dt>ConfidenceInterval getCapability(State s, Goal g)</dt>
   *
   *   <dd>With this method the behavior informs the superbehavior of
   *   the capability of achieving a certain goal starting from a
   *   certain state.</dd>
   *
   * </dl>
   */
  class Behavior : public RefAble
  {
  protected:
    /// The behavior's name
    std::string d_name;

    /// The behavior's id  
    std::string d_id;

    /** Tree describing sub behavior semantics
     *
     *  This tree is built up as follows:
     *  - Root node of type sequenceType
     *  - Sequence steps: the behavior tries to do these steps in the given sequence. These nodes have type andType, because nodes underneath these can be run together
     *  - Slots: every sequence step has 1 or more slots. These nodes are of type orType, because only one behavior in a slot can be picked (exclusive or)
     *  - Behaviors: in every slot there can be 1 or more behaviors. These nodes are of type behaviorType
     */
    rf<AST::Node> d_tree;
    
    /// Current step index of the sequence
    int d_curStep;
    
    /// Time at which current step is started
    double d_curStepStart;
    
    /// The goal we are trying to achieve
    rf<Goal> d_goal;
  
    /// Whether the behavior was reset before the current getCapability call, true between achieveGoal and reset, false otherwise.
    bool d_reset;
    
    /// The player class index (defined in the configuration)
    std::string d_behaviorTree;
    
    /// Confidence interval showing the expected capability of achieving the goal
    ConfidenceInterval d_c;
  
    /// Action this behavior wants to perform. This is only used by primitive behaviors
    rf<Action> d_action;
    
    /// Whether the behavior should commit (defined in the configuration)
    bool d_commit;
    
    /// Whether the behavior is committed
    bool d_committed;
    
    /// Whether the last ran children are committed
    bool d_childrenCommitted;
    
    /// 'Should Commit If Children Commit'
    bool d_scicc;
    
    /// Server time of the last time update is ran
    double d_lastUpdate;

    /// List of subbehaviors that this behavior chose last time step
    std::vector<rf<Behavior> > d_lastChosen;
    
    /// Standard low capability
    double d_standard_capability_low;

    /// Standard high capability
    double d_standard_capability_high;

    /** Return the confidence interval for the standard capability
      The default low capability is 0
      The default high capability is 0.5
    
      @high Return the high capability when true, the low capability otherwise.
      @return ConfidenceInterval(standard_capability, 0.2)
    */
    ConfidenceInterval capable(bool const high = true) const;
    
    /// Called before calling generateGoal for each slot
    virtual void preGenerateGoal(unsigned step) {};
    
    /** Generate the current goal for a slot
     *
     * @param step Step number in the sequence
     * @param slot Slot number in the step
     */
    virtual rf<Goal> generateGoal(unsigned step, unsigned slot)
    {
      // The default state is just this simple empty structure.
      // Subclasses may override as needed.
      rf<Goal> goal = new Goal();
      rf<OrNode> dis = goal->addDisjunct();
      rf<AndNode> con = dis->addConjunct();
      return goal;
    };
    
    /// Get the current state. This state is passed to getCapability
    virtual rf<State> getCurrentState()
    {
      // The default state is just this simple empty structure.
      // Subclasses may override as needed.
      rf<State> state = new State();
      rf<OrNode> dis = state->addDisjunct();
      rf<AndNode> con = dis->addConjunct();
      return state;
    };
    
    /// Get the capability of the behavior to achieve goal g from state s
    virtual ConfidenceInterval getCapability(rf<State> s, rf<Goal> g)
    {
      // By default, behaviors are capable. Subclasses should override
      // if custom logic is required.
      return capable(true);
    };
    
    /// Try doing the next step in the sequence
    bool doNextStep();
    
    /// Try doing the current step in the sequence
    bool doCurrentStep();
    
    /// Perform the subbehaviors that this behavior chose last time step again
    bool doLastChosen();
    
    /// Recursively generate an empty configuration template
    void generateConf(std::ostream& out, rf<AST::Node> node, unsigned& index);

    /// Add this behavior to the list of action-command behaviors, signalling that this behavior wants to perform an action
    void addToActionCommandBehaviors() { s_actionCommandBehaviors.insert(this); }
    
    /// Remove this behavior from the list of action-command behaviors
    void removeFromActionCommandBehaviors() { s_actionCommandBehaviors.erase(this); }
    
    /// Global map containing all the agent's behaviors
    static std::map<std::string, rf<Behavior> > s_behaviors;
    
    /// The root behavior of the agent's hierarchical behavior structure
    static rf<Behavior> s_win;
    
    /// list of behaviors that want to perform an action
    static std::set<rf<Behavior> > s_actionCommandBehaviors;

    /// Stack of currently running agents
    static std::list<rf<Behavior> > s_behStack;

    /** Get a parameter from the XML configuration
     *
     * This method is mostly used in it's template based overloaded version where the getParam function
     * returns the given parameter in the correct data type.
     * @param name The xpath of the parameter relative to this behavior's node
     * @returns the node of the requested paramter or 0 when the parameter isn' t found
     */
    XMLNode getParam(std::string const &name) const { XMLNodeSet ns(getParams(name)); if(ns) return ns.front(); return 0; }
    
    /** Get a parameter of a specific type from the XML configuration
     *
     * @param xpath The xpath of the parameter relative to this behavior's node
     * @param def Default value when the parameter is noet defined in the configuration
     * @returns the node of the requested paramter or @a def when the parameter isn' t found
     */
    template <typename T> T getParam(std::string const &xpath, T def) const
    {
      XMLNodeSet ns(getParams(xpath));
      if(ns && !ns.empty())
      {
        std::istringstream s(ns.front().getContent());
        s >> def;
      }
      return def;
    };
    
    //XMLNode getParam() const { XMLNodeSet ns(getParams("")); if(ns) return ns.front(); return 0; }
    
    /** Get the behavior's parameters from the XML configuration
     *
     * @ returns the configuration parameters (<params> block) list which match the xpath
     *   from the conf class using the id and playerclass this behavior instance in is part of.
     */
    XMLNodeSet getParams(std::string const &path) const
    {
      Conf& conf = SConf::getInstance();
      return conf.getParams(d_behaviorTree, d_id, path);
    }
    
  public:
    Behavior(std::string const &name, std::string const &id);
    Behavior(std::string const &name, std::string const &id, std::string behaviorTree);
    virtual ~Behavior() {}
    
    /** Get the behavior's name
     *
     * @returns the behavior's type name
     */
    std::string getName() { return d_name; }
    
    /** Get the behavior's ID
     *
     * @returns the behavior's ID
     */
    std::string getId() { return d_id; }
    
    /** Set the agent's agent class (defined by the configuration) */
    void setBehaviorTree(std::string const& tree)
    {
      d_behaviorTree = tree;
    }
	
    /** Add a behavior to one of the behavior's slots
     *
     * @param behavior The behavior to add
     * @param step Step number of the slot in the sequence
     * @param slot Slot number in the step
     */
    void addToSlot(rf<Behavior> behavior, unsigned step, unsigned slot);

	  /// Called when the full behavior tree is constructed
    virtual void constructed() {};

    /// Called at the beginning of a timestep in which the behavior might be run to update it's internal state
    virtual void update();
    
    /// Set goal, returns the capability of achieving it from the current state
    ConfidenceInterval setGoal(rf<Goal> g);
        
    /// Get the capability of this behavior of achieving the last set goal
    ConfidenceInterval getCapability() const { return d_c; }
    
    /** Try to achieve the goal previously set
     *
     * First tries to go to the next step in the sequence. Generates goals for the slots in the next step and checks the capability of the behaviors in the slots of achieving those goals.
     * If this fails, the current slot is tried.
     * \todo This method is virtual so primitive behaviors can overload it, perhaps main loop should check the type instead?
     * @returns False if neither the next nor the current step in the sequence can be done, true otherwise
     */
    virtual bool achieveGoal();
    
    /// Get the action command that this behavior wants to send to the server
    rf<Action> getAction() const { return d_action; }
    
    /// Generate an empty configuration template describing the slots that this behavior has
    void generateConf(std::ostream& out);
    
    /// Reset the behavior to his initial state. Called when behavior is not (longer) chosen by its superbehavior
    virtual void reset();
    
    /// Tell this behavior whether he should commit when possible
    void shouldCommit(bool c) { d_commit = c; }
    
    /// Check whether this behavior should commit when possible
    bool shouldCommit() { return d_commit; }
    
    /// Tell this behavior whether he should commit when its last chosen children are committed
    void shouldCommitIfChildrenCommitted( bool c) { d_scicc = c; }
    
    /// Check whether this behavior should commit when its last chosen children are committed
    bool shouldCommitIfChildrenCommitted() { return d_scicc; }
    
    /// Check whether the behavior is committed (should be ignored if shouldCommit returns false)
    bool isCommitted() { return d_committed; }
    
    
    
    /// Identify yourself
    std::string ident() { return d_id; }
    
    /// Get last server time at which update was called
    double getLastUpdate() { return d_lastUpdate; }
    
    /// Get the set of behaviors that want to perform action commands
    static std::set<rf<Behavior> > getActionCommandBehaviors() { return s_actionCommandBehaviors; }
    
    /// Clear the set of behaviors that want to perform action commands
    static void clearActionCommandBehaviors() { s_actionCommandBehaviors.clear(); }
    
    /// Dump the current stack of running behaviors to stdout
    static void dumpStack();
    
    /// A signal that can be used to listen to events thrown by the behavior
    static sigc::signal<void, rf<BehaviorEvent> > behavior_signal;
    
    /// A signal that can be used to get notified of successfull runs of achieveGoal
    static sigc::signal<void, rf<AchieveGoalSuccessEvent> > achieveGoal_success_signal;
  };
  
  /// A simple class used to sort BehaviorNodes by descending capability
  class CapabilityCompare
  {
    public:
      bool operator()(rf<AST::Node> a, rf<AST::Node> b) 
      {
        ConfidenceInterval c1 = rf_cast<BehaviorNode>(a)->getBehavior()->getCapability();
        ConfidenceInterval c2 = rf_cast<BehaviorNode>(b)->getBehavior()->getCapability();
        
        c1.round();
        c2.round();
        
        return c1.first > c2.first;
        return c1.first + c1.second > c2.first + c2.second;
      }
  };

  typedef rf<Behavior> RBehavior;
          
};

#endif
