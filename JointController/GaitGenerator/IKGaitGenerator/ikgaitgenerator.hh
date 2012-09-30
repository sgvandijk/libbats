#ifndef BATS_IKGAITGENERATOR_HH
#define BATS_IKGAITGENERATOR_HH

#include "../gaitgenerator.hh"

namespace bats
{
  class IKGaitGenerator : public GaitGenerator
  {
  public:
    IKGaitGenerator(std::string const& id);

    virtual void run(JointControlParams* params);

  private:
    struct Step
    {
      double height;
      double width;
      double length;
      double offsetZ;
      double turn;
      double hipAngle;

      Eigen::Vector3d targetDir;
      Eigen::Vector3d targetFaceDir;

      Eigen::Vector3d lStart;
      Eigen::Vector3d lEnd;
      Eigen::Vector3d rStart;
      Eigen::Vector3d rEnd;
    };

    // Gait parameters
    double d_ellipseHeight;
    double d_ellipseLength;
    double d_sideWidth;
    double d_offsetX;
    double d_offsetY;
    double d_offsetZ;
    double d_period;
    double d_maxTurnAngle;

    double d_startupTime;
    double d_lastTime;

    // State variables
    double d_phase;
    double d_speed;
    int d_stage;

    Step d_prevStep;
    Step d_curStep;

    // Fill full body joint angle vector with leg vectors
    void fillJointAngles(Eigen::VectorXd& jointAngles,
			 Eigen::VectorXd const& lLegAngles,
			 Eigen::VectorXd const& rLegAngles);
 
    // Resets everything to startgait from the beginning
    void resetGait();

    // Determine with what speed to walk
    void updateSpeed();

    // Progress gait phase
    void updatePhase();

    // Determine what the next step is to take
    void determineNextStep(Eigen::VectorXd const& targetDir,
			   Eigen::VectorXd const& targetFaceDir);

    // Determine where the swing foot will end at the determined step
    void determineSwingFootEnd();

    // Determine the desired joint angles for this step
    Eigen::VectorXd doStep();

  };
}

#endif
