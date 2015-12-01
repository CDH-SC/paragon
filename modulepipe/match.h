extern "C" {

#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <stdio.h>
#include <time.h>


#define KDTREE_BBF_MAX_NN_CHKS 200
#define NN_SQ_DIST_RATIO_THR 0.49


int compute_feat_num( IplImage* img1, IplImage* img2 )
{
	struct feature* feat1, * feat2, * feat;
	struct feature** nbrs;
	struct kd_node* kd_root;
	double d0, d1;
	int n1, n2, k, i = 0;

	n1 = sift_features( img1, &feat1 );
	n2 = sift_features( img2, &feat2 );
	kd_root = kdtree_build( feat2, n2 );

    int feat_num = 0;

    /******************  Compute feat_num  **********************/
	for( i = 0; i < n1; i++ )
	{
		feat = feat1 + i;
		k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
		if( k == 2 )
		{
			d0 = descr_dist_sq( feat, nbrs[0] );
			d1 = descr_dist_sq( feat, nbrs[1] );
			if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
			{
				feat_num++;
			}
		}
		free( nbrs );
	}

	return feat_num;
}

}


extern "C" {

#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <stdio.h>
#include <time.h>


#define KDTREE_BBF_MAX_NN_CHKS 200
#define NN_SQ_DIST_RATIO_THR 0.49


int rcnt[3], ccnt[3];


int compute_sift( IplImage* img1, IplImage* img2, Point2f **feat_img1, Point2f **feat_img2, int feat_num_max )
{
	IplImage* stacked;
	struct feature* feat1, * feat2, * feat;
	struct feature** nbrs;
	struct kd_node* kd_root;
	CvPoint pt1, pt2;
	double d0, d1;
	int n1, n2, k, i, m = 0;


//    clock_t start, finish;
//    double elapsed_time;
//	char img1_file[512] = "/home/kang/Dropbox/paragon/code-matlab/1120/files/2a.jpg";
//	char img2_file[512] = "/home/kang/Dropbox/paragon/code-matlab/1120/files/2b_2.jpg";
//	img1_file = "/home/kang/Dropbox/paragon/code-matlab/1120/files/2a.jpg";
//	img2_file = "/home/kang/Dropbox/paragon/code-matlab/1120/files/2b_2.jpg";


//	printf("Usage: ./sift-hess FileName1 FileName2\n");
//	if(argc==3)
//	{
//		strcpy(img1_file, argv[1]);
//		strcpy(img2_file, argv[2]);
//	}
//	else
//	{
//		printf("Usage: ./sift-hess FileName1 FileName2\n");
//	}

	stacked = stack_imgs( img1, img2 );
	//cvNamedWindow("Stacked",1);
	//cvShowImage("Stacked", stacked);
	//cvWaitKey(0);

//	fprintf( stderr, "Finding features in img1\n" );
//	start = clock();
	n1 = sift_features( img1, &feat1 );
//	finish = clock();
//    elapsed_time = (double) (finish-start) / CLOCKS_PER_SEC;
//    printf("Elapsed time for finding features in img1: %.2f s\n", elapsed_time);

//	fprintf( stderr, "Finding features in img2\n" );
//	start = clock();
	n2 = sift_features( img2, &feat2 );
//	finish = clock();
//    elapsed_time = (double) (finish-start) / CLOCKS_PER_SEC;
//    printf("Elapsed time for finding features in img2: %.2f s\n", elapsed_time);

	kd_root = kdtree_build( feat2, n2 );

    //printf("%d matches found.\n", feat_num);

    int cnt = 0;
//	start = clock();
	for( i = 0; i < n1; i++ )
	{
		feat = feat1 + i;

        //printf("%d, %d\n", cvRound(feat->x), cvRound(feat->y));
        //printf("%d, %d\n", cvRound(feat1->x), cvRound(feat1->y));

		k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
		if( k == 2 )
		{
			d0 = descr_dist_sq( feat, nbrs[0] );
			d1 = descr_dist_sq( feat, nbrs[1] );
			if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
			{
				pt1 = cvPoint( cvRound( feat->x ), cvRound( feat->y ) );
				pt2 = cvPoint( cvRound( nbrs[0]->x ), cvRound( nbrs[0]->y ) );

				(*feat_img1)[cnt].x = feat->x;
				(*feat_img1)[cnt].y = feat->y;
				(*feat_img2)[cnt].x = nbrs[0]->x;
				(*feat_img2)[cnt].y = nbrs[0]->y;
				cnt++;

				//pt2.y += img1->height;
				pt2.x += img1->width;
				cvLine( stacked, pt1, pt2, CV_RGB(255,0,255), 1, 8, 0 );
				m++;

				feat1[i].fwd_match = nbrs[0];
			}
		}
		free( nbrs );

		if (cnt >= feat_num_max)
		{
		    break;
		}
	}
//	finish = clock();
//    elapsed_time = (double) (finish-start) / CLOCKS_PER_SEC;
//    printf("Elapsed time for matching features: %.2f s\n", elapsed_time);


//	for(i=0; i<3; i++)
//	{
//	    printf("rcnt[%d]=%d\n", i, rcnt[i]);
//	}
//
//	for(i=0; i<3; i++)
//	{
//	    printf("ccnt[%d]=%d\n", i, ccnt[i]);
//	}

//	fprintf( stderr, "Found %d total matches %d %d\n", m, n1, n2 );
//	cvNamedWindow( "Matches", 1 );
//	cvShowImage( "Matches", stacked );
//	cvWaitKey( 0 );

	//cvNamedWindow("New Matches", 1);
	//cvShowImage("New Matches", stacked);
	//cvNamedWindow("Image 1", 1);
	//cvShowImage("Image 1", img1);
	//cvWaitKey( 0 );

	//avRANSAC(feat1, feat2, m);
	/*
	UNCOMMENT BELOW TO SEE HOW RANSAC FUNCTION WORKS

	Note that this line above:

	feat1[i].fwd_match = nbrs[0];

	is important for the RANSAC function to work.
	*/
	/*
	{
		CvMat* H;
		H = ransac_xform( feat1, n1, FEATURE_FWD_MATCH, lsq_homog, 4, 0.01,
			homog_xfer_err, 3.0, NULL, NULL );
		if( H )
		{
			IplImage* xformed;
			xformed = cvCreateImage( cvGetSize( img2 ), IPL_DEPTH_8U, 3 );
			cvWarpPerspective( img1, xformed, H,
				CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS,
				cvScalarAll( 0 ) );
			cvNamedWindow( "Xformed", 1 );
			cvShowImage( "Xformed", xformed );
			cvWaitKey( 0 );
			cvReleaseImage( &xformed );
			cvReleaseMat( &H );
		}
	}
	*/

	cvReleaseImage( &stacked );
	kdtree_release( kd_root );
	free( feat1 );
	free( feat2 );
//	return 0;
    return cnt;
}

}
