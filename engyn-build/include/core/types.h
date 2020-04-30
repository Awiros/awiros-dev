#ifndef _TYPES_HPP_
#define _TYPES_HPP_

//typedef long unsigned int aid_t;
typedef std::streamsize       aid_t;  /* similar to POSIX ssize_t */
typedef std::string        aserial_t;
typedef std::size_t       anum_t;
typedef unsigned int      acoord_t;
typedef float            afloat_t;


typedef enum awiros_severity {
    LOW,
    MID,
    HIGH,
    CRITICAL,
    N_SEVERITY_LEVELS
} awiros_severity;


typedef enum awiros_orient{
    COLI = 0,
    CLK = -1,
    ACLK = 1
} awiros_orient;


typedef struct awiros_color {
  int r;
  int g;
  int b;

} awiros_color;


typedef struct awiros_timeslot{
    int starth;
    int endh;
    awiros_timeslot(int sh, int eh): starth(sh), endh(eh) {}

} awiros_timeslot;


class awiros_dims{
    public:
    afloat_t w;
    afloat_t h;
    awiros_dims(){};
    awiros_dims(afloat_t w, afloat_t h):
      w(w), h(h){};
};



typedef enum awiros_app_state{
    CONTINUE,
    STOP,
    RESTART
} awiros_app_state;


typedef enum awiros_status{
  SUCCESS,
  ERROR,
  NOACTION
} awiros_status;



#endif// _TYPES_HPP_
