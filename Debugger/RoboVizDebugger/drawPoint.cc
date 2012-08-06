#include "robovizdebugger.ih"

void RoboVizDebugger::drawPoint(float x, float y, float z, float size, float r, float g, float b, const string* setName) {
  float center[3] = {x,y,z};
  float color[3] = {r,g,b};

  int bufSize = -1;
  unsigned char* buf = newPoint(center, size, color, setName, &bufSize);
  sendto(d_sockfd, buf, bufSize, 0, d_p->ai_addr, d_p->ai_addrlen);
  delete[] buf;
}
