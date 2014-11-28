#!/bin/bash

killall -q ArduCopter.elf
pkill -f sim_multicopter.py

autotest=$(dirname $(readlink -e $0))
firmware=/tmp/ArduCopter.build/ArduCopter.elf

set -e
set -x

target=sitl
frame="X"

if [ $# -gt 0 ]; then
    case $1 in
	+|X|quad)
	    target="sitl"
	    frame="$1"
	    shift
	    ;;
	octa)
	    target="sitl-octa"
	    frame="$1"
	    shift
	    ;;
    esac
fi

echo "Building with target $target for frame $frame"

if [ ! -f ${firmware} ]; then
	pushd $autotest/../../ArduCopter
	make clean $target
	popd
fi

tfile=$(mktemp)
echo r > $tfile

# ArduCopter.elf options:
#	-w          wipe eeprom and dataflash
#	-r RATE     set SITL framerate
#	-H HEIGHT   initial barometric height
#	-C          use console instead of TCP ports
#	-I          set instance of SITL (adds 10*instance to all port numbers)

#gnome-terminal -e "gdb -x $tfile --args /tmp/ArduCopter.build/ArduCopter.elf"
gnome-terminal -e ${firmware}
#gnome-terminal -e "valgrind -q /tmp/ArduCopter.build/ArduCopter.elf"

sleep 2
rm -f $tfile

# sim_multicopter.py options:
#  -h, --help       show this help message and exit
#  --fgout=FGOUT    flightgear output (IP:port)
#  --simin=SIMIN    SIM input (IP:port)
#  --simout=SIMOUT  SIM output (IP:port)
#  --home=HOME      home lat,lng,alt,hdg (required)
#  --rate=RATE      SIM update rate
#  --wind=WIND      Simulate wind (speed,direction,turbulance)
#  --frame=FRAME    frame type (+,X,octo)

# HOME ORIGINAL: 40.072842,-105.230575,1586,0

gnome-terminal -e "${autotest}/pysim/sim_multicopter.py --frame=$frame --home=60.196051,30.323340,70,270"

# TODO: for some reason it doen't work via locations.txt
# gnome-terminal -e "${autotest}/pysim/sim_multicopter.py --frame=$frame --home=Kasomovo"

sleep 2

# mavproxy.py options:
#  -h, --help            show this help message and exit
#  --master=DEVICE[,BAUD]
#                        MAVLink master port and optional baud rate
#  --out=DEVICE[,BAUD]   MAVLink output port and optional baud rate
#  --baudrate=BAUDRATE   default serial baud rate
#  --sitl=SITL           SITL output port
#  --streamrate=STREAMRATE
#                        MAVLink stream rate
#  --source-system=SOURCE_SYSTEM
#                        MAVLink source system for this GCS
#  --target-system=TARGET_SYSTEM
#                        MAVLink target master system
#  --target-component=TARGET_COMPONENT
#                        MAVLink target master component
#  --logfile=LOGFILE     MAVLink master logfile
#  -a, --append-log      Append to log files
#  --quadcopter          use quadcopter controls
#  --setup               start in setup mode
#  --nodtr               disable DTR drop on close
#  --show-errors         show MAVLink error packets
#  --speech              use text to speach
#  --aircraft=AIRCRAFT   aircraft name
#  --cmd=CMD             initial commands
#  --console             use GUI console
#  --map                 load map module
#  --load-module=LOAD_MODULE
#                        Load the specified module. Can be used multiple times,
#                        or with a comma separated list
#  --mav09               Use MAVLink protocol 0.9
#  --auto-protocol       Auto detect MAVLink protocol version
#  --nowait              don't wait for HEARTBEAT on startup
#  -c, --continue        continue logs
#  --dialect=DIALECT     MAVLink dialect
#  --rtscts              enable hardware RTS/CTS flow control
#  --mission=MISSION     mission name

mavproxy.py --master tcp:127.0.0.1:5760 --sitl 127.0.0.1:5501 --out 127.0.0.1:14550 --out 127.0.0.1:14551 --quadcopter --load-module=joystick,rc,rcsetup $*
