cd /usr/src/app/paragon/
python ./modulepipe/Setup_class.py build_ext --inplace
rm -rf ./modulepipe/build

python ./modulepipe/Setup_collate.py build_ext --inplace
rm -rf ./modulepipe/build

python ./modulepipe/Setup_patch.py build_ext --inplace
rm -rf ./modulepipe/build

python ./modulepipe/Setup_patchsing.py build_ext --inplace
rm -rf ./modulepipe/build

#cp ./modulepipe/collatemod.so
