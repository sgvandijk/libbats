#include "gtkdebugger.ih"

void GtkDebugger::drawCurve()
{
  Glib::RefPtr<Gtk::TreeSelection> curvesSelection =
    d_curvesList->get_selection();
  Gtk::TreeModel::iterator iter = curvesSelection->get_selected();
  if (iter)
  {
    Gtk::TreeModel::Row row = *iter;
    Glib::ustring str;
    row.get_value(0, str);
    Curve& c = d_curves[str];
    

    Cairo::RefPtr<Cairo::Context> curveContext = d_curveDrawingArea->get_window()->create_cairo_context();

    Gtk::Allocation allocation = d_curveDrawingArea->get_allocation();
    double width = allocation.get_width();
    double height = allocation.get_height();

    double margin = 10.0;
    
    double graphWidth = width - 2 * margin;
    double graphHeight = height - 2 * margin;
    
    Glib::RefPtr<Gdk::Pixmap> buffer = Gdk::Pixmap::create(d_curveDrawingArea->get_window(), width, height);
    Cairo::RefPtr<Cairo::Context> bufferContext = buffer->create_cairo_context();
    
/*  context->set_source_rgb(1.0, 1.0, 1.0);
  context->set_line_width(.1);

  context->rectangle(-6, -4, 12, 8);
  context->stroke();
  context->move_to(0, -4);
  context->line_to(0, 4);*/

    bufferContext->set_source_rgb(0.75, 0.75, 0.75);
    bufferContext->paint();

    bufferContext->rectangle(margin, margin, width - 2*margin, height - 2*margin);
    bufferContext->set_source_rgb(1.0, 1.0, 1.0);
    bufferContext->fill();

    bufferContext->set_source_rgb(0, 0, 0);
    ostringstream out1;
    out1 << c.maxY;
    bufferContext->move_to(0, margin - 1);
    bufferContext->show_text(out1.str());

    ostringstream out2;
    out2 << c.minY;
    bufferContext->move_to(0, height - 1);
    bufferContext->show_text(out2.str());

    bufferContext->set_line_width(1);

    bufferContext->move_to(margin, height - graphHeight * (0 - c.minY) / (c.maxY - c.minY) - margin);
    bufferContext->line_to(width - margin, height - graphHeight * (0 - c.minY) / (c.maxY - c.minY) - margin);
    bufferContext->set_source_rgb(0.5, 0.5, 0.5);
    bufferContext->stroke();

    bufferContext->move_to(margin, height - graphHeight * (c.data[c.minX] - c.minY) / (c.maxY - c.minY) - margin);
    for (unsigned x = c.minX + 1; x <= c.maxX; ++x)
      bufferContext->line_to(margin + graphWidth * (x - c.minX) / (c.maxX - c.minX),
                            height - graphHeight * (c.data[x] - c.minY) / (c.maxY - c.minY) - margin);

    bufferContext->set_source_rgb(1.0, 0.0, 0.0);

    bufferContext->stroke();
    
    d_curveDrawingArea->get_window()->draw_drawable(d_curveDrawingArea->get_style()->get_fg_gc(d_curveDrawingArea->get_state()),
                                                    buffer, 0, 0, 0, 0);
  }
}

