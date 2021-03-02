#/bin/sh -c
pwd
echo "Current PATH=$PATH"
case "$1" in
	ignitech)
	#    - name: Install libIgnitech
		git clone https://github.com/techie66/libIgnitech.git
		cd libIgnitech
		./autogen.sh
		./configure
		make
		sudo make install
        	cd ..
		;;

	isp)
	#    - name: Install libISP2
		git clone https://github.com/techie66/libISP2.git
		cd libISP2
		./autogen.sh
		./configure
		make
		sudo make install
		cd ..
		;;

	bluetooth)
	#    - name: Install Bluetooth headers
		sudo apt-get install libbluetooth-dev
		;;
	flatbuffers)
	#    - name: Install Flatbuffers
		git clone https://github.com/google/flatbuffers.git
		cd flatbuffers
		cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
		make
		sudo make install
		cd ..
		;;

	bcm2835)
	#    - name: Install bcm2835lib
		wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.68.tar.gz
		tar -xf bcm2835-1.68.tar.gz
		cd bcm2835-1.68
		./configure
		make
		sudo make install
		cd ..
		;;
	build-essential)
	#    - name: Install build-essential
		sudo apt-get install build-essential
		cd ..
		;;
esac
