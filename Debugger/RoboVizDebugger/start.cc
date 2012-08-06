#include "robovizdebugger.ih"

// #define ROBOVIS_HOST "localhost"
// #define ROBOVIS_PORT "32769"

void RoboVizDebugger::start()
{
  struct addrinfo hints, *servinfo;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(d_robovizHost.c_str(), d_robovizPort.c_str(), &hints, &servinfo)) != 0) {
      cerr << "getaddrinfo:" << gai_strerror(rv) << endl;
      return;
  }

  // loop through all the results and make a socket
  for(d_p = servinfo; d_p != NULL; d_p = d_p->ai_next) {
      if ((d_sockfd = socket(d_p->ai_family, d_p->ai_socktype,
              d_p->ai_protocol)) == -1) {
          perror("socket");
          continue;
      }

      break;
  }

  if (d_p == NULL) {
      cerr << "Failed to bind socket\n" << endl;
      return;
  }
  
  cerr << "RoboVizDebugger connected!" << endl;
  sigc::slot<void> sl = sigc::mem_fun(*this, &RoboVizDebugger::onThinkEnd);
  HumanoidAgent::think_end_signal.connect(sl);
  
  d_isStarted = true;
}

