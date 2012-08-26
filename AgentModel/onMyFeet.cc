#include "agentmodel.ih"

bool AgentModel::onMyFeet() const
{
    //_debugLevel4(d_fieldNormal << " " << d_gyro << " " << d_footForces[0] << " " << d_footForces[1]);

    Localizer& loc = SLocalizer::getInstance();

    Affine3d trans = loc.getGlobalTransformation();

    return Math::getUp(trans).z() > 0.90 &&
            (d_footForces[0].z() > 0 ||
            d_footForces[1].z() > 0);   // if you are walking you dont need to have forces on both your feet, but you are still on your feet
}
