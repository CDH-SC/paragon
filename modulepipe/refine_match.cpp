#include "opencv2/core/core.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"
#include "highgui.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>

#include "pm_headers.h"

using namespace cv;
using namespace std;

void refine_match(vector<Point> init_feat_template, vector<Point> init_feat_target, Mat img_template, Mat img_target, vector<Point> &feat_template_f, vector<Point> &feat_target_f)
{
    double jaccard_thresh = 0.70;
    int ws = 40;
    int range = 10;
    int step = 1;

    int height1 = img_template.rows;
    int width1 = img_template.cols;
    int height2 = img_target.rows;
    int width2 = img_target.cols;
    int feat_num = (int) init_feat_template.size();
//    cout << "height1: " << height1 << ", " << "width1: " << width1 << endl;
//    cout << "height2: " << height2 << ", " << "width2: " << width2 << endl;

    vector<int> ix_jaccard;
    double jaccard_temp=0, jaccard=0;

    int xa, ya, xb, yb;
    int left, right, top, bottom;
    int temp1, temp2, temp3, temp4;
    int left_a, right_a, top_a, bottom_a;
    int left_b, right_b, top_b, bottom_b;
    int windowH, windowW;
    int min_kk, min_ll, max_kk, max_ll;
    int kk, ll;
    int t2, l2, b2, r2;
    bool isAout=0, isBout=0;
    Mat roiImg_template, roiImg_target;
    Point p1temp, p2temp;
    int counter=0;
    Rect rect_template, rect_target;
//    int assertion_success;

    for (int i=0; i<feat_num; i++)
    {
//        assertion_success = 1;
//        cout << "i: " << i << endl;
//        if (i==1396)
//        {
//            cout << "problem incoming!!!" << endl;
//        }
        jaccard = 0;
        jaccard_temp = 0;
        xa = init_feat_template[i].x;
        ya = init_feat_template[i].y;
        xb = init_feat_target[i].x;
        yb = init_feat_target[i].y;



//        Vec3b intensity = img_template.at<Vec3b>(ya,xa);
//        uchar blue = intensity.val[0];
//        uchar green = intensity.val[1];
//        uchar red = intensity.val[2];
//        cout << "img1(" << ya << "," << xa << "): " << img_template.at<uchar>(ya,xa) << endl;

//        intensity = img_template.at<Vec3b>(ya,xa);
//        blue = intensity.val[0];
//        green = intensity.val[1];
//        red = intensity.val[2];
//        cout << "img2(" << yb << "," << xb << "): " << img_target.at<uchar>(yb,xb) << endl;

        if (xb<=1 || xb >=width2-1 || yb<=1 || yb>=height2-1)
        {
            counter++;
            continue;
        }

        /// Displacement
        left  = max(-ws/2+1, -xa);
        temp1 = max(left, -xb);

        right = min(ws/2, width1-xa-1);
        temp2 = min(right, width2-xb-1);

        top   = max(-ws/2+1, -ya);
        temp3 = max(top, -yb);

        bottom = min(ws/2, height1-ya-1);
        temp4  = min(bottom, height2-yb-1);

        left_a   = xa+temp1;
        right_a  = xa+temp2;
        top_a    = ya+temp3;
        bottom_a = ya+temp4;

        left_b   = xb+temp1;
        right_b  = xb+temp2;
        top_b    = yb+temp3;
        bottom_b = yb+temp4;

        isAout = (right_a<=1 || bottom_a<=1 || left_a>=width1-2 || top_a>=height1-2);
        isBout = (right_b<=1 || bottom_b<=1 || left_b>=width1-2 || top_b>=height1-2);

        if (isAout || isBout)
        {
            counter++;
            continue;
        }

        windowH = bottom_a-top_a+1;
        windowW = right_a-left_a+1;

        rect_template = Rect(left_a, top_a, windowW, windowH);
//        if (left_a>=0 && windowW>=0 && left_a+windowW<=img_template.cols && top_a>=0 && windowH>=0 && top_a+windowH<=img_template.rows)
//            assertion_success = 1;
//        else
//            assertion_success = 0;
//        if (assertion_success == 0)
//        {
//            int xxx=0;
//            cout << "TEMPLATE patch" << endl;
//            cout << left_a << ", " << windowW << ", " << top_a << ", " << windowH << endl;
//            cout << xa << ", " << ya << ", " << xb << ", " << yb << endl;
//            cout << temp1 << ", " << temp2 << ", " << temp3 << ", " << temp4 << endl;
//            cin >> xxx;
//        }

        roiImg_template = img_template(rect_template);

        min_kk = max(top_b-range,0);
        max_kk = min(top_b+range,height2-windowH);
        min_ll = max(left_b-range,0);
        max_ll = min(left_b+range,width2-windowW);


        for (kk=min_kk; kk<max_kk; kk+=step)
        {
            for (ll=min_ll; ll<max_ll; ll+=step)
            {
                t2 = kk;
                l2 = ll;
                b2 = t2+windowH-1;
                r2 = l2+windowW-1;

                rect_target   = Rect(l2, t2, windowW, windowH);
//                if (l2>=0 && windowW>=0 && l2+windowW<=img_target.cols && t2>=0 && windowH>=0 && t2+windowH<=img_target.rows)
//                    assertion_success = 1;
//                else
//                    assertion_success = 0;
//                if (assertion_success == 0)
//                {
//                    int xxx=0;
//                    cout << "target patch" << endl;
//                    cout << l2 << ", " << windowW << ", " << t2 << ", " << windowH << endl;
//                    cin >> xxx;
//                }
                roiImg_target = img_target(rect_target);
                jaccard_temp  = compute_jaccard(roiImg_template, roiImg_target);

                if (jaccard_temp > jaccard)
                {
                    top_b    = t2;
                    bottom_b = b2;
                    left_b   = l2;
                    right_b  = r2;
                    jaccard  = jaccard_temp;
                }
            }
        }
//        printf("jaccard coeff: %.4f\n", jaccard);

//        namedWindow("roiTemplate", WINDOW_AUTOSIZE);
//        imshow("roiTemplate", roiImg_template);
//        waitKey(0);
//
//        rect_target   = Rect(left_b, top_b, windowW, windowH);
//        roiImg_target = img_target(rect_target);
//        namedWindow("roiTarget", WINDOW_AUTOSIZE);
//        imshow("roiTarget", roiImg_target);
//        waitKey(0);

        if (jaccard >= jaccard_thresh)
        {
            p1temp.x = floor((left_a+right_a)/2);
            p1temp.y = floor((top_a+bottom_a)/2);
            p2temp.x = floor((left_b+right_b)/2);
            p2temp.y = floor((top_b+bottom_b)/2);
            feat_template_f.push_back(p1temp);
            feat_target_f.push_back(p2temp);
        }
    }
//    printf("Init number: %d\n", feat_num);
//    printf("Skipped #: %d\n", counter);
//    printf("final num: %d\n", (int) feat_template_f.size());
}
