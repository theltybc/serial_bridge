const net = require("net");

const client = new net.Socket();

const HOST = "192.168.1.131";
const PORT = 2000;

var server = net.createServer(function (socket) {
  socket.write('Echo server\r\n');
  socket.pipe(process.stdin).pipe(socket);
});

server.listen(PORT, HOST);

process.stdin.resume();

process.stdin.setEncoding('utf8');
