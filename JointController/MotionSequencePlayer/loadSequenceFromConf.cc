#include "motionsequenceplayer.ih"

void MotionSequencePlayer::loadSequenceFromConf()
{
  // Check whether sequence is symmetric, i.e. left and right are the same
  bool isSymmetric = getConfParam("/symmetric", 0);

  // Get sequence from conf
  string sequenceStr = getConfParamContent("/sequence");

  istringstream seqIn(sequenceStr);

  d_sequence.jointSequences.clear();
  for (unsigned j = 0; j < Types::NJOINTS; ++j)
    d_sequence.jointSequences.push_back(JointSequence());

  unsigned joint;
  // Lines are formatted as:
  // jointnr: t alpha, t alpha, t alpha, ...;
  while (seqIn >> joint)
  {
    // After joint ID should come a colon
    char punc;
    seqIn >> punc;
    if (punc != ':')
    {
        ostringstream out;
        out << "MotionSequencePlayer: unexpected character:" << punc <<
	       ", joint " << joint << ". Expected ':'. Full params:\n" << sequenceStr;
        throw runtime_error(out.str());
    }
    
    // Magix ID for specifying sequence end time
    if (joint == Types::NJOINTS)
    {
      // Read in end time
      seqIn >> d_sequence.length >> punc;

      // Should end with semicolon
      if (punc != ';')
      {
        ostringstream out;
        out << "MotionSequencePlayer: unexpected character:" << punc <<
	       " at end time decleration. Expected ';'. Full params:\n" << sequenceStr;
        throw runtime_error(out.str());
      }
    }
    // Normal joint ID
    else
    {
      // Process key frames. Format: time angle, time angle, ...
      while (punc != ';')
      {
	JointKeyFrame kf;
        seqIn >> kf.t >> kf.angle >> punc;
        
        if (punc != ',' && punc != ';')
        {
          ostringstream out;
          out << "MotionSequencePlayer: unexpected character:" << punc << ", joint " << joint <<
	         ", key frame: " << kf.t << " " << kf.angle <<
	         ". Expected ',' or ';'. Full params:\n" << sequenceStr;
          throw runtime_error(out.str());
        }
        
	// Convert to radians
        kf.angle = Math::degToRad(kf.angle);
        
	// Add keyframe
        d_sequence.jointSequences[joint].push_back(kf);
        
	// If sequence is symmetric, also ass keyframe for joint on other side
        if (isSymmetric)
        {
	  Types::Joint dualJoint;
          double scale = Types::dualOf(dualJoint, (Types::Joint)joint);
          if (dualJoint != Types::NJOINTS)
          {
	    JointKeyFrame dualKf = kf;
	    dualKf.angle = kf.angle * scale;
            d_sequence.jointSequences[dualJoint].push_back(dualKf);
          }
        }
      }
    }
  }

}
