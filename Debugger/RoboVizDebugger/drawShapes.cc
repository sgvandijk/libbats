#include "robovizdebugger.ih"

bool removeShapeUntilIfExpired(RoboVizDebugger::ShapeUntil& shapeUntil)
{
  double time = SWorldModel::getInstance().getTime();
  bool expired = shapeUntil.time < time;
  return expired;
}

void RoboVizDebugger::drawShapes()
{
  // draw all one-shot shapes
  for (vector<shared_ptr<Shape> >::iterator iter = d_shapes.begin(); iter != d_shapes.end(); ++iter)
    drawShape(*iter);
  d_shapes.clear();
  
  // draw any sticky shapes
  d_shapesUntil.remove_if(removeShapeUntilIfExpired);
  for (list<ShapeUntil>::iterator iter = d_shapesUntil.begin(); iter != d_shapesUntil.end(); ++iter)
    drawShape((*iter).shape);
}

void RoboVizDebugger::drawShape(shared_ptr<Shape> shape)
{
  string setName = getSetName(shape);
  
  WorldModel& wm = bats::SWorldModel::getInstance();  
  
  /*
  context->set_source_rgba(shape->color(0), shape->color(1), shape->color(2), shape->color(3));
  context->set_line_width(shape->strokeWidth);
  */
  
  if (shared_ptr<Line> l = dynamic_pointer_cast<Line>(shape))
  {
    Vector3d start = wm.checkFlipSide(l->pos);
    Vector3d end = wm.checkFlipSide(l->pos + l->dir);
    
    drawLine(start, end, l->thickness, l->color, &setName);
  }
  else if (shared_ptr<LineSegment> ls = dynamic_pointer_cast<LineSegment>(shape))
  {
    Vector3d start = wm.checkFlipSide(ls->start);
    Vector3d end = wm.checkFlipSide(ls->end);
    
    drawLine(start, end, ls->thickness, ls->color, &setName);
  }
  else if (shared_ptr<Circle> c = dynamic_pointer_cast<Circle>(shape))
  {
    Vector3d centre = wm.checkFlipSide(c->center);
    
    drawCircle(centre.x(), centre.y(), c->radius, c->thickness, c->color(0), c->color(1), c->color(2), &setName);
  }
  else if (shared_ptr<Rectangle> r = dynamic_pointer_cast<Rectangle>(shape))
  {
    Vector3d v1 = r->origin;
    Vector3d v3 = r->origin + r->size;
    Vector3d v2 = Vector3d(v3.x(), v1.y(), 0);
    Vector3d v4 = Vector3d(v1.x(), v3.y(), 0);
    
    v1 = wm.checkFlipSide(v1);
    v2 = wm.checkFlipSide(v2);
    v3 = wm.checkFlipSide(v3);
    v4 = wm.checkFlipSide(v4);

    drawLine(v1, v2, r->thickness, r->color, &setName);
    drawLine(v2, v3, r->thickness, r->color, &setName);
    drawLine(v3, v4, r->thickness, r->color, &setName);
    drawLine(v4, v1, r->thickness, r->color, &setName);
  }
  else if (shared_ptr<Polygon> p = dynamic_pointer_cast<Polygon>(shape))
  {
    for (list<Vector3d>::iterator iter = p->vertices.begin(); iter != p->vertices.end(); ++iter)
      (*iter) = wm.checkFlipSide(*iter);
      
    drawPolygon(p->vertices, p->color, &setName);
  }
  else if (shared_ptr<Annotation> a = dynamic_pointer_cast<Annotation>(shape))
  {
    Vector3d pos = wm.checkFlipSide(a->pos);
    drawAnnotation(&a->text, pos.x(), pos.y(), pos.z(), a->color(0), a->color(1), a->color(2), &setName);
  }
  else if (shared_ptr<AgentAnnotation> a = dynamic_pointer_cast<AgentAnnotation>(shape))
  {
    drawAgentAnnotation(&a->text, a->unum, a->side, a->color(0), a->color(1), a->color(2));
  }
  else if (shared_ptr<PlayerSkeleton> s = dynamic_pointer_cast<PlayerSkeleton>(shape))
  {
    drawPlayerSkeleton(s->playerInfo, s->thickness, s->color, &setName);
  }
  else if (shared_ptr<TransformationAxes> t = dynamic_pointer_cast<TransformationAxes>(shape))
  {
    drawTransformationAxes(t->matrix, t->thickness, t->axisLength, &setName);
  }
  else if (shared_ptr<ProgressCircle> p = dynamic_pointer_cast<ProgressCircle>(shape))
  {
    Vector3d center = wm.checkFlipSide(p->center);
    drawProgressCircle(p->center, p->radius, p->progress, p->color, &setName);
  }
  else if (shared_ptr<Sphere> s = dynamic_pointer_cast<Sphere>(shape))
  {
    Vector3d center = wm.checkFlipSide(s->center);    
    drawSphere(center.x(), center.y(), center.z(), s->radius, s->color(0), s->color(1), s->color(2), &setName);
  }
  else if (shared_ptr<Point> p = dynamic_pointer_cast<Point>(shape))
  {
    Vector3d center = wm.checkFlipSide(p->center);    
    drawPoint(center.x(), center.y(), p->center.z(), p->size, p->color(0), p->color(1), p->color(2), &setName);
  }
  else
  {
    cerr << "Unsupported shape type with category: " << shape->category << endl;
  }
}
