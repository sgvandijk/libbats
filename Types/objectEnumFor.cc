#include "types.ih"


Types::PlayMode Types::objectEnumFor(std::string const &a, Types::PlayMode def)
{
      for(size_t i = 0; i < Types::NOBJECTS; ++i)
    {
      Types::PlayMode o = static_cast<Types::PlayMode>(i);
      if(a == nameOf(o))
      {
        def = o;
        break;
      }
    }
  return def;
}

Types::Object Types::objectEnumFor(std::string const &a, Types::Object def)
{
      for(size_t i = 0; i < Types::NOBJECTS; ++i)
    {
      Types::Object o = static_cast<Types::Object>(i);
      if(a == nameOf(o))
      {
        def = o;
        break;
      }
    }
  return def;
}
