#ifndef __BATS_ROBOVIZDEBUGGER_HH__
#define __BATS_ROBOVIZDEBUGGER_HH__

#include "../debugger.hh"
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../../Socket/socket.hh"
#include "../../SocketAddress/socketaddress.hh"
#include "../../Types/types.hh"
#include "../../WorldModel/worldmodel.hh"

namespace bats
{
  class RoboVizDebugger : public Debugger
  {
    friend class Singleton<Debugger>;

    public:
      virtual void init(int argc, char** argv);
      virtual void start();

      virtual void reDraw();

      virtual void debug(std::string const& dbg) {}
      
      virtual void plot(std::string const& curve, float value) {}
      
      virtual void draw(rf<Shape> shape) { d_shapes.push_back(shape); }
      
      virtual void drawForPeriod(rf<Shape> shape, double durationSeconds) {
        double time = SWorldModel::getInstance().getTime();
        drawUntil(shape, time + durationSeconds);
      }
      
      virtual void drawUntil(rf<Shape> shape, double time) { 
        ShapeUntil shapeUntil = ShapeUntil();
        shapeUntil.shape = shape;
        shapeUntil.time = time;
        d_shapesUntil.push_back(shapeUntil);
      }

      virtual bool isEnabled() { return d_isStarted; }
      virtual bool isVerbose() { return d_isVerbose; }
      virtual void setVerbose(bool isVerbose) { d_isVerbose = isVerbose; }

      struct ShapeUntil { public: rf<Shape> shape; double time; };

      void drawSelf();
      void drawBall();
      void drawPlayers();

    private:
      std::string d_robovizHost;
      std::string d_robovizPort;
      bool d_isStarted;
      bool d_isVerbose;
            
      int d_sockfd;
      struct addrinfo* d_p;

      std::vector<rf<Shape> > d_shapes;
      std::list<ShapeUntil> d_shapesUntil;

      RoboVizDebugger ();

      const std::string getSetName(rf<Shape> shape);
      const std::string getSetName(const std::string suffix);
      
      int writeCharToBuf(unsigned char* buf, unsigned char value) const;
      int writeFloatToBuf(unsigned char* buf, float value) const;
      int writeColorToBuf(unsigned char* buf, const float* rgb, int channels) const;
      int writeStringToBuf(unsigned char* buf, const std::string* text) const;

      unsigned char* newBufferSwap(const std::string* name, int* bufSize);

      unsigned char* newCircle(const float* center, float radius, float thickness, const float* color, const std::string* setName, int* bufSize);
      unsigned char* newLine(const float* a, const float* b, float thickness, const float* color, const std::string* setName, int* bufSize);
      unsigned char* newPoint(const float* p, float size, const float* color, const std::string* setName, int* bufSize);
      unsigned char* newSphere(const float* p, float radius, const float* color, const std::string* setName, int* bufSize);
      unsigned char* newPolygon(const std::list<Eigen::Vector3d>& vertices, const Eigen::Vector4d& color, const std::string* setName, int* bufSize);
      unsigned char* newAnnotation(const std::string* text, const float* p, const float* color, const std::string* setName, int* bufSize);
      unsigned char* newAgentAnnotation(const std::string* text, unsigned int unum, Types::Side side, const float* color, int* bufSize);

      void swapBuffers(const std::string* setName);

      void drawCircle(float x, float y, float radius, float thickness, float r, float g, float b, const std::string* setName);
      void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float thickness, float r, float g, float b, const std::string* setName);
      void drawLine(Eigen::Vector3d const& p1, Eigen::Vector3d const& p2, float thickness, Eigen::Vector4d const& color, std::string const* setName)
      { drawLine(p1.x(), p1.y(), p1.z(), p2.x(), p2.y(), p2.z(), thickness, color(0), color(1), color(2), setName); }
      void drawPoint(float x, float y, float z, float size, float r, float g, float b, const std::string* setName);
      void drawSphere(float x, float y, float z, float radius, float r, float g, float b, const std::string* setName);
      void drawPolygon(std::list<Eigen::Vector3d> const& vertices, Eigen::Vector4d const& color, const std::string* setName);
      void drawAnnotation(const std::string* text, float x, float y, float z, float r, float g, float b, const std::string* setName);
      void drawAgentAnnotation(const std::string* text, unsigned unum, Types::Side side, float r, float g, float b);
      void drawPlayerSkeleton(rf<bats::PlayerInfo> const info, float thickness, Eigen::Vector4d const& color, const std::string* setName);
      void drawTransformationAxes(Eigen::Matrix4d const& matrix, float thickness, float axisLength, const std::string* setName);
      void drawProgressCircle(Eigen::Vector3d const& center, double radius, double progress, Eigen::Vector4d color, const std::string* setName);

      void onThinkEnd();
      void drawShapes();
      void drawShape(rf<Shape> shape);
      //bool removeShapeUntilIfExpired(ShapeUntil& shapeUntil);
  };

  inline int RoboVizDebugger::writeCharToBuf(unsigned char* buf, unsigned char value) const {
    *buf = value;
    return 1;
  }

  inline int RoboVizDebugger::writeFloatToBuf(unsigned char* buf, float value) const {
    char temp[20];
    sprintf(temp, "%6f", value);
    memcpy(buf, temp, 6);
    return 6;
  }

  inline int RoboVizDebugger::writeColorToBuf(unsigned char* buf, const float* color, int channels) const {
    int i;
    for (i = 0; i < channels; i++)
      writeCharToBuf(buf+i, (unsigned char)(color[i]*255)); 
    return i;
  }

  inline int RoboVizDebugger::writeStringToBuf(unsigned char* buf, const std::string* text) const {
    long i = 0;
    if (text != NULL)
      i += text->copy((char*)buf+i, text->length(), 0); 
    i += writeCharToBuf(buf+i, 0);
    return i;
  }

  inline unsigned char* RoboVizDebugger::newBufferSwap(const std::string* name, int* bufSize) {
    *bufSize = 3 + ((name != NULL) ? name->length() : 0);
    unsigned char* buf = new unsigned char[*bufSize];

    long i = 0;
    i += writeCharToBuf(buf+i, 0);
    i += writeCharToBuf(buf+i, 0);
    i += writeStringToBuf(buf+i, name);

    return buf;
  }

  inline unsigned char* RoboVizDebugger::newCircle(const float* center, float radius, float thickness, const float* color, const std::string* setName, int* bufSize) {
    *bufSize = 30 + ((setName != NULL) ? setName->length() : 0);
    unsigned char* buf = new unsigned char[*bufSize];
    
    long i = 0;
    i += writeCharToBuf(buf+i, 1);
    i += writeCharToBuf(buf+i, 0);
    i += writeFloatToBuf(buf+i, center[0]);
    i += writeFloatToBuf(buf+i, center[1]);
    i += writeFloatToBuf(buf+i, radius);
    i += writeFloatToBuf(buf+i, thickness);
    i += writeColorToBuf(buf+i, color, 3);
    i += writeStringToBuf(buf+i, setName);

    return buf;
  }

  inline unsigned char* RoboVizDebugger::newLine(const float* a, const float* b, float thickness, const float* color, const std::string* setName, int* bufSize) {
    *bufSize = 48 + ((setName != NULL) ? setName->length() : 0);
    unsigned char* buf = new unsigned char[*bufSize];

    long i = 0;
    i += writeCharToBuf(buf+i, 1);
    i += writeCharToBuf(buf+i, 1);
    i += writeFloatToBuf(buf+i, a[0]);
    i += writeFloatToBuf(buf+i, a[1]);
    i += writeFloatToBuf(buf+i, a[2]);
    i += writeFloatToBuf(buf+i, b[0]);
    i += writeFloatToBuf(buf+i, b[1]);
    i += writeFloatToBuf(buf+i, b[2]);
    i += writeFloatToBuf(buf+i, thickness);
    i += writeColorToBuf(buf+i, color, 3);
    i += writeStringToBuf(buf+i, setName);
    
    return buf;
  }

  inline unsigned char* RoboVizDebugger::newPoint(const float* p, float size, const float* color, const std::string* setName, int* bufSize) {
    *bufSize = 30 + ((setName != NULL) ? setName->length() : 0);
    unsigned char* buf = new unsigned char[*bufSize];

    long i = 0;
    i += writeCharToBuf(buf+i, 1);
    i += writeCharToBuf(buf+i, 2);
    i += writeFloatToBuf(buf+i, p[0]);
    i += writeFloatToBuf(buf+i, p[1]);
    i += writeFloatToBuf(buf+i, p[2]);
    i += writeFloatToBuf(buf+i, size);
    i += writeColorToBuf(buf+i, color, 3);
    i += writeStringToBuf(buf+i, setName);
    
    return buf;
  }

  inline unsigned char* RoboVizDebugger::newSphere(const float* p, float radius, const float* color, const std::string* setName, int* bufSize) {
    *bufSize = 30 + ((setName != NULL) ? setName->length() : 0);
    unsigned char* buf = new unsigned char[*bufSize];

    long i = 0;
    i += writeCharToBuf(buf+i, 1);
    i += writeCharToBuf(buf+i, 3);
    i += writeFloatToBuf(buf+i, p[0]);
    i += writeFloatToBuf(buf+i, p[1]);
    i += writeFloatToBuf(buf+i, p[2]);
    i += writeFloatToBuf(buf+i, radius);
    i += writeColorToBuf(buf+i, color, 3);
    i += writeStringToBuf(buf+i, setName);
    
    return buf;
  }

  inline unsigned char* RoboVizDebugger::newPolygon(const std::list<Eigen::Vector3d>& vertices, const Eigen::Vector4d& color, const std::string* setName, int* bufSize) {
    int numVerts = vertices.size();
    *bufSize = 18 * numVerts + 8 + ((setName != NULL) ? setName->length() : 0);
    unsigned char* buf = new unsigned char[*bufSize];
    float colorf[] = {static_cast<float>(color(0)),
		      static_cast<float>(color(1)),
		      static_cast<float>(color(2)),
		      static_cast<float>(color(3))}; // including alpha

    long i = 0;
    i += writeCharToBuf(buf+i, 1);
    i += writeCharToBuf(buf+i, 4);
    i += writeCharToBuf(buf+i, numVerts);
    i += writeColorToBuf(buf+i, colorf, 4);

    for (std::list<Eigen::Vector3d>::const_iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
    {
      Eigen::Vector3d v = *iter;
      i += writeFloatToBuf(buf+i, v.x()); // implicit conversion from double to float
      i += writeFloatToBuf(buf+i, v.y());
      i += writeFloatToBuf(buf+i, v.z());
    }

    i += writeStringToBuf(buf+i, setName);

    return buf;
  }

  inline unsigned char* RoboVizDebugger::newAnnotation(const std::string* text, const float* p, const float* color, const std::string* setName, int* bufSize) {
    *bufSize = 25 + text->length() + setName->length();
    unsigned char* buf = new unsigned char[*bufSize];

    long i = 0;
    i += writeCharToBuf(buf+i, 2);
    i += writeCharToBuf(buf+i, 0);
    i += writeFloatToBuf(buf+i, p[0]);
    i += writeFloatToBuf(buf+i, p[1]);
    i += writeFloatToBuf(buf+i, p[2]);
    i += writeColorToBuf(buf+i, color, 3);
    i += writeStringToBuf(buf+i, text);
    i += writeStringToBuf(buf+i, setName);

    return buf;
  }

  inline unsigned char* RoboVizDebugger::newAgentAnnotation(const std::string* text, unsigned int unum, Types::Side side, const float* color, int* bufSize) {
    *bufSize = (text == NULL) ? 3 : 7 + text->length();
    unsigned char* buf = new unsigned char[*bufSize];

    if (unum < 1 || unum > bats::SWorldModel::getInstance().getNumberOfPlayers())
      std::cerr << "Attempt to send agent annotation for invalid unum: " << unum << std::endl;
    
    long i = 0;
    i += writeCharToBuf(buf+i, 2);
    
    if (text == NULL) {
      i += writeCharToBuf(buf+i, 2);
      i += writeCharToBuf(buf+i, (side==Types::LEFT) ? unum - 1 : unum + 127);
    } else {
      i += writeCharToBuf(buf+i, 1);
      i += writeCharToBuf(buf+i, (side==Types::LEFT) ? unum - 1 : unum + 127);
      i += writeColorToBuf(buf+i, color, 3);
      i += writeStringToBuf(buf+i, text);
    }

    return buf;
  }

  inline void RoboVizDebugger::swapBuffers(const std::string* setName) {
    int bufSize = -1;
    unsigned char* buf = newBufferSwap(setName, &bufSize);
    sendto(d_sockfd, buf, bufSize, 0, d_p->ai_addr, d_p->ai_addrlen);
    delete[] buf;
  }
};


#endif /* __BATS_ROBOVIZDEBUGGER_HH__ */
