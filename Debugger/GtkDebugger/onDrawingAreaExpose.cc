#include "gtkdebugger.ih"

void bats::onDrawingAreaExpose(GtkObject *object, gpointer user_data)
{
  SDebugger::getInstance().reDraw();  
}

