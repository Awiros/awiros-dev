#ifndef _EVENT_HPP_
#define _EVENT_HPP_

class awiros_event{
  public:
    std::vector<awiros_blob> blobs;
    //TODO: create a lookup/enum for event name
    std::string type;
    std::string source_type_key;
    int         source_entity_idx;
};

#endif
