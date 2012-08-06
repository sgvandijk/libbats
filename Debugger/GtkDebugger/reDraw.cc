#include "gtkdebugger.ih"

void GtkDebugger::reDraw()
{
  Cairo::RefPtr<Cairo::Context> myContext = d_drawingArea->get_window()->create_cairo_context();
  
  Gtk::Allocation allocation = d_drawingArea->get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
  
  double fieldRatio = 20.0 / 14.0;
  double screenRatio = 1.0 * width / height;
  double scale = screenRatio > fieldRatio ? height / 14 : width / 20;
  
  double visWidth = width / scale;
  double visHeight = height / scale;
  
  myContext->scale(scale, -scale);
  myContext->translate(visWidth / 2, -visHeight / 2);
  
  drawField(myContext);
  
  if (SLocalizer::initialized())
  {
    drawSelf(myContext);
    drawBall(myContext);
    drawPlayers(myContext);
  }
  drawShapes(myContext);
  
  onDbgText();
  
  if (d_curves.size() > d_curvesListStore->children().size())
  {
    d_curvesListStore->clear();
    for (map<string, Curve>::iterator iter = d_curves.begin(); iter != d_curves.end(); ++iter)
    {
      Gtk::ListStore::iterator it = d_curvesListStore->append();
      it->set_value(0, iter->first);
    }
  }
  
  drawCurve();
}

