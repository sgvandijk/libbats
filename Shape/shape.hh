#ifndef __BATS_SHAPE_HH_
#define __BATS_SHAPE_HH_

#include <Eigen/Core>
#include <list>
#include "../RefAble/refable.hh"
#include "../Ref/rf.hh"
#include "../Types/types.hh"
#include "../Localizer/localizer.hh"

namespace bats
{
  struct Shape : public RefAble
  {
    Shape()
    : color(0,0,0,1), strokeWidth(0.01)
    {}
    
    Shape(Eigen::Vector4d const& color, std::string const& category)
    : color(color), category(category), strokeWidth(0.01)
    {}
    
    virtual ~Shape() {}
    
    Eigen::Vector4d color;
    std::string category;
    double strokeWidth;
    
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
  
  /** A line consisting of a start point and a vector relative to that position. */
  struct Line : public Shape
  {
    Line(Eigen::Vector3d const& pos, Eigen::Vector3d const& dir, float thickness = 2)
    : pos(pos), dir(dir), thickness(thickness)
    {}

    Line(Eigen::Vector3d const& pos, Eigen::Vector3d const& dir, Eigen::Vector4d const& color, std::string const& category, float thickness = 2)
    : Shape(color, category), pos(pos), dir(dir), thickness(thickness)
    {}
    
    Eigen::Vector3d pos;
    Eigen::Vector3d dir;
    float thickness;
  };

  /** A line consisting of a start and end point, both as absolute vectors in global coordinates. */
  struct LineSegment : public Shape
  {
    LineSegment()
    : start(0,0,0), end(0,0,0), thickness(0)
    {}
    
    LineSegment(Eigen::Vector3d const& start, Eigen::Vector3d const& end, float thickness = 2)
    : start(start), end(end), thickness(thickness)
    {}
    
    LineSegment(Eigen::Vector3d const& start, Eigen::Vector3d const& end, Eigen::Vector4d const& color, std::string const& category, float thickness = 2)
    : Shape(color, category), start(start), end(end), thickness(thickness)
    {}
    
    Eigen::Vector3d start;
    Eigen::Vector3d end;
    float thickness;
  };
  
  struct Circle : public Shape
  {
    Circle(Eigen::Vector3d const& center, double radius, float thickness = 2)
    : center(center), radius(radius), thickness(thickness)
    {}

    Circle(Eigen::Vector3d const& center, double radius, Eigen::Vector4d const& color, std::string const& category, float thickness = 2)
    : Shape(color, category), center(center), radius(radius), thickness(thickness)
    {}

    Eigen::Vector3d center;
    double radius;
    float thickness;
  };
  
  struct Sphere : public Shape
  {
    Sphere(Eigen::Vector3d const& center, double radius)
    : center(center), radius(radius)
    {}

    Sphere(Eigen::Vector3d const& center, double radius, Eigen::Vector4d const& color, std::string const& category)
    : Shape(color, category), center(center), radius(radius)
    {}

    Eigen::Vector3d center;
    double radius;
  };
  
  struct Point : public Shape
  {
    Point(Eigen::Vector3d const& center, double size, float thickness = 2)
    : center(center), size(size), thickness(thickness)
    {}

    Point(Eigen::Vector3d const& center, double size, Eigen::Vector4d const& color, std::string const& category, float thickness = 2)
    : Shape(color, category), center(center), size(size), thickness(thickness)
    {}

    Eigen::Vector3d center;
    double size;
    float thickness;
  };
  
  struct Rectangle : public Shape
  {
    Rectangle(Eigen::Vector3d const& origin, Eigen::Vector3d const& size, float thickness = 2)
    : origin(origin), size(size), thickness(thickness)
    {}

    Rectangle(Eigen::Vector3d const& origin, Eigen::Vector3d const& size, Eigen::Vector4d const& color, std::string const& category, float thickness = 2)
    : Shape(color, category), origin(origin), size(size), thickness(thickness)
    {}
    
    Eigen::Vector3d origin;
    Eigen::Vector3d size;
    float thickness;
  };
  
  struct Polygon : public Shape
  {
    Polygon() {}

    Polygon(std::list<Eigen::Vector3d> vertices)
    : vertices(vertices)
    {}

    Polygon(std::list<Eigen::Vector3d> vertices, Eigen::Vector4d const& color, std::string const& category)
    : Shape(color, category), vertices(vertices)
    {}

    Polygon(Eigen::Vector4d const& color, std::string const& category)
    : Shape(color, category)
    {}
    
    std::list<Eigen::Vector3d> vertices;
  };
  
  struct FilledCircle : public Polygon
  {
    FilledCircle(Eigen::Vector3d const& center, double radius, Eigen::Vector4d const& color, std::string const& category, unsigned steps = 12)
    : Polygon()
    {
      std::list<Eigen::Vector3d> vertices = std::list<Eigen::Vector3d>();
      double stepSize = M_PI / (steps / 2);
      for (double theta = 0; theta < M_PI * 2; theta += stepSize)
      {
        double x = center.x() + cos(theta) * radius;
        double y = center.y() + sin(theta) * radius;
        vertices.push_back(Eigen::Vector3d(x, y, 0));
      }
      this->color = color;
      this->category = category;
      this->vertices = vertices;
    }
  };
  
  struct Annotation : public Shape
  {
    Annotation(Eigen::Vector3d const& position, std::string const& text)
    : pos(position), text(text)
    {}
  
    Annotation(Eigen::Vector3d const& position, std::string const& text, Eigen::Vector4d const& color, std::string const& category)
    : Shape(color, category), pos(position), text(text)
    {}

    Annotation(Eigen::Vector3d const& position, double const& val, Eigen::Vector4d const& color, std::string const& category)
    : Shape(color, category), pos(position)
    {
      std::ostringstream str;
      str << val;
      text = str.str();
    }
    
    Eigen::Vector3d pos;
    std::string text;
  };
  
  struct AgentAnnotation : public Shape
  {
    // NOTE that agent annotations do not have categories assigned to them
    
    AgentAnnotation(std::string const& text, unsigned unum, Types::Side side)
    : text(text), unum(unum), side(side)
    {}

    AgentAnnotation(std::string const& text, unsigned unum, Types::Side side, Eigen::Vector4d const& color)
    : Shape(color, "AgentAnnotation"), text(text), unum(unum), side(side)
    {}

    std::string text;
    unsigned unum;
    Types::Side side;
  };
  
  /** A series of lines tracing the observed position of a player's body. */
  struct PlayerSkeleton : public Shape
  {
    PlayerSkeleton(rf<bats::PlayerInfo> const& playerInfo, Eigen::Vector4d const& color, std::string const& category, float thickness = 2)
    : Shape(color, category), playerInfo(playerInfo), thickness(thickness)
    {}
    
    rf<bats::PlayerInfo> playerInfo;
    float thickness;
  };
  
  /** A set of lines depicting the translation and rotation of a 3D transformation matrix. */
  struct TransformationAxes : public Shape
  {
    TransformationAxes(Eigen::Matrix4d const& matrix, float axisLength, std::string const& category, float thickness = 2)
    : Shape(Eigen::Vector4d(0,0,0,0), category), matrix(matrix), thickness(thickness), axisLength(axisLength)
    {}
    
    Eigen::Matrix4d matrix;
    float thickness;
    float axisLength;
  };
  
  struct ProgressCircle : public Shape
  {
    ProgressCircle(Eigen::Vector3d const& center, double radius, double progress, Eigen::Vector4d color, std::string const& category)
    : Shape(color, category), center(center), radius(radius), progress(progress)
    {}
    
    Eigen::Vector3d center;
    double radius;
    double progress;
  };
}

#endif // __BATS_SHAPE_HH_
