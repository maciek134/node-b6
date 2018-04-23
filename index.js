const EventEmitter = require('events');
const util = require('util');
const { Device } = require('node-cmake')('node_b6');

util.inherits(Device, EventEmitter);

function blankListen() { throw new Error("Cannot listen more than once!"); }

Device.prototype.listen = function (options) {
  const opt = {
    polling: true,
    pollingInterval: 500,
    ...options,
  };
  try {
    this._listen();
    this.emit('connected');
    if (opt.polling) {
      setInterval(() => this.poll(), opt.pollingInterval);
    }
    this.listen = blankListen;
  } catch (e) {
    setTimeout(() => this.listen(), opt.pollingInterval);
    this.emit('unconnected');
  }
};

Device.prototype.poll = function () {
  try {
    const { state, ...info } = this.getChargeInfo();
    switch (state) {
      case 0x01:
        this.emit('charging', info);
        break;
      case 0x03:
        this.emit('complete', info);
        break;
    }
  } catch (e) {
    this.emit('error', e);
  }
};

module.exports = new Device();