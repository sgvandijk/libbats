#include "gtkdebugger.ih"

void GtkDebugger::drawShapes(Cairo::RefPtr<Cairo::Context> context)
{
  for (vector<shared_ptr<Shape> >::iterator iter = d_shapes.begin(); iter != d_shapes.end(); ++iter)
  {
    shared_ptr<Shape> shape = *iter;
    context->set_source_rgba(shape->color(0), shape->color(1), shape->color(2), shape->color(3));
    context->set_line_width(shape->strokeWidth);
    
    if (shared_ptr<LineSegment> ls = dynamic_pointer_cast<LineSegment>(shape))
    {
      context->move_to(ls->start(0), ls->start(1));
      context->line_to(ls->end(0), ls->end(1));
      context->stroke();
    }
    else if (shared_ptr<Circle> c = dynamic_pointer_cast<Circle>(shape))
    {
      context->arc(c->center(0), c->center(1), c->radius, 0, 2 * M_PI);
      context->stroke();
    }
    else if (shared_ptr<Rectangle> r = dynamic_pointer_cast<Rectangle>(shape))
    {
      context->rectangle(r->origin(0), r->origin(1), r->size(0), r->size(1));
      context->stroke();
    }
    else if (shared_ptr<Polygon> p = dynamic_pointer_cast<Polygon>(shape))
    {
      list<Eigen::Vector3d>::iterator viter = p->vertices.begin();
      context->move_to((*viter)(0), (*viter)(1));
      ++viter;
      for (; viter != p->vertices.end(); ++viter)
        context->line_to((*viter)(0), (*viter)(1));
      context->stroke();
    }
  }
  d_shapes.clear();
  
}
