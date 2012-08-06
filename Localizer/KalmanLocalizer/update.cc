#include "kalmanlocalizer.ih"
#include "../../Debugger/debugger.hh"

void KalmanLocalizer::update()
{
  Cochlea& cochlea = bats::SCochlea::getInstance();
  d_haveNewVisionData = cochlea.getTimestamp(Cochlea::iVision) == bats::SClock::getInstance().getTime();
  
  //
  // Copy vision data into raw (unfiltered) fields on ObjectInfo instances.
  //
  updateRaw();
  
  //
  // Integrate gyro measurement data into the global rotation matrix and
  // adjust for any drift in the rotation via comparison with vision data.
  //
  updateGlobalRotation();

  //
  // Update all objects in gobal space
  //
  updateSelfGlobal();

  updateBallGlobal();

  updatePlayersGlobal();

  //
  // Populate local frame versions of object fields
  //
  globalToLocal();
  
  Debugger& dbg = bats::SDebugger::getInstance();
  if (dbg.isEnabled() && dbg.isVerbose())
  {
    AgentModel& am = bats::SAgentModel::getInstance();
    Localizer& loc = bats::SLocalizer::getInstance();
    auto cameraToGlobalTransform = loc.getGlobalTransformation() * am.getBodyPart(Types::HEAD)->transform;
    //
    // Draw the lines that the agent can see.
    //
//     int index = 0;
    for (VisibleLine line : cochlea.getLines())
    {
      Vector3d lineStartCamera = line.end1Cartesian;
      Vector3d lineEndCamera   = line.end2Cartesian;
      Vector3d lineStartGlobal = cameraToGlobalTransform * lineStartCamera;
      Vector3d lineEndGlobal   = cameraToGlobalTransform * lineEndCamera;

      dbg.draw(new LineSegment(lineStartGlobal, lineEndGlobal, Vector4d(1,1,0,1), "Lines.3D"));
      // redraw with z == 0
      lineStartGlobal.z() = lineEndGlobal.z() = 0;
      dbg.draw(new LineSegment(lineStartGlobal, lineEndGlobal, Vector4d(0,0,0,.5), "Lines.2D"));
//       // draw the line length as an annotation at the line's midpoint
//       auto midPoint = (lineStartGlobal + lineEndGlobal) / 2;
//       std::stringstream length;
//       length << "(" << index << ") " << line.length;
//       dbg.draw(new Annotation(midPoint, length.str(), Vector4d(1,1,0,.5), "Lines.Length"));
//       // draw coloured dots at either end of the line, indicating end1/end2
//       dbg.draw(new Point(lineStartGlobal, 5, Vector4d(1,0,0,.5), "Lines.EndPoints"));
//       dbg.draw(new Point(lineEndGlobal, 5, Vector4d(0,0,1,.5), "Lines.EndPoints"));
//       index++;
    }
  }
}
