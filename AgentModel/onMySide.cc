#include "agentmodel.ih"

bool AgentModel::onMySide() const
{
    Localizer& loc = SLocalizer::getInstance();

    Affine3d trans = loc.getGlobalTransformation();

    return abs(Math::getRight(trans).z()) > 0.90;
}

