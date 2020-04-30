#ifndef _BLOB_HPP_
#define _BLOB_HPP_
#ifdef ENABLE_TF
#include "tensorflow/core/framework/tensor.h"
#endif

#include "utils.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "../ext/json/json.hpp"

#include "frame.h"
#include "types.h"
#include "classifiers.h"
#include "../interfaces/frameworks/tensorflow/tf_utils.h"

using namespace nlohmann;

class awiros_point {
  public:
    awiros_point(){};
    awiros_point(acoord_t x1, acoord_t y1);
    awiros_point(cv::Point p1); 
    acoord_t x;
    acoord_t y;

    // scaled
    afloat_t sx;
    afloat_t sy;

    void scale_up(int width, int height);
    cv::Point get_point();

    // adding virtual function
};

// class awiros_bounding_rect : public awiros_point{
  
//   public:

//     std::vector<double> rect_parameter;

//     awiros_bounding_rect(){};

//     double get_height();
//     double get_width();

// }



class awiros_blob_class {

  public:
    std::string                     name;
    aid_t                           id=-1;
    std::string                     label;
    std::string                     subclass;
    std::string        heatmap;
    std::vector<std::string>        dburl;
    std::vector<std::string>        dburl_web;
    std::vector<awiros_frame>       dbimg;

    awiros_severity                 severity;
    double                          conf;

    //int age;
    std::string age;
    std::string gender;
    bool is_fake;
    double fake_print_conf;
    double fake_replay_conf;
    json                            features;

    //std::string emotion;
    std::string             feature_class = "";

    awiros_blob_class();
    awiros_blob_class(std::string, double);
    void get_dbimg_from_url();
};



class awiros_blob_base{
    public:
      std::string color;
      std::string label;
      std::vector<awiros_point> bounding_box;
};

class awiros_blob_train{
    public:

       //TODO-SID: load lables list int system initialization
       std::string               dburl;
       std::string               name;
       std::vector<awiros_blob_base>  blobs;
       //TODO-SID: image load functions for imgurl
       //TODO-SID: export mechanism to other frameworks

       friend std::ostream& operator << (std::ostream& output, const awiros_blob_train* blob_train){
            output << "IMGURL: " << blob_train->dburl << '\n'
                   << "BLOBS" << '\n'
                   << "=====================\n";

            for(auto blob_base = blob_train->blobs.begin(); blob_base != blob_train->blobs.end(); blob_base++){
                output << blob_base->label << ": ";
                for(auto bb = blob_base->bounding_box.begin(); bb != blob_base->bounding_box.end(); bb++){
                        output << '[' << bb->sx << ", " << bb->sy << ']' << " ";
                }
                output << '\n';
            }

       return output;
       }
};

class awiros_blob_config {
  public:

    std::vector<awiros_color> color;
    awiros_severity           severity;
    double                    min_dwell_time_ms;
    bool                      with_classification=false;
    double                    conf_classification=0.20;
    std::map<aid_t, awiros_blob_class> class_db;
    std::vector<awiros_blob_train> train_db;
};

class awiros_blob {
    aid_t                  m_id = 0;
    anum_t                 m_num = 0; //frame-number?

  public:
    awiros_blob();
    awiros_blob(acoord_t tx1, acoord_t ty1, acoord_t width1, acoord_t height1);
    awiros_blob(cv::Rect rect);
    aserial_t               label;
    acoord_t                cx;
    acoord_t                cy;
    acoord_t                tx;
    acoord_t                ty;
    acoord_t                width;
    acoord_t                height;
    //TODO-SID: add support for polygon
    cv::Mat                 mat;
    bool 		            is_assigned = false;

    bool                    is_front_profile = true;

    std::string             feature = "";
    std::string             url; // incase of filesystem
    cv::Mat                 bmat;
    awiros_frame            bframe;
    awiros_blob_class       myclass;

    std::vector<double> facenet_feat;

    char speed[30];
    std::vector<awiros_blob> inner_blobs;
    std::vector<cv::Point> contour;
    aserial_t              generate_awi_id();
    aid_t                  get_id();
    void                   set_id(aid_t id);
    //anum_t                 get_seq();
    anum_t                 get_area();
    cv::Rect               get_cv_rect();
    cv::Rect               get_cv_rect_wrt_oframe(awiros_frame aframe, int padding = 50);

    //cv::Point              get_top_coords();
    void                   print_info();
    void                   save_blob_image();
    void                   draw_on_frame(awiros_frame &aframe, bool with_data = false, int color = 0);

    void                   transform_space(awiros_blob &ref_space);
    void                   get_roi(awiros_frame &aframe, awiros_blob &dest, int offset = 40);
    void                   set_my_class(awiros_blob_class &newclass);
    void                   set_my_mat(awiros_frame aframe);
    void                   set_my_mat(cv::Mat frame, cv::Rect rect);
    void                   set_bframe(awiros_frame aframe);
    void                   get_super_rect(awiros_frame aframe, int level = 1, bool update_rect = true);


    bool operator< (const awiros_blob &right) const{
      return this->cx <right.cx;
    }
#ifdef ENABLE_TF
    tensorflow::Tensor mat_to_tens(cv::Mat, int h, int w ,int c, int mean, int std);
    std::pair<std::string, float> classify(classifier &model);

#endif
};


class awiros_line {
  public:

    bool alert;
    float check_rate;
    aserial_t label;
    aid_t id;
    bool is_hot = false;
    awiros_severity severity;
    std::vector<awiros_timeslot> active_time;
    anum_t influx_count = 0;
    anum_t outflux_count = 0;
    std::vector<awiros_blob> influx_blobs;
    std::vector<awiros_blob> outflux_blobs;

    awiros_point startp;
    awiros_point endp;
    awiros_line(){};
    awiros_line(acoord_t x1, acoord_t y1, acoord_t x2, acoord_t y2);
    awiros_line(awiros_point p1, awiros_point p2);

    double get_slope();
    double get_constant();
    cv::Point  get_midp();
    bool   check_line_intersection(awiros_line &line);
    bool   get_point_parity(awiros_point &point);
    void   scale_up(int width, int height);
    void   draw_event(awiros_frame &aframe, bool with_data = true);
    bool   check_on_segment(awiros_point p, awiros_point q, awiros_point r);
    awiros_orient check_orientation(awiros_point &point);

};


class awiros_region {

  private:
    cv::Rect bound_rect;

  public:

    bool alert;
    bool is_active = false;
    float check_rate;
    double standard_mean;
    aid_t r_tick;
    aid_t id;
    std::string label;
    bool is_hot =  false;
    awiros_severity severity;
    std::vector<awiros_timeslot> active_time;
    anum_t influx_count = 0;
    anum_t outflux_count = 0;
    std::vector<awiros_blob> influx_blobs;
    std::vector<awiros_blob> outflux_blobs;
    std::vector<cv::Point> region_points;
    std::vector<awiros_line> lines;

    //awiros_region();
    bool   check_region_intersection(awiros_line &line);
    bool   get_point_parity(awiros_point &point);
    void   scale_up(int width, int height);
    void   draw_event(awiros_frame &aframe, bool with_data = true);

    cv::Rect get_bounding_rect();
    void set_bounding_rect(acoord_t minx, acoord_t miny, acoord_t maxx, acoord_t maxy);

    awiros_region   get_test_region();
};

#endif
