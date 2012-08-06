#include "kalmanlocalizer.ih"

void KalmanLocalizer::init()
{
  sigc::slot<void, rf<BeamEvent> > sl = sigc::mem_fun(*this, &KalmanLocalizer::onBeam);
  SAgentSocketComm::getInstance().beam_signal.connect(sl);
}

