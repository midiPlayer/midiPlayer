#!/bin/bash
echo 'ATTRS{idProduct}=="6001",GROUP="users",MODE="0660"' > /etc/udev/rules.d/09-enttec-open-dmx.rules
