#!/bin/bash

mavproxy.py \
	--master tcp:127.0.0.1:5760 \
	--sitl 127.0.0.1:5501 \
	--out 192.168.1.37:14550 \
	--out 192.168.1.37:14551 \
	--quadcopter \
	--load-module=joystick,rc,rcsetup \
	$*

