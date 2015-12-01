#include "opencv2/core/core.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"
#include "highgui.h"
#include "opencv2/legacy/legacy.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>

#include "pm_headers.h"
#include "match.h"

#include <Eigen/Eigen>
#include <Eigen/Dense>

#define PI 3.141592654

using namespace cv;
using namespace std;
using namespace Eigen;


int patch_match (char img1_file[512], char img2_file[512], vector<Point> &feat_template_f, vector<Point> &feat_target_f, int feat_num_max, Mat &img_template_new, Mat &img_target_new)
{

    clock_t start, finish;
    double elapsed_time;

    IplImage* img1_init, * img2_init;
    IplImage* img1, * img2;
    img1_init = cvLoadImage(img1_file, CV_LOAD_IMAGE_COLOR);
    img2_init = cvLoadImage(img2_file, CV_LOAD_IMAGE_COLOR);

    /** Unable to read images */
    if (!img1_init)
    {
        printf("Unable to load image from %s", img1_file);
        return -1;
    }
    if (!img2_init)
    {
        printf("Unable to load image from %s", img2_file);
        return -1;
    }

    /** Resize image if original image is too large*/
	/** Updated: not resizing for displaying issues*/
    img1 = cvLoadImage(img1_file, CV_LOAD_IMAGE_COLOR);
    img2 = cvLoadImage(img2_file, CV_LOAD_IMAGE_COLOR);
    // int ideal_height = 1000;
    // int height1, width1, height2, width2;

    // if (img1_init->height > ideal_height)
    // {
        // height1 = ideal_height;
        // width1 = (int) (((double) img1_init->width) * height1 / img1_init->height);
       // // printf("height: %d, width: %d\n", height1, width1);
        // img1 = cvCreateImage(cvSize(width1, height1), img1_init->depth, img1_init->nChannels);
        // cvResize(img1_init, img1);
    // }
    // else
    // {
        // height1 = img1_init->height;
        // width1 = img1_init->width;
        // img1 = cvCreateImage(cvSize(width1, height1), img1_init->depth, img1_init->nChannels);
        // cvResize(img1_init, img1);
    // }

    // if (img2_init->height > ideal_height)
    // {
        // height2 = ideal_height;
        // width2 = (int) (((double) img2_init->width) * height2 / img2_init->height);
       // // printf("height: %d, width: %d\n", height2, width2);
        // img2 = cvCreateImage(cvSize(width2, height2), img2_init->depth, img2_init->nChannels);
        // cvResize(img2_init, img2);
    // }
    // else
    // {
        // height2 = img2_init->height;
        // width2 = img2_init->width;
        // img2 = cvCreateImage(cvSize(width2, height2), img2_init->depth, img2_init->nChannels);
        // cvResize(img2_init, img2);
    // }


    /** Compute SIFT feature points */
    int feat_num;
    Point2f* feat_img1, * feat_img2;
    vector<Point2f> feat1, feat2;

    feat_num = compute_feat_num( img1, img2 );
    if (feat_num > feat_num_max)
    {
        feat_num = feat_num_max;
    }
	feat_img1 = (Point2f*) calloc(feat_num, sizeof(Point2f));
	feat_img2 = (Point2f*) calloc(feat_num, sizeof(Point2f));
    compute_sift( img1, img2, &feat_img1, &feat_img2, feat_num_max);

    for (int i=0; i<feat_num; i++)
    {
        feat1.push_back(feat_img1[i]);
        feat2.push_back(feat_img2[i]);
    }
    free(feat_img1);
    free(feat_img2);
//    printf("%d matches found\n", (int) feat1.size());

    /** Use RANSAC to filter false positives */
    Mat mask;
    Mat H = findHomography(feat1, feat2, CV_RANSAC, 5, mask);

//    cout << "mask rows: " << mask.rows << endl;
//    cout << "mask cols: " << mask.cols << endl;

    vector<int> inlier_idx;
    vector<Point2f> feat1_filtered, feat2_filtered;
    for (int i=0; i<mask.rows; i++)
    {
        if ((int) mask.at<uchar>(i,0) > 0)
        {
            inlier_idx.push_back(i);
            feat1_filtered.push_back(feat1[i]);
            feat2_filtered.push_back(feat2[i]);
        }
    }
    feat_num = (int) feat1_filtered.size();
    cout << "inliers: " << feat1_filtered.size() << endl;

    int match_threshold = 100;
    if (feat_num > match_threshold)
    {
        printf("Same document...\n");
    }
    else
    {
        printf("Different document...\n");
        return 3;
    }



    Mat src(img1);
    Mat dst(img2);
//    src = imread(img1_file, 1);
//    dst = imread(img2_file, 1);


//    plot_matches(src, dst, feat1_filtered, feat2_filtered);


    /** Transform images: resize larger image to approximately the same size as smaller image */
    /** Rotate target image to the same orientation as the template image */
    Mat warp_dst;
    Mat warp_mat(2,3,CV_32FC1);
    Point2f srcTri[3], dstTri[3];
    double dist_src=0, dist_dst=0;
    double scaleTri=0;
    int med = (int) feat1_filtered.size()/2;

    srcTri[0] = feat1_filtered[0];
    dstTri[0] = feat2_filtered[0];
    srcTri[1] = feat1_filtered[med];
    dstTri[1] = feat2_filtered[med];
    srcTri[2] = feat1_filtered[feat_num-1];
    dstTri[2] = feat2_filtered[feat_num-1];

    dist_src += pow(srcTri[0].x-srcTri[1].x, 2) + pow(srcTri[0].y-srcTri[1].y, 2);
    dist_src += pow(srcTri[1].x-srcTri[2].x, 2) + pow(srcTri[1].y-srcTri[2].y, 2);
    dist_src += pow(srcTri[2].x-srcTri[0].x, 2) + pow(srcTri[2].y-srcTri[0].y, 2);
    dist_src = sqrt(dist_src);

    dist_dst += pow(dstTri[0].x-dstTri[1].x, 2) + pow(dstTri[0].y-dstTri[1].y, 2);
    dist_dst += pow(dstTri[1].x-dstTri[2].x, 2) + pow(dstTri[1].y-dstTri[2].y, 2);
    dist_dst += pow(dstTri[2].x-dstTri[0].x, 2) + pow(dstTri[2].y-dstTri[0].y, 2);
    dist_dst = sqrt(dist_dst);

    scaleTri = dist_src/dist_dst;

//    cout << "Triangle scale src/dst: " << scaleTri << endl;

    Mat tmp;
    if (scaleTri >= 1)
    {
        tmp = src.clone();
        resize( tmp, src, Size(round(tmp.cols/scaleTri), round(tmp.rows/scaleTri)), INTER_CUBIC );
        for (int i=0; i<feat_num; i++)
        {
            feat1_filtered[i].x = feat1_filtered[i].x / scaleTri;
            feat1_filtered[i].y = feat1_filtered[i].y / scaleTri;
        }
    }
    else
    {
        tmp = dst.clone();
        resize( tmp, dst, Size(round(tmp.cols*scaleTri), round(tmp.rows*scaleTri)), INTER_CUBIC );
        for (int i=0; i<feat_num; i++)
        {
            feat2_filtered[i].x = feat2_filtered[i].x * scaleTri;
            feat2_filtered[i].y = feat2_filtered[i].y * scaleTri;
        }
    }
//    cout << "src row: " << src.rows << " col: " << src.cols << endl;
//    cout << "dst row: " << dst.rows << " col: " << dst.cols << endl;

    // plot_matches(src, dst, feat1_filtered, feat2_filtered);


    /** Compute rotation between two images */
    vector<float> angles;
    double meanX1=0, meanY1=0, meanX2=0, meanY2=0;
    for (int i=0; i<feat_num; i++)
    {
        meanX1 += feat1_filtered[i].x;
        meanY1 += feat1_filtered[i].y;
        meanX2 += feat2_filtered[i].x;
        meanY2 += feat2_filtered[i].y;
    }
    meanX1 = meanX1/feat1_filtered.size();
    meanY1 = meanY1/feat1_filtered.size();
    meanX2 = meanX2/feat1_filtered.size();
    meanY2 = meanY2/feat1_filtered.size();

    for (int i=0; i<feat_num; i++)
    {
        float xa,ya,xb,yb;
        xa = feat1_filtered[i].x-meanX1;
        ya = feat1_filtered[i].y-meanY1;
        xb = feat2_filtered[i].x-meanX2;
        yb = feat2_filtered[i].y-meanY2;

        float mag;
        mag = sqrt(pow(xa,2)+pow(ya,2));
        xa = xa/mag;
        ya = ya/mag;
        mag = sqrt(pow(xb,2)+pow(yb,2));
        xb = xb/mag;
        yb = yb/mag;
        if (xa==0 && ya==0)
            continue;
        if (xb==0 && yb==0)
            continue;

        float cosVal, sinVal;
        cosVal = xa*xb+ya*yb;
        sinVal = ya*xb-xa*yb;
        if (fabs(cosVal-1) < 1e-5)
            cosVal = cosVal/fabs(cosVal);

        float angle;
        if (cosVal>0 && sinVal>0)
            angle = acos(cosVal);
        else if (cosVal==0 && sinVal>0)
            angle = PI/2;
        else if (cosVal<0 &&sinVal>0)
            angle = acos(cosVal);
        else if (cosVal<0 && sinVal==0)
            angle = PI;
        else if (cosVal<0 && sinVal<0)
            angle = acos(-cosVal)+PI;
        else if (cosVal==0 && sinVal<0)
            angle = PI*3/2;
        else if (cosVal>0 && sinVal<0)
            angle = -acos(cosVal);
        else if (cosVal>0 && sinVal==0)
            angle = 0;
        else // cosVal==0, sinVal==0
            continue;

        angles.push_back(angle);
    }

    float minAngle, maxAngle;
    minAngle = angles[0];
    maxAngle = angles[0];
    for (int i=0; i<(int) angles.size(); i++)
    {
        if (angles[i]<minAngle)
            minAngle = angles[i];
        if (angles[i]>maxAngle)
            maxAngle = angles[i];
    }
//    cout << "min angle: " << minAngle << ", " << "max angle: " << maxAngle << endl;

    /** Put all angle values into 10 bins */
    float step = (maxAngle-minAngle) / 10;
    int nAngles[10]={0,0,0,0,0,0,0,0,0,0};
    for (int i=0; i<(int) angles.size(); i++)
    {
        if (angles[i]>=minAngle+step*0 && angles[i]<(minAngle+step*1))
            nAngles[0]+=1;
        else if (angles[i]>=minAngle+step*1 && angles[i]<(minAngle+step*2))
            nAngles[1]+=1;
        else if (angles[i]>=minAngle+step*2 && angles[i]<(minAngle+step*3))
            nAngles[2]+=1;
        else if (angles[i]>=minAngle+step*3 && angles[i]<(minAngle+step*4))
            nAngles[3]+=1;
        else if (angles[i]>=minAngle+step*4 && angles[i]<(minAngle+step*5))
            nAngles[4]+=1;
        else if (angles[i]>=minAngle+step*5 && angles[i]<(minAngle+step*6))
            nAngles[5]+=1;
        else if (angles[i]>=minAngle+step*6 && angles[i]<(minAngle+step*7))
            nAngles[6]+=1;
        else if (angles[i]>=minAngle+step*7 && angles[i]<(minAngle+step*8))
            nAngles[7]+=1;
        else if (angles[i]>=minAngle+step*8 && angles[i]<(minAngle+step*9))
            nAngles[8]+=1;
        else if (angles[i]>=minAngle+step*9 && angles[i]<=maxAngle)
            nAngles[9]+=1;
    }

    int max_cnt, maxIX;
    max_cnt = nAngles[0];
    maxIX=0;
    for (int i=0; i<10; i++)
    {
        if (nAngles[i]>=max_cnt)
        {
            max_cnt = nAngles[i];
            maxIX=i;
        }
    }
    float rot_angle = minAngle + step*((float) maxIX+0.5);

//    cout << "max count: " << max_cnt << endl;
//    cout << "max index: " << maxIX << endl;
//    cout << "rotation angle: " << rot_angle << endl;

    // Transform the img2 and corresponding point coordinates
    float cosVal = cos(rot_angle);
    float sinVal = sin(rot_angle);

    for (int i=0; i<feat_num; i++)
    {
        float tmpX, tmpY;
        tmpX = (feat2_filtered[i].x-(float) dst.cols/2)*cosVal - (feat2_filtered[i].y-(float) dst.rows/2)*sinVal;
        tmpY = (feat2_filtered[i].x-(float) dst.cols/2)*sinVal + (feat2_filtered[i].y-(float) dst.rows/2)*cosVal;
        feat2_filtered[i].x = tmpX+(float) dst.cols/2;
        feat2_filtered[i].y = tmpY+(float) dst.rows/2;
    }


    Mat rot_mat(2, 3, CV_32FC1);
    Mat rot_dst;
    Point center = Point(dst.cols/2, dst.rows/2);
    rot_mat = getRotationMatrix2D(center, -rot_angle*180/PI, 1);
    warpAffine(dst, rot_dst, rot_mat, rot_dst.size());


//    plot_matches(src, rot_dst, feat1_filtered, feat2_filtered);




    /** Densify matching points, using newly generately SURF features */


    /***********************************************************/
    /** Finding feature points using Surf Feature Detector*/

    int minHessian = 400;
    vector<KeyPoint> keypoints;
    vector<Point> points;
    vector<KeyPoint>::iterator it;

    /** Draw keypoints*/
    Mat imga_keypoints;
    SurfFeatureDetector detector(minHessian);
    detector.detect(src, keypoints);


    /** Show keypoints*/
//    printf("keypoints #: %d\n", (int) keypoints.size());
//    drawKeypoints(imga, keypoints, imga_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//    imshow("Imga keypoints", imga_keypoints);
//    waitKey(0);


	/// Select part of newly detected surf points, max_num = 1000
	int max_num = 500;
	int init_feat_num;
    int idx;
    double idx_double = 0.0;
    double idx_step = 0.0;
	vector<Point> keypoints_new;

//	cout << "SURF detector feat size: " << keypoints.size() << endl;

	if ((int) keypoints.size() > max_num)
	{
        Point ptemp;
        init_feat_num = max_num;
        idx_step = ((double) keypoints.size()) / ((double) max_num);

        for(int i=0; i<init_feat_num; i++)
        {
            idx_double = ((double) i)*idx_step;;
            idx = floor(idx_double);
            ptemp.x = (int) (keypoints[idx].pt.x + 0.5);
            ptemp.y = (int) (keypoints[idx].pt.y + 0.5);
            keypoints_new.push_back(ptemp);
        }
	}
	else
	{
        Point ptemp;
        init_feat_num = (int) keypoints.size();

        for (int i=0; i<init_feat_num; i++)
        {
            ptemp.x = (int) (keypoints[i].pt.x + 0.5);
            ptemp.y = (int) (keypoints[i].pt.y + 0.5);
            keypoints_new.push_back(ptemp);
        }
	}

    /** Compute vec = feat2_filtered - feat1_filtered; */
    vector<Point> sift_feat_vec;
    for (int i=0; i<(int) feat1_filtered.size(); i++)
    {
        Point p1temp, p2temp;

        p1temp.x = (int) (feat1_filtered[i].x + 0.5);
        p1temp.y = (int) (feat1_filtered[i].y + 0.5);
        p2temp.x = (int) (feat2_filtered[i].x + 0.5);
        p2temp.y = (int) (feat2_filtered[i].y + 0.5);

        sift_feat_vec.push_back(p2temp-p1temp);
    }

    /// Use nearest neighbor to locate corresponding target feat point
    vector<Point> init_feat_template, init_feat_target;

    for (int i=0; i<init_feat_num; i++)
    {
        Point p1temp, p2temp;
        p1temp = keypoints_new[i];


        int min_diff=0, min_ix=0;
        int dx=0, dy=0;
        int temp=0;


        /*Set initial min value and index*/
        dx = keypoints_new[i].x - ((int) (feat1_filtered[0].x+0.5));
        dy = keypoints_new[i].y - ((int) (feat1_filtered[0].y+0.5));
        min_diff = dx*dx+dy*dy;
        min_ix = 0;

        /**Start comparison*/
        for (int j=1; j<(int) feat1_filtered.size(); j++)
        {
            dx = keypoints_new[i].x - ((int) (feat1_filtered[0].x+0.5));
            dy = keypoints_new[i].y - ((int) (feat1_filtered[0].y+0.5));
            temp = dx*dx+dy*dy;
            if (temp<min_diff)
            {
                min_diff = temp;
                min_ix = j;
            }
        }
        p2temp = p1temp + sift_feat_vec[min_ix];

        if (p2temp.x<rot_dst.cols-1 && p2temp.x>1 && p2temp.y<rot_dst.rows && p2temp.y>1)
        {
            init_feat_template.push_back(p1temp);
            init_feat_target.push_back(p2temp);
        }
        else
            continue;
    }

    for (int i=0; i<(int) feat1_filtered.size(); i++)
    {
        Point p1temp, p2temp;

        p1temp.x = (int) (feat1_filtered[i].x + 0.5);
        p1temp.y = (int) (feat1_filtered[i].y + 0.5);
        p2temp.x = (int) (feat2_filtered[i].x + 0.5);
        p2temp.y = (int) (feat2_filtered[i].y + 0.5);

        init_feat_template.push_back(p1temp);
        init_feat_target.push_back(p2temp);
    }



    /// Show points before refining
    vector<Point2f> feat_template_vis, feat_target_vis;
    for (int i=0; i<(int) init_feat_template.size(); i++)
    {
        Point2f p1temp, p2temp;

        p1temp.x = (float) init_feat_template[i].x;
        p1temp.y = (float) init_feat_template[i].y;
        p2temp.x = (float) init_feat_target[i].x;
        p2temp.y = (float) init_feat_target[i].y;

        feat_template_vis.push_back(p1temp);
        feat_target_vis.push_back(p2temp);
    }

//    cout << "Init feat size: " << init_feat_template.size() << endl;
//    plot_matches(src, rot_dst, feat_template_vis, feat_target_vis);



    /// Refine match based on previous initialization
    vector<Point> feat_template_refine, feat_target_refine;

    Mat imga_gray, imgb_gray;
    Mat imga, imgb;
    cvtColor(src, imga_gray, CV_RGB2GRAY);
    cvtColor(rot_dst, imgb_gray, CV_RGB2GRAY);

    threshold(imga_gray, imga, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    threshold(imgb_gray, imgb, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

//    cout << "image type, imga: " << imga.type() << endl;
//    cout << "image type, imgb: " << imgb.type() << endl;

//    namedWindow("imga", WINDOW_AUTOSIZE);
//    imshow("imga", imga_gray);
//    waitKey(0);
//
//    namedWindow("imgb", WINDOW_AUTOSIZE);
//    imshow("imgb", imgb_gray);
//    waitKey(0);

    start = clock();
    refine_match(init_feat_template, init_feat_target, imga, imgb, feat_template_refine, feat_target_refine);
    finish = clock();
    elapsed_time = (double) (finish-start) / CLOCKS_PER_SEC;
    printf("\nElapsed time for refine matches: %.2f s.\n", elapsed_time);


    feat_template_vis.clear();
    feat_target_vis.clear();

    cout << "Matches after refining: " << feat_template_refine.size() << endl;

    for (int i=0; i<(int) feat_template_refine.size(); i++)
    {
        Point2f p1temp, p2temp;

        p1temp.x = (float) feat_template_refine[i].x;
        p1temp.y = (float) feat_template_refine[i].y;
        p2temp.x = (float) feat_target_refine[i].x;
        p2temp.y = (float) feat_target_refine[i].y;

        feat_template_vis.push_back(p1temp);
        feat_target_vis.push_back(p2temp);
    }
//    plot_matches(src, rot_dst, feat_template_vis, feat_target_vis);


/*****************************************************************/
    /** Replace feat1_filtered, feat2_filtered from here   */

    /** Pad both images */
    double mean_pg1x=0, mean_pg1y=0, mean_pg2x=0, mean_pg2y=0;

    feat_num = (int) feat_template_vis.size();
    for (int i=0; i<feat_num; i++)
    {
        mean_pg1x += feat_template_vis[i].x;
        mean_pg1y += feat_template_vis[i].y;
        mean_pg2x += feat_target_vis[i].x;
        mean_pg2y += feat_target_vis[i].y;
    }
    mean_pg1x = mean_pg1x / feat_num;
    mean_pg1y = mean_pg1y / feat_num;
    mean_pg2x = mean_pg2x / feat_num;
    mean_pg2y = mean_pg2y / feat_num;

    int wl = max(round(mean_pg1x), round(mean_pg2x));
    int wr = max(src.cols-round(mean_pg1x), rot_dst.cols-round(mean_pg2x));
    int ht = max(round(mean_pg1y), round(mean_pg2y));
    int hb = max(src.rows-round(mean_pg1y), rot_dst.rows-round(mean_pg2y));

    int trans_pg1x = wl-round(mean_pg1x);
    int trans_pg1y = ht-round(mean_pg1y);
    int trans_pg2x = wl-round(mean_pg2x);
    int trans_pg2y = ht-round(mean_pg2y);

    int width_new = wl+wr;
    int height_new = ht+hb;

    Mat src_new(Size(width_new, height_new), src.type());
    Mat dst_new(Size(width_new, height_new), src.type());
    for (int i=0; i<src_new.rows; i++)
    {
        for (int j=0; j<src_new.cols; j++)
        {
            src_new.at<Vec3b>(i,j).val[0] = 0;
            src_new.at<Vec3b>(i,j).val[1] = 0;
            src_new.at<Vec3b>(i,j).val[2] = 0;
            dst_new.at<Vec3b>(i,j).val[0] = 0;
            dst_new.at<Vec3b>(i,j).val[1] = 0;
            dst_new.at<Vec3b>(i,j).val[2] = 0;
        }
    }

//    namedWindow("src new", WINDOW_AUTOSIZE);
//    imshow("src new", src_new);
//    waitKey(0);
//
//    namedWindow("dst new", WINDOW_AUTOSIZE);
//    imshow("dst new", dst_new);
//    waitKey(0);

    src.copyTo(src_new(Rect(trans_pg1x, trans_pg1y, src.cols, src.rows)));
    rot_dst.copyTo(dst_new(Rect(trans_pg2x, trans_pg2y, rot_dst.cols, rot_dst.rows)));
    for (int i=0; i<feat_num; i++)
    {
        feat_template_vis[i].x += trans_pg1x;
        feat_template_vis[i].y += trans_pg1y;
        feat_target_vis[i].x += trans_pg2x;
        feat_target_vis[i].y += trans_pg2y;
    }

//    FILE *f1 = fopen("/home/kang/Desktop/tmp/ft1.txt", "w");
//    FILE *f2 = fopen("/home/kang/Desktop/tmp/ft2.txt", "w");
//    for (int i=0; i<feat_num; i++)
//    {
//        fprintf(f1, "%.2f, %.2f\n", feat_template_vis[i].x, feat_template_vis[i].y);
//        fprintf(f2, "%.2f, %.2f\n", feat_target_vis[i].x, feat_target_vis[i].y);
//    }
//    fclose(f1);
//    fclose(f2);

//    plot_matches(src_new, dst_new, feat_template_vis, feat_target_vis);

    img_template_new = src_new.clone();
    img_target_new = dst_new.clone();

//    namedWindow("src new", WINDOW_AUTOSIZE);
//    imshow("src new", src_new);
//    waitKey(0);
//
//    namedWindow("dst new", WINDOW_AUTOSIZE);
//    imshow("dst new", dst_new);
//    waitKey(0);

    Point ft1, ft2;
    for (int i=0; i<(int) feat_template_vis.size(); i++)
    {
        ft1.x = (int) (feat_template_vis[i].x+0.5);
        ft1.y = (int) (feat_template_vis[i].y+0.5);
        ft2.x = (int) (feat_target_vis[i].x+0.5);
        ft2.y = (int) (feat_target_vis[i].y+0.5);
        feat_template_f.push_back(ft1);
        feat_target_f.push_back(ft2);
    }

/***************************************************************/
    /*Write to files*/
//    FILE *ft_template_file = fopen("/home/kang/Desktop/ft1.txt", "w");
//    FILE *ft_target_file = fopen("/home/kang/Desktop/ft2.txt", "w");
//
//    for (int i=0; i<(int) feat1_filtered.size(); i++)
//    {
//        int x1 = feat_template_f[i].x;
//        int y1 = feat_template_f[i].y;
//        int x2 = feat_target_f[i].x;
//        int y2 = feat_target_f[i].y;
//
//        fprintf(ft_template_file, "%d, %d\n", x1, y1);
//        fprintf(ft_target_file, "%d, %d\n", x2, y2);
//    }
//    fclose(ft_template_file);
//    fclose(ft_target_file);

/***************************************************************/


//    namedWindow( "src resized", WINDOW_AUTOSIZE );
//    imshow( "src resized", src );
//    waitKey(0);
//    namedWindow( "dst resized", WINDOW_AUTOSIZE );
//    imshow( "dst resized", dst );
//    waitKey(0);

    return 0;

}


void plot_matches(Mat img1, Mat img2, vector<Point2f> feat1, vector<Point2f> feat2)
{

    int h1, w1, h2, w2, maxH;
    h1 = img1.rows;
    w1 = img1.cols;
    h2 = img2.rows;
    w2 = img2.cols;
    if (h1>h2)
    {
        maxH = h1;
    }
    else
    {
        maxH = h2;
    }

    Mat new_imgs(Size(w1+w2, maxH), img2.type());

    Rect r1(0,0,w1,h1);
    Mat destROI1 = new_imgs(Rect(0,0,w1,h1));
    img1(r1).copyTo(destROI1);
    Rect r2(0,0,w2,h2);
    Mat destROI2 = new_imgs(Rect(w1,0,w2,h2));
    img2(r2).copyTo(destROI2);

    Point pt1, pt2;

    for (int i=0; i<(int) feat1.size(); i++)
    {
        feat2[i].x = feat2[i].x+w1;

        pt1 = Point(round(feat1[i].x), round(feat1[i].y));
        pt2 = Point(round(feat2[i].x), round(feat2[i].y));

        line(new_imgs, pt1, pt2, CV_RGB(255,0,255), 1, 8, 0);
    }

    namedWindow("Matches", WINDOW_AUTOSIZE);
    imshow("Matches", new_imgs);
    waitKey(0);

}
