#include "ast.ih"

shared_ptr<AST::Node> AST::Node::findDeep(string const &query) const
{
  for (NodeVector::const_iterator i = d_nodes.begin(); i != d_nodes.end(); ++i) {
    if ((*i)->match(query)) {
      return *i;
    } else {
      shared_ptr<AST::Node> p = (*i)->findDeep(query);
      if (p)
        return p;
    }
  }

  // Return 0 if there is no match.
  return 0; //make_shared<AST::Node>(0);
}
