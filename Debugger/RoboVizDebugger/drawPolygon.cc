#include "robovizdebugger.ih"

void RoboVizDebugger::drawPolygon(std::list<Eigen::Vector3d> const& vertices, Eigen::Vector4d const& color, const std::string* setName) {
  int bufSize = -1;
  unsigned char* buf = newPolygon(vertices, color, setName, &bufSize);
  sendto(d_sockfd, buf, bufSize, 0, d_p->ai_addr, d_p->ai_addrlen);
  delete[] buf;
}
