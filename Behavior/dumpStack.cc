#include "behavior.ih"

void Behavior::dumpStack()
{
  for (list<rf<Behavior> >::iterator iter = s_behStack.begin(); iter != s_behStack.end(); ++iter)
    cout << (*iter)->getName() << "::" << (*iter)->ident() << endl;
}
