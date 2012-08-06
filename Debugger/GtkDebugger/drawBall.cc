#include "gtkdebugger.ih"

void GtkDebugger::drawBall(Cairo::RefPtr<Cairo::Context> context)
{
  Localizer& loc = SLocalizer::getInstance();
  
  Vector3d l = loc.ball->getPositionGlobal();
  Matrix3d cv = loc.ball->getPositionDistributionGlobal()->getSigma();
  double s = sqrt(cv.diagonal().sum() / 3);
  
  context->set_source_rgba(1.0, 0.5, 0.0, 0.5);
  context->arc(l(0), l(1), s * 2, 0, 2 * M_PI);
  context->fill_preserve();
  
  context->set_source_rgba(1.0, 0.5, 0.0, 1.0);
  context->stroke();
  
  context->arc(l(0), l(1), 0.1, 0, 2 * M_PI);
  context->fill();
}