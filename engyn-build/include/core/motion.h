#ifndef _MOTION_HPP_
#define _MOTION_HPP_

#include <opencv2/opencv.hpp>

#include "types.h"
#include "blob.h"
#include "tracker.h"
#include <math.h>

class awiros_motion{
  private:
    void scale_up_lines(int width, int height);
    void scale_up_regions(int width, int height);
    void draw_line_cross(awiros_frame &aframe, bool draw_event_only);

    void draw_region_cross(awiros_frame &aframe, bool draw_event_only);
    void print_lines_data();
    void print_regions_data();

  public:

    std::vector<awiros_line>   lines;
    std::vector<awiros_region> regions;
    bool                       alert_status;

    void draw_number(awiros_frame &aframe, int counter);
    //awiros_motion();
    void init_lines();
    void clear_motion_data();
    void print_line_id();
    void print_region_id();
    //void fill_region_points();
    void init();
    void get_initial_region_mean(awiros_frame &aframe);
    void detect_line_cross(blobhist_pair_t &bit);
    void detect_region_cross(blobhist_pair_t &bit);
    bool process_motion_analysis(blobhist_t &blob_history);
    void scale_up_data(awiros_frame &aframe);
    void draw_motion_events(awiros_frame &aframe, bool draw_event_only = false);
    void print_motion_data();
    void clear_hot_spaces();
};
#endif //_MOTION_HPP_
