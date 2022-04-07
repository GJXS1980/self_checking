sudo apt-get install mplayer
sudo touch -f /etc/udev/rules.d/UART.rules
sudo touch -f /etc/udev/rules.d/video.rules

echo  'KERNEL=="ttyUSB*", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", MODE:="0777", GROUP:="dialout",  SYMLINK+="STM32_UART"' >/etc/udev/rules.d/UART.rules

echo  'KERNEL=="ttyUSB*", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE:="0777", GROUP:="dialout",  SYMLINK+="radar"' >>/etc/udev/rules.d/UART.rules

echo  'KERNEL=="ttyS*", MODE:="0777"' >>/etc/udev/rules.d/UART.rules

#echo  'KERNEL=="video*", ATTRS{idVendor}=="1689", ATTRS{idProduct}=="2304", MODE:="0777", GROUP:="dialout",  SYMLINK+="prep_camera"' >/etc/udev/rules.d/video.rules

echo  'KERNELS=="1-4.3:1.0", SUBSYSTEM=="video4linux", ATTR{index}=="0",SUBSYSTEMS=="usb", MODE:="0666", OWNER:="hgrobot",  SYMLINK+="prep_camera"' >/etc/udev/rules.d/video.rules

echo  'KERNELS=="1-4.2:1.0",SUBSYSTEM=="video4linux",ATTR{index}=="0",SUBSYSTEMS=="usb", MODE:="0666", OWNER:="hgrobot",  SYMLINK+="post_camera"' >>/etc/udev/rules.d/video.rules

service udev reload
sleep 2
service udev restart

sudo rm -f /etc/udev/rules.d/rplidar.rules
sudo rm -f /etc/udev/rules.d/castle_stm32.rules

gnome-session-properties
