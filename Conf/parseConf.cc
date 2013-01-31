#include "conf.ih"
#include <stdexcept>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xinclude.h>
#include <sys/stat.h>
#include <sys/types.h>

void Conf::parseConf(std::string const &confFile)
{
  if (d_doc)
    xmlFreeDoc(d_doc);

  xmlResetLastError();

  //The following is based on the implementation of xmlParseFile(), in xmlSAXParseFileWithData():
  auto context = xmlCreateFileParserCtxt(confFile.c_str());

  if(!context)
  {
    throw runtime_error("Could not create parser context");
  }

  if(context->directory == 0)
  {
    context->directory = xmlParserGetDirectory(confFile.c_str());
  }

  int options = context->options;
  options |= (XML_PARSE_NOENT | XML_PARSE_DTDLOAD | XML_PARSE_NOBLANKS);

  xmlCtxtUseOptions(context, options);

  //defineEntities(context->myDoc);

  const int parseError = xmlParseDocument(context);

  d_doc = context->myDoc;

  int subs = xmlXIncludeProcessFlags(d_doc, options);

  if (subs < 0)
    throw runtime_error("error processing XInclude on config file: "+confFile);
  else
    _debugLevel4("number of XInclude substitutions: " << subs);

}
