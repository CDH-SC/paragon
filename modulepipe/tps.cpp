#include "cv.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <time.h>

#include <iostream>

#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/LU>
#include <Eigen/SVD>

using namespace cv;
using namespace std;
using namespace Eigen;

void thin_plate_spline(Mat img1, Mat img2, Mat &registered, vector<Point> feat_template_f, vector<Point> feat_target_f)
{
    size_t feat_num = feat_template_f.size();
    cout << "Final feat num: " << feat_num << endl;

    /*TPS transform*/

    MatrixXd K = MatrixXd::Constant(feat_num, feat_num, 0);
    MatrixXd P = MatrixXd::Constant(feat_num, 3, 0);
    MatrixXd L = MatrixXd::Constant(feat_num+3, feat_num+3, 0);

    MatrixXd tempL2 = MatrixXd::Constant(feat_num+3, 2, 0);
    MatrixXd tform = MatrixXd::Constant(feat_num+3, 2, 0);
    MatrixXd tform_x = MatrixXd::Constant(feat_num+3, 1, 0);
    MatrixXd tform_y = MatrixXd::Constant(feat_num+3, 1, 0);

    MatrixXd pinv_L = MatrixXd::Constant(feat_num+3, feat_num+3, 0);
    double dx=0, dy=0;
    double r=0;
    double r_new = 0;

    for (size_t i=0; i<feat_num; i++)
    {
        for (size_t j=0; j<feat_num; j++)
        {
            dx = (double) (feat_template_f[i].x - feat_template_f[j].x);
            dy = (double) (feat_template_f[i].y - feat_template_f[j].y);
            r = sqrt(pow(dx,2)+pow(dy,2));
            if (r>0.0)
            {
                K(i,j) = r*r*log(r);
            }
            else
            {
                K(i,j) = 0.0;
            }
        }
    }

    for (size_t i=0; i<feat_num; i++)
    {
        P(i,0) = (double) 1;
        P(i,1) = (double) feat_template_f[i].y;
        P(i,2) = (double) feat_template_f[i].x;
    }

    L.block(0,0, feat_num,feat_num) = K;
    L.block(0,feat_num,feat_num,3) = P;
    L.block(feat_num,0,3,feat_num) = P.transpose();
    L.block(feat_num,feat_num,3,3) << 0,0,0,
                                      0,0,0,
                                      0,0,0;
    for (size_t i=0; i<feat_num; i++)
    {
        tempL2(i,0) = (double) feat_target_f[i].y;
        tempL2(i,1) = (double) feat_target_f[i].x;
    }
    tempL2.block(feat_num,0,3,2) << 0,0,
                                    0,0,
                                    0,0;
/***************************************************************/
    clock_t start, finish;
    double elapsed_time;
//    printf("\nStarting solving equation...\n");
    start = clock();
    L.svd().solve(tempL2, &tform);
    finish = clock();
    elapsed_time = (double) (finish-start) / CLOCKS_PER_SEC;
    printf("Elapsed time for solving equation: %2.f seconds\n", elapsed_time);
/***************************************************************/

    /*Transform to get registered image*/
//    Mat img1, img2;
//    img1 = imread(img1_file, 1);
//    if (!img1.data)
//    {
//        cout << "Image " << img1_file << " NOT found!";
//        return;
//    }
//    img2 = imread(img2_file, 1);
//    if (!img2.data)
//    {
//        cout << "Image " << img2_file << " NOT found!";
//        return;
//    }
//    resize(img1, img1, Size(width, height));
//    resize(img2, img2, Size(width, height));
//    int height = img1.rows;
//    int width  = img1.cols;

//    Mat registered(Size(width, height), img2.type());
    registered = img2.clone();


    MatrixXd tpsW = tform.block(0,0,feat_num,2);
    MatrixXd tpsT = tform.block(feat_num,0,1,2);
    MatrixXd tpsA = tform.block(feat_num+1,0,2,2);
    MatrixXd val0 = MatrixXd::Constant(1,2,0);
    MatrixXd val = MatrixXd::Constant(1,2,0);
    MatrixXd ij_t = MatrixXd::Constant(1,2,0);
    MatrixXd tempij = MatrixXd::Constant(feat_num,2,0);
    MatrixXd test = MatrixXd::Constant(feat_num,2,0);
    MatrixXd rtest = MatrixXd::Constant(feat_num,2,0);
    MatrixXd rcomp = MatrixXd::Constant(feat_num,1,0);
    MatrixXd testmul = MatrixXd::Constant(feat_num,2,0);
    MatrixXd newtpsW = MatrixXd::Constant(feat_num,2,0);


    MatrixXd landmark_template = MatrixXd::Constant(feat_num,2,0);
    for (size_t k=0; k<feat_num; k++)
    {
        landmark_template(k,0) = feat_template_f[k].y;
        landmark_template(k,1) = feat_template_f[k].x;
    }

    int x=0, y=0;

//    printf("\nimg1 height: %d, width: %d\n", img1.rows, img1.cols);
//    printf("img2 height: %d, width: %d\n\n", img2.rows, img2.cols);

    /** Timing */
    start = clock();

    for (int i=0; i<img1.rows; i++)
    {
//        if (i%100==0)
//        {
//            printf("row #: %d\n", i);
//        }
        for (int j=0; j<img1.cols; j++)
        {

            val = tpsT;
            ij_t << i+1,j+1;
            val = val + ij_t * tpsA;
            val0 = val;

            /*Old way*/
            for (size_t k=0; k<feat_num; k++)
            {
                dx = (double) (j + 1 - feat_template_f[k].x);
                dy = (double) (i + 1 - feat_template_f[k].y);
                /*Plus 1 to compare with MATLAB result*/

                r = sqrt(pow(dx,2)+pow(dy,2));
                r_new = (r>0) ? r*r*log(r) : 0;
                val(0,0) += tpsW(k,0)*r_new;
                val(0,1) += tpsW(k,1)*r_new;

            }

            x = (int) (val(0,0) - 1 +0.5f);
            y = (int) (val(0,1) - 1 +0.5f);
            /*Minus 1 to compare with MATLAB result*/
            if (x>=0 && x<img1.rows && y>=0 && y<img1.cols)
            {
                registered.at<Vec3b>(i,j) = img2.at<Vec3b>(x,y);
            }
            else
            {
                x = -2;
                y = -2;
                registered.at<Vec3b>(i,j).val[0] = 0;
                registered.at<Vec3b>(i,j).val[1] = 0;
                registered.at<Vec3b>(i,j).val[2] = 0;
            }

        }

    }

    finish = clock();
    elapsed_time = (double) (finish-start) / CLOCKS_PER_SEC;
    printf("Elapsed time for computing new image: %2.f seconds\n", elapsed_time);

/***************************************************************/


//    imshow("img1", img1);
//    imshow("img2", img2);
//    imshow("registered", registered);
//    waitKey(0);



}
