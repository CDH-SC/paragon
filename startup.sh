#!/bin/bash
rm -rf /usr/src/app/paragon/modulepipe/build
cd /usr/src/app/paragon/modulepipe/
# Class build
python Setup_class.py build_ext --inplace
rm -rf /usr/src/app/paragon/modulepipe/build

# Collate build
python Setup_collate.py build_ext --inplace
rm -rf /usr/src/app/paragon/modulepipe/build

# Patchmod build
python Setup_patch.py build_ext --inplace
rm -rf /usr/src/app/paragon/modulepipe/build

# Patchsing build
python Setup_patchsing.py build_ext --inplace
rm -rf /usr/src/app/paragon/modulepipe/build

# Patchsing build
python Setup_patchsing.py build_ext --inplace
rm -rf /usr/src/app/paragon/modulepipe/build


cd /usr/src/app/paragon/dewarpmodule/src/ocropy-master/
# Dewarpmod build
python setup.py # Setup Ocropy
cd /usr/src/app/paragon/dewarpmodule/src/leptonlib-1.67/src/
python MakeDewarp.py build_ext --inplace
#rm -rf /usr/src/app/paragon/dewarpmodule/src/leptonlib-1.67/src/build
cp /usr/src/app/paragon/dewarpmodule/src/leptonlib-1.67/src/dewarpmod.so /usr/src/app/paragon/dewarpmod.so

# Move .so files to proper locations
cp /usr/src/app/paragon/modulepipe/patchmod.so /usr/src/app/paragon/patchmod.so
cp /usr/src/app/paragon/modulepipe/patchsingmod.so /usr/src/app/paragon/patchsingmod.so
cp /usr/src/app/paragon/modulepipe/collatemod.so /usr/src/app/paragon/collatemod.so
cp /usr/src/app/paragon/modulepipe/classmod.so /usr/src/app/paragon/classmod.so

# Start Django server
#gunicorn paragon.wsgi:application -w 2 -b :8002
