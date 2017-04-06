

 apt-get -y update
 apt-get -y upgrade
 apt-get -y dist-upgrade
 apt-get -y autoremove


# INSTALL THE DEPENDENCIES

# Build tools:
 apt-get install -y build-essential apt-utils cmake zip unzip

apt-get remove ffmpeg x264 libx264-dev
# GUI (if you want to use GTK instead of Qt, replace 'qt5-default' with 'libgtkglext1-dev' and remove '-DWITH_QT=ON' option in CMake):
 apt-get install -y libvtk6-dev libgtkglext1-dev libfreetype6-dev libxft-dev python-opencv

# Media I/O:
 apt-get install -y zlib1g-dev libjpeg-dev libwebp-dev libpng-dev libtiff5-dev libjasper-dev libopenexr-dev libgdal-dev yasm

# Video I/O:
 apt-get install -y libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev yasm libopencore-amrnb-dev libopencore-amrwb-dev libv4l-dev libxine2-dev checkinstall git cmake libfaac-dev libjack-jackd2-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libsdl1.2-dev libtheora-dev libva-dev libvdpau-dev libvorbis-dev libx11-dev libxfixes-dev libxvidcore-dev texi2html yasm zlib1g-dev


apt-get install -y libgstreamer0.10-0 libgstreamer0.10-dev gstreamer0.10-tools gstreamer0.10-plugins-base libgstreamer-plugins-base0.10-dev gstreamer0.10-plugins-good gstreamer0.10-plugins-ugly gstreamer0.10-plugins-bad gstreamer0.10-ffmpeg
apt-get install -y libgtk2.0-0 libgtk2.0-dev
apt-get install -y libjpeg8 libjpeg8-dev

# Parallelism and linear algebra libraries:
 apt-get install -y libtbb-dev libeigen3-dev


# Python:
 apt-get install -y python-tk python-numpy pkg-config libgs-dev

# Java:
# apt-get install -y ant default-jdk

# Documentation:
 apt-get install -y doxygen


apt-get install -y unzip wget
apt-get -y update
# INSTALL THE LIBRARY (YOU CAN CHANGE '2.4.10' FOR THE LAST STABLE VERSION)

# find / -iname "ft2build.h"

cd /usr/local/
mkdir ~/src
## Install x264
cd ~/src
wget ftp://ftp.videolan.org/pub/videolan/x264/snapshots/x264-snapshot-20120528-2245-stable.tar.bz2

tar xvf x264-snapshot-20120528-2245-stable.tar.bz2

cd x264-snapshot-20120528-2245-stable

./configure --enable-shared --enable-pic
make
make install

apt-get update

#find / -iname "ft2build.h"



#sudo mkdir /usr/local

## Install ffmpeg
# cd ~/src
# wget http://ffmpeg.org/releases/ffmpeg-0.11.1.tar.bz2
# tar xvf ffmpeg-0.11.1.tar.bz2
# cd ffmpeg-0.11.1
#
# ./configure --enable-gpl --enable-libtheora --enable-libvorbis --enable-libx264 --enable-libxvid --enable-nonfree --enable-postproc --enable-version3 --enable-x11grab --enable-shared --enable-pic #--enable-libfaac --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb
# make
# make install
# find / -iname "ft2build.h"
## Install OpenCV
cd /usr/local/
wget http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.11/opencv-2.4.11.zip

unzip opencv-2.4.11.zip
rm  opencv-2.4.11.zip
cd  opencv-2.4.11/
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DWITH_FFMPEG=OFF .. #-DCMAKE_INSTALL_PREFIX=/usr/local  .. #-DWITH_OPENGL=ON -DFORCE_VTK=ON -DWITH_TBB=ON -DWITH_GDAL=ON -DWITH_XINE=ON -DBUILD_EXAMPLES=ON -DOPENCV_EXTRA_MODULES_PATH=/usr/src/opencv_contrib/modules/ ..
make -j4
 make install
#find / -iname "ft2build.h"
export LD_LIBRARY_PATH=/usr/local/lib
echo "PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH" >> /etc/bash.bashrc
 ldconfig
#find / -iname "ft2build.h"
#Fix (hopefully) for matplotlib
ln -s /usr/include/freetype2/ft2build.h /usr/include

#install ghostscript
wget http://ghostscript.com/releases/ghostscript-8.71.tar.gz
tar -xvzf ghostscript-8.71.tar.gz
cd ghostscript-8.71
./configure --prefix=/usr/local/ --with-system-libtiff
make XCFLAGS=-DHAVE_SYS_TIME_H=1
make install


apt-get -y update
apt-get install  -y rabbitmq-server
# Class build
# python /usr/src/app/paragon/modulepipe/Setup_class.py build_ext --inplace
# rm -rf /usr/src/app/paragon/modulepipe/build
#
# # Collate build
# python /usr/src/app/paragon/modulepipe/Setup_collate.py build_ext --inplace
# rm -rf /usr/src/app/paragon/modulepipe/build
#
# # Patchmod build
# python /usr/src/app/paragon/modulepipe/Setup_patch.py build_ext --inplace
# rm -rf /usr/src/app/paragon/modulepipe/build
#
# # Patchsing build
# python /usr/src/app/paragon/modulepipe/Setup_patchsing.py build_ext --inplace
# rm -rf /usr/src/app/paragon/modulepipe/build
#
# # Patchsing build
# python /usr/src/app/paragon/modulepipe/Setup_patchsing.py build_ext --inplace
# rm -rf /usr/src/app/paragon/modulepipe/build
#
# # Move .so files to proper locations
# cp /usr/src/app/paragon/modulepipe/patchmod.so /usr/src/app/paragon/patchmod.so
# cp /usr/src/app/paragon/modulepipe/patchsingmod.so /usr/src/app/paragon/patchsingmod.so
# cp /usr/src/app/paragon/modulepipe/collatemod.so /usr/src/app/paragon/collatemod.so
# cp /usr/src/app/paragon/modulepipe/classmod.so /usr/src/app/paragon/classmod.so
#
#
# # Dewarpmod build
# python /usr/src/app/paragon/dewarpmodule/src/ocropy-master/setup.py # Setup Ocropy
# python /usr/src/app/paragon/dewarpmodule/src/leptonlib-1.67/src/MakeDewarp.py build_ext --inplace
# #rm -rf /usr/src/app/paragon/dewarpmodule/src/leptonlib-1.67/src/build
# cp /usr/src/app/paragon/dewarpmodule/src/leptonlib-1.67/src/dewarpmod.so /usr/src/app/paragon/dewarpmod.so
