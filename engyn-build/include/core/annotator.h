#ifndef _AWIROS_ANNOTATOR_H_
#define _AWIROS_ANNOTATOR_H_

#include "app.h"
#include "tracker.h"

class awiros_annotator{
    private:
        cv::Scalar m_awiros_color = cv::Scalar(0,204,255);
        int m_colratio = 10;
        int m_rect_ratio  =12;
        int m_margin_logo = 18;
        int m_margin = 22;
        int m_box_start_y = 50;
    
    public:


      int total_detections = 0;
      int detections_in_current_frame = 0;
      int max_crowd_density = 0;
      int frame_number = 0;



      awiros_app& app;
      awiros_tracker& tracker;
      std::vector<std::pair<std::string, int>> app_fields;


        //awiros_annotator(awiros_app &app, awiros_tracker &tracker);
      awiros_annotator(awiros_app &app, awiros_tracker &tracker):
                    app(app), tracker(tracker){}

      //std::map<std::string, int> app_data;

        //std::map<std::string, std::string> create_table(std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string);

      void annotate(std::string app_name = "default", std::map<std::string, std::string> app_data = std::map<std::string, std::string>());
        //void annotate(std::string app_name = "default", std::string f1= "f1", std::string v1="v1", std::string f2="f2", std::string v2="v2", std::string f3="f3", std::string v3="v3", std::string f4="f4", std::string v4="v4" );
        
        void update_event_gen_tracks(std::vector<int> &ids);
        void persist_event_blob_annotations(std::vector<int> &ids);
        void add_header(std::string app_name, std::map<std::string, std::string> app_data);
        //void add_header(std::string app_name, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string);
        void add_watermark_wrt_header(unsigned char *logo_png, unsigned int logo_png_len, awiros_app& app);
};
#endif // _AWIROS_ANNOTATOR_H_
