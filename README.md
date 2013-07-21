
   Libbats (2008), AI department, University of Groningen
 
   Authors: 	Sander van Dijk (sgdijk@gmail.com)
                Martin Klomp (martin@ai.rug.nl)
 		Mart van de Sanden (vdsanden@ai.rug.nl)
 		A. Bram Neijt (bneijt@gmail.com)
 		Matthijs Platje (mplatje@gmail.com)
 
   Date: 	February, 2012
 
   Website:	https://launchpad.net/libbats
 
   Comment:	Please feel free to contact us if you have any 
 		problems or questions about the code.
 
 
   License: 	This program is free software; you can redistribute 
 		it and/or modify it under the terms of the GNU General
 		Public License as published by the Free Software 
 		Foundation; either version 3 of the License, or (at 
 		your option) any later version.
 
    		This program is distributed in the hope that it will
 		be useful, but WITHOUT ANY WARRANTY; without even the
 		implied warranty of MERCHANTABILITY or FITNESS FOR A
 		PARTICULAR PURPOSE.  See the GNU General Public
 		License for more details.
 
    		You should have received a copy of the GNU General
 		Public License along with this program; if not, write
 		to the Free Software Foundation, Inc., 59 Temple Place - 
 		Suite 330, Boston, MA  02111-1307, USA.
 
DEPENDENCIES

        - Eigen 3
        - libxml-2.0
        - sigc++-2.0
        - gtkmm-2.4 (optional)

INSTALL

	Run "cmake && make && make install"

DOCUMENTATION

        If you have pdflatex installed while running configure, you
        can find the user manual at docs/manual/libbatsmanual.pdf. If
        you have doxygen installed while running configure, HTML based
        documentation can be found in docs/html. To generate
        documentation at a later time, run "make" in doc/manual for
        the user manual, and "make doc" or "doxygen" in the main
        directory for the HTML documentation. The latest version of
        the manual can also be found on the libbats project website:
        https://launchpad.net/libbats.


Happy hacking,
  The libbats team
