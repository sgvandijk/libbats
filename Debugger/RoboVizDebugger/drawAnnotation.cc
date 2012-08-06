#include "robovizdebugger.ih"

void RoboVizDebugger::drawAnnotation(const string* text, float x, float y, float z, float r, float g, float b, const string* setName)
{
  float color[3] = {r,g,b};
  float pos[3] = {x,y,z};

  int bufSize = -1;
  unsigned char* buf = newAnnotation(text, pos, color, setName, &bufSize);
  sendto(d_sockfd, buf, bufSize, 0, d_p->ai_addr, d_p->ai_addrlen);
  delete[] buf;
}

