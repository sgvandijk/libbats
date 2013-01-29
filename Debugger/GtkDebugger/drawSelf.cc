#include "gtkdebugger.ih"

void GtkDebugger::drawSelf(Cairo::RefPtr<Cairo::Context> context)
{
  context->set_line_width(.02);
  
  Localizer& loc = SLocalizer::getInstance();
  
  Vector3d l = loc.getMe()->getPositionGlobal();
  Matrix3d cv = loc.getMe()->getPositionDistributionGlobal()->getSigma();
  double s = sqrt(cv.diagonal().sum() / 3);
  
  context->set_source_rgba(0.0, 0.0, 1.0, 0.5);
  context->arc(l(0), l(1), s * 2, 0, 2 * M_PI);
  context->fill_preserve();
  
  context->set_source_rgba(0.0, 0.0, 1.0, 1.0);
  context->stroke();
  
  context->arc(l(0), l(1), 0.1, 0, 2 * M_PI);
  context->fill();
}
