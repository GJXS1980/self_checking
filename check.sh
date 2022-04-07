#! /bin/bash
### BEGIN INIT INFO
# Provides:          xxxx.com
# Required-Start:    $local_fs $network
# Required-Stop:     $local_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: mylaunch service
# Description:       mylaunch service test
### END INIT INFO
gnome-terminal -- bash -c  "source /opt/ros/melodic/setup.bash;roslaunch xqserial_server xqserial.launch" #新建终端启动节点
sleep 2 #等待2秒
wait
exit 0



