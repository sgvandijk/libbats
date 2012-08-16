#include "gtkdebugger.ih"

void GtkDebugger::drawPlayers(Cairo::RefPtr<Cairo::Context> context)
{
  Localizer& loc = SLocalizer::getInstance();
  
  for (shared_ptr<PlayerInfo> player : loc.players)
  {
    if (!player->isAlive)
      continue;
    
    Vector3d l = player->getPositionGlobal();
    Matrix3d cv = player->posVelGlobal->getSigma().block<3,3>(0,0);
    double s = sqrt(cv.diagonal().sum() / 3);
    
    // Draw circle where player is
    if (player->isTeamMate)
      context->set_source_rgba(1.0, 1.0, 0.0, 0.5);
    else
      context->set_source_rgba(1.0, 0.0, 1.0, 0.5);
    context->arc(l(0), l(1), s * 2, 0, 2 * M_PI);
    context->fill_preserve();
    if (player->isTeamMate)
      context->set_source_rgba(1.0, 1.0, 0.0, 1.0);
    else
      context->set_source_rgba(1.0, 0.0, 1.0, 1.0);
    context->stroke();
    context->arc(l(0), l(1), 0.1, 0, 2 * M_PI);
    context->fill();
    
    // Draw arms and feet
    /*
    context->set_source_rgba(.0,.0,.0,1);
    context->arc(l(0) + player->locLArmGlobal(0), l(1) + player->locLArmGlobal(1), 0.04, 0, 2 * M_PI);
    context->fill();
    context->arc(l(0) + player->locRArmGlobal(0), l(1) + player->locRArmGlobal(1), 0.04, 0, 2 * M_PI);
    context->fill();
    context->arc(l(0) + player->locLFootGlobal(0), l(1) + player->locLFootGlobal(1), 0.04, 0, 2 * M_PI);
    context->fill();
    context->arc(l(0) + player->locRFootGlobal(0), l(1) + player->locRFootGlobal(1), 0.04, 0, 2 * M_PI);
    context->fill();
    */
  }
}
