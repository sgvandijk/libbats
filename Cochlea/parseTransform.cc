#include "cochlea.ih"
#include "../Math/math.hh"

Transform3d Cochlea::parseTransform(shared_ptr<Predicate> const &pred)
{
  assert(pred);
  
  assert(pred->size() >= 3);
  
  // 3 values for x,y,z translation
  Vector3d trans = Vector3d();
  trans(0) = *static_pointer_cast<Predicate>(pred->getChild(0));
  trans(1) = *static_pointer_cast<Predicate>(pred->getChild(1));
  trans(2) = *static_pointer_cast<Predicate>(pred->getChild(2));

  Vector3d forward = Vector3d(0,0,0);
  Vector3d up = Vector3d(0,0,0);
  Vector3d right = Vector3d(0,0,0);
  
  // if the server supports sending orientation data as well, take it
  if (pred->size() == 12)
  {
    // 3 values for forward vector
    forward(0) = *static_pointer_cast<Predicate>(pred->getChild(3));
    forward(1) = *static_pointer_cast<Predicate>(pred->getChild(4));
    forward(2) = *static_pointer_cast<Predicate>(pred->getChild(5));
    
    // 3 values for right vector
    right(0) = *static_pointer_cast<Predicate>(pred->getChild(6));
    right(1) = *static_pointer_cast<Predicate>(pred->getChild(7));
    right(2) = *static_pointer_cast<Predicate>(pred->getChild(8));
    
    // 3 values for up vector
    up(0) = *static_pointer_cast<Predicate>(pred->getChild(9));
    up(1) = *static_pointer_cast<Predicate>(pred->getChild(10));
    up(2) = *static_pointer_cast<Predicate>(pred->getChild(11));
    
    forward /= 1000;
    right /= 1000;
    up /= 1000;
  }

  // Columns:
  // 0. right
  // 1. forward
  // 2. up
  // 3. translation
  
  // server stores right/up/forward
  //return Math::makeTransform(right, forward, up, trans);
  return Math::makeTransform(right, up, forward, trans);
}

