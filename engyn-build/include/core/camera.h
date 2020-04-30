#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "types.h"
#include "frame.h"
#include "blob.h"
#include "resource_manage.h"
#include <queue>
#include <thread>

void test_linking(int num);

typedef struct frame_struct{
  //awiros_frame starv_frame;
  //int starv_frame_index = 0;
  std::size_t processed_frame = 0;
  std::vector<awiros_region> app_regions;
  std::vector<awiros_line> app_lines;
  std::vector<awiros_blob> app_blobs;
  int app_width;
  int app_height;

  std::vector<cv::Mat> frame_bank;
  int frame_ind_new = -1;
  //int thread_running = 0;
  //int thread_stop = 0;

  cv::Mat mask_box_binary;
  cv::Mat mask_box_color;

} frame_struct;

class awiros_camera : public awiros_resource{
    
    aid_t             m_id=0;
    aid_t             m_tid=0;
    aid_t             m_client_id=0;
    std::string       m_url;
    time_t            m_tepoch;
    std::size_t       m_hres=0;
    std::size_t       m_vres=0;
    std::size_t       m_tframes=0;
    double            m_fps=0.0;
    std::string       m_format;
    std::size_t       m_nframes=0;
    cv::VideoCapture  m_cap;
    cv::VideoWriter   m_writer;
    int               m_is_initialized=0;
    cv::Mat           m_frame;
    bool              video_looping = false;
    //char*            m_base64_frame;
    
    mutable std::mutex  g_cam;
    mutable std::condition_variable g_cv;


    int               queue_wait = 10;//25;
    int               app_registry_time = 100;
    
    // TODO So far built directly on OpenCV VideoCapture, need to build on top of awiros driver

  public:

    /* Creating a global decoder for frame */
    //awiros_frame global_cam_frame;
    //std::size_t frame_indexer=0;
     
    std::size_t frame_ind = 0;
    
    //int frame_ind_new = -1;
    //static std::size_t frame_bank_round;// = 0;
    //static int frame_limit;// = 25;
    
    std::size_t frame_bank_round = 0;
    int frame_limit = 25;
    int cam_ready = 1;
    
    std::queue<std::size_t> frame_index;
    std::queue<awiros_frame> global_frame_q;

    frame_struct fs_caller;
    std::map<int, frame_struct> app_frames;
    std::map<int, frame_struct>::iterator app_frame_itr;

    //std::vector<std::thread> subscriber_thread;
    std::size_t global_frame_index = 0;
    
    bool check_apps = false; // If all apps using that decoder are stopped then stop the decoder as well. 
    void test_link(int num);
    awiros_camera();
    awiros_camera(aid_t id, aid_t tid, const std::string url);
    ~awiros_camera();

    aid_t get_id(void);
    void  set_id(aid_t id);
    void  set_video_looping(bool status);

    //static void capture_frame(awiros_frame&, frame_struct&, std::size_t&);

    aid_t get_tid();
    void  set_tid(aid_t tid);

    std::string   get_url();
    void          set_url(std::string url);

    int           init();
    int           init_writer(std::string filename, awiros_frame aframe);
    bool          is_writer_opened();
    void          write(awiros_frame aframe, bool show_mask = false);

    int           is_initialized();

    // int           check_camera_status();

    int           is_working();

    std::size_t   get_current_frame(cv::Mat &frame);

    // overloading just keep tampering app consistent as of now!
    std::size_t   get_current_frame(awiros_frame &aframe);
    void          set_current_frame(cv::Mat &frame);

    // For decoder in camera
    //std::size_t   get_current_frame(awiros_frame &aframe, aid_t cam_id); // function overloading
    //std::size_t   get_current_frame(awiros_frame &aframe, aid_t cam_id, int appID); // function overloading
    //std::size_t   get_global_frame(awiros_frame &aframe);
    std::size_t   get_global_frame_new(awiros_frame &aframe);
    std::size_t   get_current_global_frame(awiros_frame &aframe, aid_t cam_id);

    //anum_t        get_awiros_frame(awiros_frame &af);
    void set_height(afloat_t){};
    void set_vertical_angle(afloat_t){};
    void set_fov_angle(afloat_t){};
    void set_focal_len(afloat_t){};
    void set_sensor_size(afloat_t){};
    void set_sensor_type(std::string){};

    cv::Point     get_cam_res();
    std::size_t   get_nframes();
    double        get_fps();

    //char*         get_base64_frame();
    time_t        get_current_time();
    std::string   get_base64_frame();
    //void          set_frame(cv::Mat frame);
    void          release();
};

std::string convert_to_base64(cv::Mat& frame);

#endif //_CAMERA_HPP_
