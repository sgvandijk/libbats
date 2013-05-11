#ifndef BATS_IKGAITGENERATOR_HH
#define BATS_IKGAITGENERATOR_HH

#include "../gaitgenerator.hh"

namespace bats
{
  /** Inverse Kinematics based gait generator
   *
   * This gait generator uses inverse kinematics to move feet in an
   * elliptical fashion. The following parameters can be set in the
   * configuration file to determine the gait behavior:
   *
   * * ellipseheight - the maximum height of the swing foot w.r.t the ground
   * * ellipselength - the maximum forward/backward step size
   * * sidewidth - the maximum sideway step size
   * * offsetx - the offset of the feet in rest position/halfway a step, along x-axis
   * * offsety - the offset of the feet in rest position/halfway a step, along y-axis
   * * offsetz - the offset of the stance foot w.r.t the hip joint, along y-axis
   * * period - the time it takes to perform a full gait cycle, i.e. a step with each foot
   * * maxturnangle - maximum turn angle per step
   * * leanangle - angle added to hip joint to lean the torso forward
   * * startuptime - time it takes to reach maximum speed when starting from standing
   */
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
    double d_leanAngle;
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
