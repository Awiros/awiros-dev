#ifndef _TRACK_H_
#define _TRACK_H_

#define OBJ_HIST_LENGTH 20
#define MAX_OBJ_ID 8090

//#include "src/hungarian/defines.h"
//#include "src/hungarian/Ctracker.h"
#include "blob.h"
#include "frame.h"
#include "cascade.h"
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "blob.h"
#include "hungarian.h"
#include "ext_api.h"
//#include "mtcnn.h"
#include "kalman.h"


#ifdef ENABLE_DARKNET
#include "../interfaces/frameworks/darknet/dnet.h"
#endif


//latest_timestamp:objid
//typedef std::map<anum_t, std::vector<aid_t>>       tsblob_t;
//int TIME_HISTORY_SIZE = 10;
//typedef std::map<anum_t, aid_t> tsblob_t;
typedef std::pair<const aid_t, std::vector<awiros_blob>>  blobhist_pair_t;
//typedef std::map<aid_t, aid_t> cooling_period_t;
//typedef std::vector<aid_t> cooling_period_t;

class awiros_track_variance{
  private:

    cv::Point sum;
    acoord_t x_sq_sum = 0;
    acoord_t y_sq_sum= 0;

  public:

    cv::Point variance;
    awiros_track_variance(){};
    void update_values(acoord_t cx, acoord_t cy);
    cv::Point2f calculate_variance(size_t n_blobs);

};

class awiros_track{
  /* This is an abstraction for a trajectory of
   * of single awiros_track element*/
  private:

    cv::Point m_prediction_point;
    cv::Rect m_prediction_rect;
    TKalmanFilter* m_kalman;
    bool m_filterObjectSize;
    float m_delta_time = 1.f; // Time step for kalman.
    float  m_noise_magnitude = 0.1f; //noise magnitude for kalman


  public:

    anum_t  skipped_frame = 0;
    anum_t  alert_count = 0;
    anum_t  track_id = 0;

    bool    is_abandoned = true;

    int64   initiated_tick = cv::getTickCount();
    int64   terminated_tick;
    int64   first_tick;
    int64   recheck_tick;
    bool    event_gen = false;

    double speed;
    std::vector<cv::Point> blob_points;

    std::size_t frame_1 = 0;
    std::size_t frame_n = 0;
    awiros_track_variance track_variance;
    //aid_t cooling_period;

    std::vector<awiros_blob> hist;
    std::vector<aid_t> cooling_period = {1,1,1,1,1}; //TODO: make it hashtable

    awiros_track(){};
    awiros_track(awiros_blob);
    cv::Point2f calculate_variance_blob();
    void assign_track(awiros_blob &new_blob);
    void get_woi(blobhist_pair_t &bit, awiros_blob &b_start, awiros_blob &b_end, anum_t window_size = 5);
    void get_woi( awiros_blob &b_start, awiros_blob &b_end, anum_t window_size = 5);
    bool get_woi_parity(awiros_line &ref_line, awiros_orient end_orient, anum_t window_size = 5);
    bool get_woi_parity(awiros_region &ref_region, double end_val, anum_t window_size = 5);
    void print_hist();
    void add_new_blob(awiros_blob &blob);
    float calc_dist(cv::Point &pt);
    void assign_track(awiros_blob &blob, bool data_correct);
    void point_update(cv::Point &pt, bool data_correct);
};

typedef std::map<aid_t, aid_t>         linehist_t;
typedef std::map<aid_t, awiros_track>  blobhist_t;


class awiros_tracker_config{
  public:
    float dist_range;
    anum_t  frames_to_live;
    awiros_tracker_config();
    awiros_tracker_config(float, anum_t);
};




class awiros_tracker {
  private:

    anum_t          m_assigned_id = 0;
    acoord_t  m_dist_range = 30;
    anum_t    m_frames_to_live = 10;
    anum_t    m_tracks_to_live = 125;

    //anum_t          m_min_blob_area = 30; //for vehicle count
    anum_t          m_min_blob_area = 400;

    void trigger_termination_event();

    std::vector<cv::Scalar> track_line_colors =
    {cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0), cv::Scalar(0, 250, 255),
      cv::Scalar(0, 255, 1000), cv::Scalar(0, 255, 255), cv::Scalar(0, 100, 255),
      cv::Scalar(0, 127, 255), cv::Scalar(0,127, 255), cv::Scalar(0, 127, 127)};

  public:


    blobhist_t  blob_hist;
    linehist_t  line_hist;
    std::vector<awiros_blob> curr_blobs;
    std::vector<awiros_blob> prev_blobs;
    //std::vector<awiros_track> tracks;
    std::vector<awiros_track> terminated_tracks;
    std::vector<awiros_track> initiated_tracks;
    std::vector<awiros_track> active_tracks;
    awiros_tracker(){};
    awiros_tracker(const awiros_tracker_config& config);
    void init(const awiros_tracker_config&);
    void set_min_dist_range(acoord_t dist_range);
    void set_min_blob_area(acoord_t min_area);
    void clear_assign_prev_blobs();
    void set_frames_to_live(anum_t);
    void set_tracks_to_live(anum_t);

    /*========= Detectors =========*/
    int detect_with_cascade(awiros_frame aframe, awiros_cascade &cascade);
#ifdef ENABLE_TF
   // int detect_with_mtcnn(awiros_frame aframe, awiros_mtcnn_detector&);
#endif

#ifdef ENABLE_DARKNET
    int detect_with_darknet(const awiros_frame&, awiros_darknet&);
#endif
    int detect_with_bgsub(awiros_frame aframe);
    int detect_with_contours(awiros_frame, int, int);
    int detect_with_ext_api(awiros_ext_api&, awiros_frame&);
    int detect_with_ext_api_face_count(awiros_ext_api&, awiros_frame&);
    void detect_with_ext_api_face_attributes(awiros_ext_api&, awiros_frame&, awiros_blob&, awiros_blob_class&);
    int detect_with_ext_api_face_db(awiros_ext_api&, awiros_frame&, std::vector<awiros_blob>&);
    void get_features_with_ext_api(awiros_ext_api& api, awiros_blob& blob,
        std::vector<std::string> payloads);
    void get_feature_with_ext_api(awiros_ext_api& api, awiros_blob& blob);
    void get_hard_feature_with_ext_api(awiros_ext_api& api, awiros_blob& blob);
    int detect_with_ext_api_2(awiros_ext_api&, awiros_blob&);
    int detect_with_openalpr(awiros_ext_api&, awiros_frame,
        std::vector<awiros_blob>&);
    int detect_with_ext_api_3(awiros_ext_api&, std::vector<awiros_blob>&, awiros_frame); 
    json detect_with_ext_api_crowd(awiros_ext_api&, awiros_frame&, awiros_blob&);
    int detect_with_ext_api_violence(awiros_ext_api&, awiros_frame&);
    int detect_with_ext_api_fire_smoke(awiros_ext_api&, awiros_frame&);

    void fill_blob_frames(awiros_frame &aframe, int padding=20);
    void fill_blob_frames_reid(awiros_frame &aframe);
    void track_curr_blobs();
    void track();
    void print_blob_hist();
    void print_active_blobs();
    void draw_curr_blobs(awiros_frame &aframe, bool with_data = true);
    
    void draw_frs_live_view(awiros_frame &, awiros_blob&, awiros_blob_class&, double);
    void draw_webstreamer_liveliness_blobs(awiros_frame &aframe, int, bool);
    
    cv::Point calculate_variance_blob(std::vector<cv::Point> blob_hist);
    void apply_template_search(awiros_frame &in_frame, std::vector<awiros_blob> &tm_blobs);
    void trigger_termination_event(int obj_id);
    void trigger_initiation_event(awiros_track &track);
    void find_distance(awiros_line line1,
    awiros_line line2);
    void draw_tracking_line(int blob_id, awiros_frame& aframe);
    void draw_tracking_lines(awiros_frame& aframe);
    
    void filter_detected_blobs(awiros_frame aframe);
    void filter_ppe_detected_blobs(awiros_frame aframe);
    double get_overlap_percent(awiros_blob, awiros_blob);
    void track_with_kalman(cv::Mat grayFrame);
    void create_blobs_mask(awiros_frame aframe, std::vector<awiros_region> regions, std::vector<awiros_line> lines, std::vector<awiros_blob> blobs, cv::Mat& binary_mask, cv::Mat &color_mask);

};



#endif // _TRACK_H_
