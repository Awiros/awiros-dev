#ifndef _DARKNET_H_
#define _DARKNET_H_

#ifdef ENABLE_DARKNET
#include "../../../core/detector.h"
#include "../../../ext/darknet/include/darknet.h"
#include <mutex>
#include <condition_variable>
#include "../../../core/resource_manage.h"


class awiros_darknet: public awiros_detector, public awiros_resource{
  float nms=.45;
  list* options;
  char* name_list;
  char** names;
  network* net;
  layer l;
  float hier_conf = 0.1;
  mutable std::mutex m; //TODO-SID: make it clean routeine
  mutable std::condition_variable cv;

  public:
    //void init(const awiros_detector_config&) override;
  float conf = 0.8;
    void init(const awiros_detector_config&) override;
    void init(const awiros_detector_config&, char*, char*, char*);
    void detect(const awiros_frame&, std::vector<awiros_blob>&) override;
    void collect_detections(image, detection*, int, std::vector<awiros_blob>&);
    void free_resources();
    ~awiros_darknet();
};

class awiros_darknet_classifier: public awiros_resource {
  float nms=.45;
  list* options;
  char* name_list;
  char** names;
  network* net;
  int* indexes;
  layer l;
  float conf = 0.8;
  float hier_conf = 0.1;
  image** alphabet;

  public:
    void init();
    void classify(awiros_blob&);
};

//extern awiros_darknet darknet;
//extern awiros_darknet_classifier darknet_classifier;
//extern awiros_darknet darknet_helmet_detection;
#endif // ENABLE_DARKNET

#endif // _DARKNET_H_
