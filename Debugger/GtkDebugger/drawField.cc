#include "gtkdebugger.ih"

void GtkDebugger::drawField(Cairo::RefPtr<Cairo::Context> context)
{
  // Green background
  context->set_source_rgb(0.0, 0.5, 0.0);
  context->paint();
  context->set_source_rgb(1.0, 1.0, 1.0);
  context->set_line_width(.1);

  // Field border
  context->rectangle(-9, -6, 18, 12);
  context->stroke();

  // Penalty areas
  context->move_to(-9, 3);
  context->line_to(-7.2, 3);
  context->line_to(-7.2, -3);
  context->line_to(-9, -3);
  context->stroke();
  
  context->move_to(9, 3);
  context->line_to(7.2, 3);
  context->line_to(7.2, -3);
  context->line_to(9, -3);
  context->stroke();

  // Center lines
  context->move_to(0, -6);
  context->line_to(0, 6);
  context->stroke();
  
  // Middle circle
  context->arc(0, 0, 1.3, 0, 2 * M_PI);
  context->stroke();
}

