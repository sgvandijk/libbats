#include "robovizdebugger.ih"

void RoboVizDebugger::drawCircle(float x, float y, float radius, float thickness, float r, float g, float b, const string* setName) {
  float center[2] = {x,y};
  float color[3] = {r,g,b};
  
  assert(d_sockfd);
  assert(d_p);

  int bufSize = -1;
  unsigned char* buf = newCircle(center, radius, thickness, color, setName, &bufSize);
  sendto(d_sockfd, buf, bufSize, 0, d_p->ai_addr, d_p->ai_addrlen);
  delete[] buf;
}
