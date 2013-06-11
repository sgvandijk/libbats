#include "socketcomm.ih"
#include "../WorldModel/worldmodel.hh"

void SocketComm::receive()
{
  unsigned prefix = 0;

  unsigned totalRead = 0;
  unsigned bufFree = sizeof(d_buffer);

  // Buffer cursor
  unsigned char *buf = d_buffer;

  bool havePrefix = false;
  bool haveFullMsg = false;
  while (!haveFullMsg)
  {
    // Read as much as we can, until buffer is full
    int nRead = d_socket.read(reinterpret_cast<char *>(buf + totalRead),
                                   bufFree);

    // If we don't parse, just return
    if (!d_parseInput)
      return;

    if (nRead > 0)
      // Successful read, mark time of last read
      gettimeofday(t0, 0);
    else if (bufFree != 0)
    {
      // Read failed, and not because buffer is full
      // Check whether server timed out
      gettimeofday(t1,0);

      if ((t1 - t0).getRealSeconds() > 5)
        throw runtime_error("Server seems to have given up");
    }

    totalRead += nRead;
    bufFree -= nRead;

    // Check if we at least have a prefix
    if (totalRead < 4)
      continue;

    if (!havePrefix)
    {
      memcpy(reinterpret_cast<char*>(&prefix), buf, 4);
      prefix = ntohl(prefix);
      havePrefix = true;
    }

    unsigned expectedRead = 4 + prefix;
    // Keep reading if we don't have a full message yet
    if (totalRead < expectedRead)
      continue;

    // Check if we read more than one message
    if (totalRead > expectedRead)
    {
      totalRead -= expectedRead;
      buf += expectedRead;
      havePrefix = false;
      continue;
    }

    // We have one and exactly one message!
    haveFullMsg = true;
  }

  // Parse message
  d_parser.reset();

  unsigned long plen = d_parser.parse(buf + 4, prefix);
  assert(plen == prefix);
  
  d_parser.parse('\n');
  return;
}

