var net = require('net');

var LOCAL_PORT = 28001;
var validTags = ['X3C86AD9'];

var validateTag = function(tag) {
    return validTags.indexOf(tag) > -1;
};

var server = net.createServer(function (socket) {
    console.log(socket.remoteAddress + ":" + socket.remotePort);
    socket.on('data', function(msg) {
        var out;
        [action, tag] = msg.toString().toUpperCase().trim().split(":");
        console.log(action, tag);
        switch (action) {
            case 'OPEN':
                out = validateTag(tag) ? "OK" : "NOK";
                console.log(out);
                socket.write(out);
                break;
            default:
                console.log("unknown action:", action);
        }

        socket.destroy();
    });
});

server.listen(LOCAL_PORT, '0.0.0.0');
console.log("server on port: " + LOCAL_PORT);
