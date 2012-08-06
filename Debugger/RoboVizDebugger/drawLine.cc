#include "robovizdebugger.ih"

void RoboVizDebugger::drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float thickness, float r, float g, float b, const string* setName) {
  float pa[3] = {x1,y1,z1};
  float pb[3] = {x2,y2,z2};
  float color[3] = {r,g,b};

  int bufSize = -1;
  unsigned char* buf = newLine(pa, pb, thickness, color, setName, &bufSize);
  sendto(d_sockfd, buf, bufSize, 0, d_p->ai_addr, d_p->ai_addrlen);
  delete[] buf;
}
