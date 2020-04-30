//const {IsPrime} = require('./build/Release/addon'); // native c++
var myAwiros = require('../build/Release/awiros-node'); // native c++
//var myAwiros = require("bindings")("awiros-node");
var fs = require("fs");

process.env.UV_THREADPOOL_SIZE = 16;

function test_launch(){
    var register_acs = {  
          "awi_term":{  
              "valid_until":{  
                  "year":2019,
                  "month":8,
                  "m_day":1,
                  "hour":7,
                  "min":28,
                  "sec":33,
                  "timezone":"UTC",
                  "timestamp":"2019-08-01T07:28:33.491Z"
                },
              "key": ""

          }
    };

    var register_ads;

    console.log("Registering terminal ..");
    // Register Terminal and store key in the database
    myAwiros.EngynProgressWithACS(JSON.stringify(register_acs), 3, function(data) {
        register_ads = JSON.parse(data);
        console.log("Key: " + register_ads["awi_term"]["key"]);
        authenticate_term(register_ads);
      }, function (idid) {
        console.log(idid);

      });

}


function authenticate_term(register_ads) {
    // Create new auth acs [SAME AS ABOVE ADS WITH KEY FILLED]
    var auth_acs = register_ads;

    console.log("Authenticating terminal ..");

    // Authenticate terminal
    myAwiros.EngynProgressWithACS(JSON.stringify(auth_acs), 5, function(data) {
         auth_ads = JSON.parse(data);
         key_status = auth_ads["awi_term"]["key_status"];
         
         if(key_status == "AUTH_OK"){
            console.log("Key status " + key_status);
            review_and_launch_app();
         }
         
    
      }, function (idid) {
        console.log(idid);

      });

}

// Start application
function start_app(acs){
  
    myAwiros.EngynProgressWithACS(JSON.stringify(acs), 4, function(data) {
         var app_ads = JSON.parse(data);
         console.log(app_ads);
      
      }, function (idid) {
         console.log(idid);

      });

}


function review_and_launch_app(){

  var acs = {
    "awi_client_id" : 123,
    "awi_terminal_id" : 32,
    "awi_camid" : 3,
    "awi_name" : "awi_camera",
    "awi_app" : "awi_tampering",

    "awi_camera":{
      "url": "rtsp://admin:admin123@192.168.0.59:554/Streaming/Channels/302",
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
    }

  }
  
  
    var cams = [302,502,702,902,1102,1302,1602];
    var cams = [302];
    for (var i in cams){

        console.log("Adding camera number: " + cams[i]);

        acs["awi_camera"]["url"] =  "rtsp://admin:admin123@192.168.0.59:554/Streaming/Channels/"+ cams[i];
        acs["awi_camera"]["url"] =  "/home/awiros-tech/sources/awiros/fd.mp4"; 

        acs["awi_camera"]["id"] = cams[i];
        
        console.log(acs["awi_camera"]["url"]);

        // Add camera
        myAwiros.EngynProgressWithACS(JSON.stringify(acs), 1, function(data) {
            cam = JSON.parse(data)["awi_camera"];
            cam_status = cam["status"];
            
            if(cam_status){
                console.log("[NODE-DEBUG] camera "+ cam["id"] + " is up");
                start_app(acs);
            }

            else{
                console.log("[NODE-DEBUG] camera "+ cam["id"] + " is down");
            }

          }, function (idid) {
            console.log(idid);

          });
     }
}



test_launch();

// Install application
// skip


// Application level authentication
// skip






// Update configuration

// 1. Stop application
// 2. fetch new config
// 3. Authenticate 
// 4. Restart

