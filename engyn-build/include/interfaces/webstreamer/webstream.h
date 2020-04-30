#ifndef _AWIROS_WEB_STREAM_H
#define _AWIROS_WEB_STREAM_H
#include "../../core/frame.h"
#include <thread>
#include <mutex>

#include "../../ext/mjpeg-streamer/MJPEGWriter.h" 
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>


class awiros_stream {
  private:
    int                 m_port=8111;
    bool                m_status=false;
    std::string         m_srv_add;
    std::vector<uchar>  m_imgbuf;
    bool                m_frame_ready=false;
    MJPEGWriter         m_mjpeg_writer;

    
public:
   // awiros_stream(){};

    awiros_stream(int port, std::string srv_add);
    ~awiros_stream();
    void init(int port, std::string srv_add);
    void set_port(int port);
    bool get_state();

    void publish_frame(awiros_frame aframe);
    void kill();
    void cleanup_all_clients();
    void kill_stream_server();
};



#endif// _AWIROS_WEB_STREAM_H


