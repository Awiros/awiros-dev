#ifndef _AWIROS_APP_H_
#define _AWIROS_APP_H_
#include <string>

#include <opencv2/opencv.hpp>
#include "camera.h"
#include "frame.h"
#include "motion.h"
#include "blob.h"
#include "event.h"
#include "types.h"
#include "consts.h"
#include "detector.h"
#include "triggers.h"

//#include "src/trainer_config.h"

#include <set>

class conversion_cycle_layer; //forward declaration TODO

/*
template<typname T>
class awiros_base_app{
  public:
    static std::unique_ptr<awiros_base_app> create_instance(){
      return T::create_instance();
    };
}
*/

//class awiros_app: public awiros_base_app<awiros_app>{
class awiros_app{
  private:

    std::size_t m_last_frame_num=0;
    int64_t m_last_ticks = 0;
    int64_t m_last_ticks_fps = 0;
  public:

    //CRTP static awiros_app* get_instance();

    awiros_app();
    ~awiros_app();
    std::string description = "Default description (Please fill appropriate app description)";
    int id; //TODO: convert type to cycle size_t
    aserial_t key;
    unsigned int time_frequency;
    unsigned int time_senstivity;
    double       space_senstivity_x;
    double       space_senstivity_y;
    double       fps;
    std::string name;
    std::size_t max_run_time;
    bool is_forever; // if the app is forever running
    bool web_stream=false;
    std::string model_path = "";
    std::string ip_address = "";

    /* Global camera decoder variable */
    aid_t camera_id;
    int app_id;

    std::vector<awiros_event>    events;
    std::vector<awiros_blob>     blobs;
    std::set<awiros_blob>        event_gen_blobs;
    std::vector<awiros_timeslot> active_time;
    std::vector<awiros_point>    active_mask;
    std::set<int, awiros_blob>   unique_event_blobs;
    awiros_severity severity;
    bool alert;

    //awiros_camera       cam;
    awiros_frame        aframe;
    awiros_motion       motion;
    awiros_blob_config  blob_config;
    awiros_tracker_config  tracker_config;
    awiros_detector_config detector_config;

    //awiros_trainer_config      trainer_config;

    virtual void trigger_state_change_events(blobhist_t &){};
    virtual void trigger_state_termination_events(std::vector<awiros_blob> &){};

    virtual void run(){};
    virtual void send_event();
    virtual void send_event_with_heat();
    conversion_cycle_layer * p_conv_layer_obj;
    void print_message(msg_t msg, std::string func,  std::string str);
    void print_fps(int secs);
    void print_app_fps(int secs, int id);
    double get_fps(int secs, bool with_print=false);
    double get_app_fps(int secs, int id, bool with_print=false);


    void clean_up();
};

typedef awiros_app* create_t();

#endif //ifndef _AWIROS_APP_H_
