var dgram = require('dgram');
var server = dgram.createSocket('udp4');

console.log("Socket created\n");
server.on("message", function(msg, rinfo) {
   console.log("Received message" + msg.toString()); 
});
server.bind(9000);
