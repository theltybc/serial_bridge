const SerialPort = require('serialport');

let timeStart;
let time = 0;
let count = 0;
function exit() {
  console.log("avg", time / count);
  process.exit();
}

const port1 = new SerialPort('/dev/ttyUSB3', {baudRate: 115200});

port1.on('data', function (data) {
  // console.log('Data1:', data.toString());
  if (data.toString().includes("z")) {
    time += new Date() - timeStart;
    count++;
    send(port0);
    setTimeout(exit, 1000);
  }
});
port1.on('error', function (err) {
  console.log('Error1: ', err.message);
});


const port0 = new SerialPort('/dev/ttyUSB2', {baudRate: 115200});
port0.on('data', function (data) {
  console.log('Data0:', data.toString());
});
port0.on('error', function (err) {
  console.log('Error0: ', err.message);
});

function send(port) {
  timeStart = new Date();
  port.write("x".repeat(10000) + 'z', function (err) {
    if (err) {
      return console.log('Error on write: ', err.message);
    } else {
      // console.log('message written');
    }
  });
}

send(port0);