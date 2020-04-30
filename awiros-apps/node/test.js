var myAwiros = require('../build/Release/awiros-apps.node'); // native c++


var fs = require("fs");
//process.env.UV_THREADPOOL_SIZE = 16;
var acsFile = fs.readFileSync("node/new_updated_acs.json");
var acs = JSON.parse(acsFile);

myAwiros.EngynProgressWithACS(JSON.stringify(acs), parseInt(101,10), function(resads) {
  console.log("[NODE} EVENT RECEIVED ");
}, function (idid) {
  console.log(idid)
})


setTimeout(function(){
  console.log("starting" + process.argv[2] + "app");
  myAwiros.EngynProgressWithACS(JSON.stringify(acs), parseInt(process.argv[2],10), function(resads) {
    console.log("[NODE} EVENT RECEIVED ");
  }, function (idid) {
    console.log(idid)
  })
}, 5000);

