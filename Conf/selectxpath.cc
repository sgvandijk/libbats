#include "conf.ih"

XMLNodeSet Conf::selectXPath(std::string const &expr) const
{
  return XMLNodeSet(xmlXPathEvalExpression(reinterpret_cast<xmlChar const *>(expr.c_str()), xpathCtx));
}
