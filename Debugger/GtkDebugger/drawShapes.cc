#include "gtkdebugger.ih"

void GtkDebugger::drawShapes(Cairo::RefPtr<Cairo::Context> context)
{
  for (vector<rf<Shape> >::iterator iter = d_shapes.begin(); iter != d_shapes.end(); ++iter)
  {
    rf<Shape> shape = *iter;
    context->set_source_rgba(shape->color(0), shape->color(1), shape->color(2), shape->color(3));
    context->set_line_width(shape->strokeWidth);
    
    if (rf<LineSegment> ls = rf_dynamic_cast<LineSegment>(shape))
    {
      context->move_to(ls->start(0), ls->start(1));
      context->line_to(ls->end(0), ls->end(1));
      context->stroke();
    }
    else if (rf<Circle> c = rf_dynamic_cast<Circle>(shape))
    {
      context->arc(c->center(0), c->center(1), c->radius, 0, 2 * M_PI);
      context->stroke();
    }
    else if (rf<Rectangle> r = rf_dynamic_cast<Rectangle>(shape))
    {
      context->rectangle(r->origin(0), r->origin(1), r->size(0), r->size(1));
      context->stroke();
    }
    else if (rf<Polygon> p = rf_dynamic_cast<Polygon>(shape))
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
