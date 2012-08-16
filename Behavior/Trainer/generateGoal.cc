#include "trainer.ih"

shared_ptr<Goal> Trainer::generateGoal(unsigned step, unsigned slot)
{
  shared_ptr<Goal> goal = make_shared<Goal>();
  shared_ptr<OrNode> dis = goal->addDisjunct();
  shared_ptr<AndNode> con = dis->addConjunct();

  // con->addVar("Var", 0, 1);
  
  return goal;
}
