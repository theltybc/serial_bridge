const SerialPort = require('serialport');

let timeStart;
let time = 0;
let count = 0;
let lost = 0;
let lostTimeout = 0;
let wrong = 0;

let timeout;

const SIZE = 200
const TIMEOUT = 1000;

const END_SYMBOL = "z";
const BODY_SYMBOL = "a";

const port1 = new SerialPort('/dev/ttyUSB0', {baudRate: 115200});
const port0 = new SerialPort('/dev/ttyUSB1', {baudRate: 115200});


port1.on('data', onData(port0));
port1.on('error', function (err) {
  console.error('Error1: ', err.message);
});

port0.on('data', onData(port1));
port0.on('error', function (err) {
  console.error('Error0: ', err.message);
});

function onData(port) {
  let dataLen = 0;
  return function onData(data) {
    dataLen += data.length;
    data = data.toString();
    // console.log('Data1:', data);
    if (data[data.length - 1] == END_SYMBOL) {
      if (dataLen !== SIZE) {
        console.log("dataLen", dataLen);
        lost += Math.ceil(dataLen / SIZE);
      }
      dataLen = 0;
      time += new Date() - timeStart;
      count++;
      write(port);
    }
    data.split("").forEach(function (char) {
      if (char === BODY_SYMBOL || char === END_SYMBOL) {
        return;
      } else {
        wrong++;
      }
    });
  }
}

function write(port) {
  timeStart = new Date();
  port.write(BODY_SYMBOL.repeat(SIZE - 1) + END_SYMBOL, function (err) {
    if (err) {
      return console.error('Error on write: ', err.message);
    } else {
      // console.log('message written');
    }
  });
  clearTimeout(timeout);
  timeout = setTimeout(function () {
    lostTimeout++;
    write(port);
  }, TIMEOUT);
};

write(port0);

process.stdin.resume();
process.stdin.setEncoding('utf8');
process.stdin.on("data", logResult);

process.on("beforeExit", logResult2);
process.on("SIGTERM", logResult2);
process.on("SIGINT", logResult2);

function logResult2() {
  logResult();
  process.exit();
}

function logResult() {
  console.log("avg:", time / count);
  console.log("time:", time);
  console.log("count:", count);
  console.log("wrong:", wrong);
  console.log("lost:", lost);
  console.log("lostTimeout:", lostTimeout);
}
