const net = require("net");

const HOST = "192.168.1.131";
const PORT = 3000;

var server = net.createServer(function (socket) {
  console.log("connect");
  socket.pipe(process.stdin).pipe(socket);
});

server.listen(PORT, HOST);

process.stdin.resume();

process.stdin.setEncoding('utf8');
