#include "agentmodel.ih"

bool AgentModel::onMySide() const
{
    Localizer& loc = SLocalizer::getInstance();

    Transform3d trans = loc.getGlobalTransformation();

    return abs(Math::getRight(trans).z()) > 0.90;
}

