#include "robovizdebugger.ih"

void RoboVizDebugger::drawAgentAnnotation(const string* text, unsigned unum, bats::Types::Side side, float r, float g, float b)
{
  float color[3] = {r,g,b};
  
  int bufSize = -1;
  unsigned char* buf = newAgentAnnotation(text, unum, side, color, &bufSize);
  sendto(d_sockfd, buf, bufSize, 0, d_p->ai_addr, d_p->ai_addrlen);
  delete[] buf;
}

