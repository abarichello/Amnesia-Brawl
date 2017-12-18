var dgram = require('dgram'),
file_system = require('fs'),
highscores,
server;

file_system.readFile("highscores.txt", 'utf8', function(err, data) {
    if (err) {
        console.log("Error ocurred while handling file");
        process.exit();
    }

    console.log("Loading highscores...");

    try {
        highscores = JSON.parse(data);
    } catch (e) {
        console.log("Exception ocurred on JSON");
        process.exit();
    }

    highscores.Scores.sort(function(a, b) {
        return b.Score - a.Score;
    });

    console.log(highscores);
});

server = dgram.createSocket('udp4');
console.log("Socket created\n");

server.on("message", function(msg, rinfo) {
   console.log(rinfo);

    if (rinfo.size != 4) {
        console.log("Received message " + msg.toString());
        var jsonData, i;

        try {
            jsonData = JSON.parse(msg);
        } catch (exception) {
            console.log("Invalid JSON request");
            return;
        }

        switch (jsonData.action) {
            case "AddScore":
                console.log("AddScore called...\n");
            
                if (highscores != undefined) {
                    for (i = 0; i < highscores.Scores.length; ++i) {
                        if (highscores.Scores[i].Score < jsonData.score) {
                            highscores.Scores.splice(i, 0, {"Name" : jsonData.name, "Score" : jsonData.score});
                            console.log("Inserted highscore by: " + jsonData.name);
                            break;
                        }
                    }
                }
                console.log(highscores.Scores);
                break;

            case "GetScores":
                console.log("Getting scores called...");
                var highscore_string = JSON.stringify(highscores);
                var response_buffer = new Buffer(highscore_string.length);
                response_buffer.write(highscore_string);

                server.send(response_buffer, 0, response_buffer.length, rinfo.port, rinfo.address, function(err, sent) {
                    if (err)
                        console.log("Error sending response");
                    else
                        console.log("Responded to client at " + rinfo.address + ":" + rinfo.port);
                });
                break;
        }
    }
});

server.on("listening", function() {
    var address = server.address();
    console.log("Server listening " + address.address + ":" + address.port);
});

server.bind(9000);

var web_server = require('http');
web_server.createServer(function(request, response) {
    response.writeHead(200, {'Content-Type': 'text/html'});
    response.write("<html><body><h1>Amnesia-Brawl Highscores</h1><ul>");
    for (i = 0; i < highscores.Scores.length; ++i) {
        response.write(highscores.Scores[i].Name + "&nbsp;&nbsp;" + highscores.Scores[i].Score + "<br />");
    }
    response.write("</ul></body></html>");
    response.end();
}).listen(80);
