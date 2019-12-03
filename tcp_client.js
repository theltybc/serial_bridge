const net = require("net");

const client = new net.Socket();

const HOST = "192.168.4.1";
const PORT = 2000;

client.connect(PORT, HOST, function () {
  console.log("Connected");
  client.write("Hello, server! Love, Client.");
});

client.on("error", function (err) {
  console.error("Connection error", err);
});

process.stdin.resume();
process.stdin.setEncoding('utf8');

process.stdin.pipe(client).pipe(process.stdin);
