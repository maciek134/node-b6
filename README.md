node-b6
=====
This is a Node.js wrapper around [maciek134/libb6](https://github.com/maciek134/libb6).
 
Installation
------------
Dependencies:
```
libusb-1.0 libb6
```

Grab `libb6` from https://github.com/maciek134/libb6.

Install with 
```
$ npm install b6
```

Either run the programs that use it as root (**not recommended**) or create an udev rule similar to this one:
```udev
SUBSYSTEM=="usb", ATTRS{idVendor}=="0000", ATTRS{idProduct}=="0001", MODE:="666", GROUP="plugdev"
KERNEL=="hidraw*", ATTRS{idVendor}=="0000", ATTRS{idProduct}=="0001", MODE="0666", GROUP="plugdev"
```