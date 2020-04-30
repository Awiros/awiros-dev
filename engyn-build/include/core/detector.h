#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include "blob.h"
#include "frame.h"

class awiros_detector_config{
  public:
    afloat_t                 conf;
    awiros_dims              min_size;
    awiros_dims              max_size;
    afloat_t                 sharp;
    afloat_t                 fuzz;
    cv::Size                 erode;
    cv::Size                 dilate;
    anum_t		     min_blob_area;
    anum_t		     max_blob_area;						
    awiros_detector_config();
    awiros_detector_config(
        afloat_t,
        awiros_dims,
        awiros_dims,
        afloat_t,
        afloat_t);
};

/* Detector interface */
class awiros_detector{


  public:

    anum_t m_shared_count = 0;

    virtual void init(const awiros_detector_config&) = 0;
    virtual void detect(const awiros_frame&, std::vector<awiros_blob>&) = 0;
};

#endif // _DETECTOR_H_
