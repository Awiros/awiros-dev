#ifndef _SIMPLE_CAMERA_H_
#define _SIMPLE_CAMERA_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/face.hpp>

#include <vector>
//#include "core/frame.h"
//#include "core/blob.h"
//#include "core/motion.h"

//#include "core/app.h"
#include "engyn.h"

class awiros_simple_camera : public awiros_app {
  //faces data
  //face reco resnet 
  private:
  public:
    awiros_simple_camera();
    awiros_simple_camera(awiros_app &app);
    void run();
    void send_event();
    // void set_face_detector(std::string filename);
   // void set_profile_detector(std::string filename);

    //std::vector<> match_faces(std::vector<awiros_blob> &blobvec);
    //std::vector<double> get_face_score(awiros_blob blob, awiros_frame frame);
      //score
      //dbid
};

extern "C" awiros_app* app_class(){
      return new awiros_simple_camera;
}

#endif// _SIMPLE_CAMERA_H_

