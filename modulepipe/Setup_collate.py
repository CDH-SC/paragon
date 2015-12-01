from distutils.core import setup, Extension


module1 = Extension('collatemod',
          include_dirs=['/usr/local/opencv-2.4.10/lib','/usr/local/opencv-2.4.10/include','/usr/local/opencv-2.4.10/include/opencv','/usr/local/opencv-2.4.10/include/opencv2','/var/www/html/paragon/modulepipe/eigen-eigen-b23437e61a07','/var/www/html/paragon/venv/lib/python2.6/site-packages/numpy'],
          library_dirs=['/usr/local/lib/','/usr/local/opencv-2.4.10/lib'],
                  libraries=['opencv_calib3d','opencv_contrib','opencv_core','opencv_features2d','opencv_flann','opencv_gpu',
	                     'opencv_highgui','opencv_imgproc','opencv_legacy','opencv_ml','opencv_nonfree','opencv_objdetect',
	                     'opencv_photo','opencv_stitching','opencv_superres','opencv_ts','opencv_video','opencv_videostab'],
		  sources=['collatemodule.cpp','patch_match.cpp','compute_jaccard.cpp','find_diff.cpp','refine_match.cpp','tps.cpp','book_pipeline_single.cpp','imgfeatures.c','kdtree.c','minpq.c','sift.c','utils.c'],
	          language='c++',
                  extra_compile_args=['-O3'])
		  
setup(name='collatemod',
      version='1.0',
	  description='blah',
	  author='blah',
	  url='blah',
	  ext_modules=[module1])
