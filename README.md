Libbats
=======

*(2008-2013), AI department, University of Groningen*


**Authors:** 
* Sander van Dijk (sgdijk@gmail.com)
* Drew Noakes (drew@drewnoakes.com)
* Martin Klomp (martin@ai.rug.nl)
* Mart van de Sanden (vdsanden@ai.rug.nl)
* A. Bram Neijt (bneijt@gmail.com)
* Matthijs Platje (mplatje@gmail.com)
 
**Website:**

https://github.com/sgvandijk/libbats
 
**Comment:**

Please feel free to contact us if you have any 
problems or questions about the code.
 	
 
**License:**

    This program is free software; you can redistribute 
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
 
**Dependencies:**

- Eigen 3
- libxml-2.0
- sigc++-2.0
- gtkmm-2.4 (optional; for GTK+ debugger)
- Doxygen (optional; for HTML documentation)
- pdflatex (optional; for PDF manual)

***Ubuntu:***

    sudo apt-get install libeigen3-dev libxml2-dev libsigc++-2.0-dev [libgtkmm-2.4-dev] [doxygen] [texlive-latex-base]
    
**Install:**

Run:

    cmake . && make && sudo make install

**Documentation:**

If you have `pdflatex` installed while running `cmake`, you
can find the user manual at `docs/manual/libbatsmanual.pdf`. If
you have `doxygen` installed while running `cmake`, HTML based
documentation can be found in `docs/html`.

 * * *
 
Happy hacking,

  *The libbats team*
