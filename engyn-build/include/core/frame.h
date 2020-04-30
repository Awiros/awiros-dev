#ifndef _FRAME_H_
#define _FRAME_H_

#define WATERMARK_PADDING 10


#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "utils.h"
#include "types.h"
//#include "src/logos.h"

class awiros_frame {

  anum_t    num;
  anum_t    cycles;

  acoord_t       rows;
  acoord_t       cols;
  std::size_t      channels;

  double    frows;
  double    fcols;

  cv::Mat   soframe;
  cv::Mat   smask;
  cv::Mat   saccum;
  cv::Mat   sprev;
  cv::Mat   shsv;
  cv::Mat   shue;
  cv::Mat   dataOverlay;
  cv::Mat   sdisplay;
  cv::Mat   featureMat;

  cv::Ptr<cv::BackgroundSubtractor> bg;
  cv::Mat erodeKernel;
  cv::Mat dilateKernel;


  int       erode_h;
  int       erode_v;
  int       dilate_h;
  int       dilate_v;
  int       blur_s;
  int       bg_thresh;
  int       shadow_thresh;
  int       bg_hist;
  int       min_obj_size;

  public:

    cv::Mat   frame;
    cv::Mat   sframe;
    cv::Mat   oframe;
    cv::Mat   mask;
    int       dheight;
    int       dwidth;
  
    //TODO:: Setting the frame severity
    awiros_severity      frame_severity = LOW;
    awiros_frame();
    //awiros_frame(const awiros_frame&);
    awiros_frame(cv::Mat mat);
    awiros_frame(awiros_frame &in_aframe, cv::Rect roi);


    double    bg_learn_rate = 0.005; // track_aband_tamp
    int       bg_threshold = 120;
    void init();
    void preprocess_frame();
    acoord_t get_rows();
    acoord_t get_cols();
    std::size_t get_channels();
    cv::Mat get_gray_scale_frame();
    cv::Mat get_hsv_frame();
    //cv::Mat get_rgb_frame(); //TODO: Need to know input frame format
    void get_result_bgMOG2();
    void get_bgmask();
    void get_bgmask_with_canny(cv::Mat);
    void get_erosion();
    void get_dilation();
    void set_mask(cv::Mat mask);
    //cv::Mat get_normalized_frame(); //TODO: supply normalization method
    std::string get_base64_frame();

    void split_merge(awiros_frame &in_aframe);
    void run_bgMOG2();
    void convertTo(awiros_frame &in_aframe, int method);
    void threshold(int thresh, int maxval, int method = cv::THRESH_BINARY);
    void resize_frame(cv::Size size, int method);
    void set_frame(cv::Mat frame);
    cv::Mat get_frame();
    void set_sframe(cv::Mat &sframe);
    cv::Mat get_sframe();
    void init_bg(cv::Ptr<cv::BackgroundSubtractor> &this_bg);
    void init_erode(cv::Mat &erodeElement);
    void init_dilate(cv::Mat &dilateElement);
    anum_t get_cycles();
    void set_cycles(anum_t nframes);
    bool get_working_status();
    void get_result_bgMOG2(cv::Mat &smask);
    void add_frame(awiros_frame in_aframe);
    void sub_frame(awiros_frame in_aframe);
    void multiply_scalar_to_frame(float scalar);
    double get_region_mean(std::vector<cv::Point> region_points);
    void add_watermark(unsigned char *logo_png, unsigned int logo_png_len);
    void merge_mask(int offset = 10);
    void calculate_featureMat();
    void equalize_intensity();
    void get_blank_frame();
    void crop(int size_x, int size_y);
    void rotate(double);
    void annotate();

    std::string server_heat;

};


#endif // _FRAME_H_
