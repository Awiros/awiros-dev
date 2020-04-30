#ifndef _CASCADE_H_
#define _CASCADE_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "blob.h"
#include <vector>
#include <string>
#include "detector.h"
#include "resource_manage.h"

class awiros_cascade: public awiros_detector, public awiros_resource {
  private:
    double    m_scale = 1.2;
    int       m_neighbors = 8; //TODO: get from config
    int       m_flags = 0;
    cv::Size  m_min_size=cv::Size(10,10); //TODO: get from config
    cv::Size  m_max_size=cv::Size(30,30); //TODO: get from config
    // int       detect_var=0;

  public:

    awiros_cascade(){};
    
    awiros_cascade(std::string filename);
    awiros_cascade(std::string filename, double scale, int nghbrs, int flags);

    void init(const awiros_detector_config& detector_config);
   
    cv::CascadeClassifier clfr;

    void set_cascade(std::string filename);
    

    void detect(const awiros_frame& aframe, std::vector<awiros_blob> &blobs); 
};
/*
class awiros_classifier {
  private:



  public:
    awiros_classifier(){};




};

*/
#endif //_CASCADE_H_
