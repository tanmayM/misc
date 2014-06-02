#!/bin/bash
# Author: Tanmay V. Mande



if [ $# -eq 0 ] || [ $1 = "--help" ]
   then
    echo ""
    echo "show  		= $./traffic-control.sh 'iface' show"
    echo ""
    echo "flush 		= $./traffic-control.sh 'iface' flush"
    echo ""
    echo "delay   	=  $./traffic-control.sh 'iface' \"delay ValUnit\" 
		where ValUnit is delay to be introduced in number followed by unit form and Unit=s|ms|us.   
		e.g. $./traffic-control eth0 \"delay 50ms\""
    echo ""
    echo "jitter  	=  $./traffic-control.sh 'iface' \"delay ValUnit1 ValUnit2 \" 
		where first argument is delay, second argument is jitter. Unit=s|ms|us
		e.g. $./traffic-control.sh eth0 \"delay 100ms 10ms   Here jitter is 10ms\""
    echo ""
    echo "packet loss	=  $./traffic-control.sh 'iface' \"loss Val% \" 
		Val is percent packet loss to be introduced. Note that you need to give percent sign at the end.
		e.g. $./traffic-control.sh lo \"loss 3%\""
    echo ""
    echo "duplicate  	=  $./traffic-control.sh 'iface' \"duplicate Val% \" 
		Val is percent packet duplication. Note that you need to give percent sign at the end.
		e.g. $./traffic-control.sh eth1 \"duplicate 2%\""
    echo ""
    echo "reorder  	=  $./traffic-control.sh 'iface' \"delay ValUnit reorder Val2% \" 
		Val2 is percent packet reordering. Note that You need to provide a delay first. you need to give percent sign at the end.
		e.g. $./traffic-control.sh usb0 \"delay 100ms reorder 2%\""
    echo ""
    echo "corrupt  	=  $./traffic-control.sh 'iface' \"corrupt Val2% \" 
		Val is percent packet corruption to be introduced. Note that you need to give percent sign at the end.
		e.g. $./traffic-control.sh lo \"corrupt 3%\""
	echo ""
	echo "real-network	= $./traffic-control.sh eth0 real-network    
		Its a readymade command which will modify conditions similar to real world network. It is same as:
		$./traffic-control eth0 \"delay 200ms 40ms 25% loss 5% 25% duplicate 1% corrupt 0.1% reorder 25% 50%\""
	echo ""
	echo "good-network	= $./traffic-control.sh eth0 good-network    
		Its a readymade command which will modify conditions similar to a good network. It is same as:
		$./traffic-control eth0 \"delay 50ms loss 0.5% 25% duplicate 1% corrupt 0.1% reorder 10% 50%\""
	echo ""
	echo "bad-network	= $./traffic-control.sh eth0 bad-network    
		Its a readymade command which will modify conditions similar to a bad network. It is same as:
		$./traffic-control eth0 \"delay 400ms loss 15% 25% duplicate 5% corrupt 1% reorder 20% 50%\""

    echo ""
   exit
 fi

if [ "$2" = "flush" ]
   then
     echo "netem profile flush"
     tc qdisc del dev $1 root netem

elif [ "$2" = "show" ]
   then
     tc qdisc show dev $1
elif [ "$2" = "real-network" ]
   then
    tc qdisc show dev $1 | grep "netem" > /dev/null
    if [ $? = 0 ]
    then
     tc qdisc change dev $1 root netem delay 200ms 40ms 25% loss 3% 25% duplicate 3% corrupt 1% reorder 5% 50%
	else
     tc qdisc add dev $1 root netem delay 200ms 40ms 25% loss 3% 25% duplicate 3% corrupt 1% reorder 5% 50%
	fi
elif [ "$2" = "good-network" ]
   then
    tc qdisc show dev $1 | grep "netem" > /dev/null
    if [ $? = 0 ]
    then
     tc qdisc change dev $1 root netem delay 50ms loss 0.5% 5% duplicate 1% corrupt 0.1% reorder 3% 50%
	else
     tc qdisc add dev $1 root netem delay 50ms loss 0.5% 5% duplicate 1% corrupt 0.1% reorder 3% 50%
	fi
elif [ "$2" = "bad-network" ]
   then
    tc qdisc show dev $1 | grep "netem" > /dev/null
    if [ $? = 0 ]
    then
     tc qdisc change dev $1 root netem delay 400ms 100ms 25% loss 15% 25% duplicate 5% corrupt 1% reorder 20% 50%
	else
     tc qdisc add dev $1 root netem delay 400ms 100ms 25% loss 15% 25% duplicate 5% corrupt 1% reorder 20% 50%
	fi
else
echo "$2" 
    tc qdisc show dev $1 | grep "netem" > /dev/null
    if [ $? = 0 ]
    then
        tc qdisc change dev $1 root netem $2
    else
        tc qdisc add dev $1 root netem $2
    fi
fi
exit
