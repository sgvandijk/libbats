#include "behavior.ih"

ConfidenceInterval Behavior::capable(bool const high) const
{
      if(high)
        return ConfidenceInterval(d_standard_capability_high, 0.2);
      return ConfidenceInterval(d_standard_capability_low, 0.2);
}
