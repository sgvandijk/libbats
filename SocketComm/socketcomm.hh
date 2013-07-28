/*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors: 	Martin Klomp (martin@ai.rug.nl)
 *		Mart van de Sanden (vdsanden@ai.rug.nl)
 *		Sander van Dijk (sgdijk@ai.rug.nl)
 *		A. Bram Neijt (bneijt@gmail.com)
 *		Matthijs Platje (mplatje@gmail.com)
 *
 *	All students of AI at the University of Groningen
 *  at the time of writing. See: http://www.ai.rug.nl/
 *
 *  Date: 	November 1, 2008
 *
 *  Website:	http://www.littlegreenbats.nl
 *
 *  Comment:	Please feel free to contact us if you have any 
 *		problems or questions about the code.
 *
 *
 *  License: 	This program is free software; you can redistribute 
 *		it and/or modify it under the terms of the GNU General
 *		Public License as published by the Free Software 
 *		Foundation; either version 3 of the License, or (at 
 *		your option) any later version.
 *
 *   		This program is distributed in the hope that it will
 *		be useful, but WITHOUT ANY WARRANTY; without even the
 *		implied warranty of MERCHANTABILITY or FITNESS FOR A
 *		PARTICULAR PURPOSE.  See the GNU General Public
 *		License for more details.
 *
 *   		You should have received a copy of the GNU General
 *		Public License along with this program; if not, write
 *		to the Free Software Foundation, Inc., 59 Temple Place - 
 *		Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef BATS_SOCKETCOMM_HH
#define BATS_SOCKETCOMM_HH

#include <queue>
#include <string>
#include <fstream>
#include <memory>
#include "../TimeVal/timeval.hh"

#define BUFFER_SIZE 64 * 1024

namespace bats {
  class Socket;
  class SocketAddress;
  class PortableParser;
  class Predicate;

  /** Interface between agent and simulation server
   *
   * The SocketComm is used to communicate with the robocup 3D
   * simulation server. It parses and queues predicates recieved from
   * the server and outputs messages to the server. Do not use this
   * class directly, you probably need either AgentSocketComm or
   * TrainerSocketComm
   *
   */
  class SocketComm {

  public:

    /**
     * Default constructor. Call initSocket() before connecting to the server.
     */
    SocketComm();
    
    /**
     *  Initializes SocketComm to use @a host at portnumber @a
     *  port. Call connect() to actually establish the connection.
     *
     *  @param host Host name/IP address of the server, eg "localhost", "192.168.1.1"
     *  @param port Port number of the server, standard number is 3001
     */
    SocketComm(std::string host, int port);
    
    ~SocketComm();
    
    /**
     * Turn on/off parsing of input recieved from the server. Set to
     * false to reduce overhead when the SocketComm is only used for
     * sending messages to the server. Messages are still read from
     * the socket to prevent overflows, but are imediatly discarded.
     */
    void parseInput(bool p);
    
    /**
     * Initialize the socket to use @a host at portnumber @a
     * port. Call connect() to actually establish the connection.
     */
    void initSocket(std::string host, int port);
    
    /**
     * Get description of the address that is being used.
     */
    //SocketAddress getSocketAddress() { return d_socketAddress; }
    
    /**
     * Open a connection to the server
     *
     * @throws runtime_error when connection failed
     */
    void connect();

    /** Directly send a string message to the server
     *
     * @param msg The (string) message to be sent to the server. The
     * message will be length prefixed and sent immediatly.
     */
    void sendMessage(std::string const &msg);
    
    
    /** Queue a predicate to be sent
     *
     * Queues the messages for sending. It will be sent
     * as soon as the update() method is called and the
     * server is ready to recieve.
     * @param pred The message to be sent
     */
    void send(std::shared_ptr<Predicate> pred) { d_oMessageQueue.push(pred); }

    /** Checks whether a new message has arrived from the server
     *
     * @returns true if a new message is queued
     */
    bool hasNextMessage() const
    {
      return !d_iMessageQueue.empty();
    }
    
    /**
     * Gives the next message recieved from the server @returns The
     * first message in the queue
     */
    std::shared_ptr<Predicate> nextMessage();

    /** Receive messages from the server when available */
    void receive();

    /** Send queued messages to the server */
    void send();

    /**
     * Update the communication by first sending queued messages and
     * then reading new available input.  This method blocks until at
     * least one message is read from the server.
     */
    void update()
    {
      send();
      receive();
    }

  protected:
    unsigned char d_buffer[BUFFER_SIZE];

    typedef std::queue<std::shared_ptr<Predicate> > MessageQueue;

    MessageQueue   d_oMessageQueue, d_iMessageQueue;
    std::unique_ptr<PortableParser> d_parser;

    bool d_parseInput;

    TimeVal t0, t1;

    SocketComm(SocketComm const &); // NI
    SocketComm &operator=(SocketComm const &); // NI

    void initConnection();
  
  private:
    std::unique_ptr<Socket> d_socket;
    std::unique_ptr<SocketAddress> d_socketAddress;
  };
}

#endif // BATS_SOCKETCOMM_HH
