//const {IsPrime} = require('./build/Release/addon'); // native c++
var myAwiros = require('../build/Release/awiros-node'); // native c++
//var myAwiros = require("bindings")("awiros-node");
var fs = require("fs");
process.env.UV_THREADPOOL_SIZE = 16;



var acs = {
  "awi_client_id" : 123,
  "awi_terminal_id" : 32,
  "awi_camid" : 3,
  "awi_name" : "awi_camera",
  "awi_app" : "awi_tampering",

  "awi_camera":{
    "url": "rtsp://admin:admin123@192.168.0.59:554/Streaming/Channels/502",
    "id": 231,
    "ip_add": "192.168.0.59",
    "cam_no": 5,
    "stream_id": 2,
    "port": 554,
    "username": "admin",
    "password": "admin123",
    "manufacturer": "awi_manu_hikvision",
    "model": "awi_model_model",
    "url_proto": "rtsp://<USERNAME>:<PASSWORD>@<IP>:<PORT>/Streaming/Channels/<CAMID>0<STREAMID>",
    "location": "office",
    "recheck_time":5
  },
  "awi_frame": 
  {
    "seq":1234,
    "width":640,
    "height":480,
    "img":"base64",
    "channels":3
  },

  "awi_term":{  
    "id":123,
    "valid_until":{  
      "year":2019,
      "month":8,
      "m_day":1,
      "hour":7,
      "min":28,
      "sec":33,
      "timezone":"UTC",
      "timestamp":"1231231213 2019-08-01T07:28:33.491Z"
    },
    "key": "63224584875655358253497040"
  },
  "awi_tampering":{
    "recheck_time":12
  },
  "awi_frs":{

  },
  "__v":0
}


acs["awi_camera"]["id"]  = 45;
acs["awi_camera"]["tid"] = 445;
acs["awi_camera"]["url"] = "/home/awiros-tech/sources/engyn/fd.mp4";

myAwiros.EngynProgressWithACS(JSON.stringify(acs), 11, function(i) {
     //var abc = i.substring(0,168);
     console.log('STR: ' + i.length);
     //var abc = JSON.parse(i);
     console.log("[NODE} EVENT RECEIVED");
  }, function (idid) {
    //var test=JSON.parse(idid);
    console.log(idid)
    //console.log("TRY2 : STATUS: "+test["awi_camera"]["status"]);

  })

