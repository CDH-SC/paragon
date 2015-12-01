#include "cv.h"
#include "highgui.h"
#include "pm_headers.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include <iostream>

using namespace cv;
using namespace std;


void FindBlobs(const Mat &binary, vector <vector<Point2i> > &blobs);
double calc_ssim(IplImage * img1_temp, IplImage * img2_temp);
double calc_corr(IplImage * img1_temp, IplImage * img2_temp);
void merge_bbox(int j, vector<int> &temp_list, vector<int> &visited_list, int **connection_mat, int mat_size);
void removeBWnoise(Mat &binary);

int find_diff(Mat img1, Mat img2, vector<bbox> &patch_coord)
{
    IplImage patch1_gray_ipl, patch2_gray_ipl;

    Mat img1_gray, img2_gray;
    Mat img1_bw, img2_bw;
    Mat diff_bw;
    int height=0, width=0;

    /*Read images*/
//    img1 = imread(img1_file, 1);
//    if (!img1.data)
//    {
//        cout << "Image " << img1_file << " NOT found!";
//        return -1;
//    }
//    img2 = imread(img2_file, 1);
//    if (!img2.data)
//    {
//        cout << "Image " << img2_file << " NOT found!";
//        return -1;
//    }

    /*Check image size*/
    if (img1.rows != img2.rows || img1.cols != img2.cols)
    {
        cout << "Images are not of the same size!\n" << endl;
        return -1;
    }
    else
    {
        height = img1.rows;
        width = img1.cols;
    }

    /*Convert to grayscale image*/
    cvtColor(img1, img1_gray, CV_BGR2GRAY);
    cvtColor(img2, img2_gray, CV_BGR2GRAY);

//    cvtColor(img1, img1_gray, CV_RGB2GRAY);
//    cvtColor(img2, img2_gray, CV_RGB2GRAY);

    /*Convert to inverse of binary image*/
    threshold(img1_gray, img1_bw, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    threshold(img2_gray, img2_bw, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

    removeBWnoise(img1_bw);
    removeBWnoise(img2_bw);


    /*XOR operation on input images to get difference image*/
    bitwise_xor(img1_bw, img2_bw, diff_bw);


//    imshow("img1_bw", img1_bw);
//    imshow("img2_bw", img2_bw);
//    imshow("diff_bw", diff_bw);
//    waitKey();

    /**Find blobs*/
    vector<vector<Point2i> > img1_blobs;
    vector<vector<Point2i> > img2_blobs;
    vector<vector<Point2i> > blobs;
    size_t max_blob_area=0;

    FindBlobs(img1_bw, img1_blobs);
    FindBlobs(img2_bw, img2_blobs);
    FindBlobs(diff_bw, blobs);

    for (size_t m=0; m<blobs.size(); m++)
    {
        if (max_blob_area < blobs[m].size())
            max_blob_area = blobs[m].size();
    }

    /*********************************************************************/
    /*Image label*/
    int **img1_label, ** img2_label;

    img1_label = (int**) calloc(height, sizeof(int*));
    img2_label = (int**) calloc(height, sizeof(int*));
    for (int i=0; i<height; i++)
    {
        img1_label[i] = (int*) calloc(width, sizeof(int));
        img2_label[i] = (int*) calloc(width, sizeof(int));
    }

    /*Compute bounding boxes for labeled img1_bw and img2_bw, n-by-4 array*/
    /*Label start from 1, let 0 be empty*/
    int **bbox_img1, **bbox_img2;

    bbox_img1 = (int**) calloc(img1_blobs.size()+1, sizeof(int*));
    for (size_t m=0; m<img1_blobs.size()+1; m++)
    {
        bbox_img1[m] = (int*) calloc(4, sizeof(int));
    }
    bbox_img2 = (int**) calloc(img2_blobs.size()+1, sizeof(int*));
    for (size_t m=0; m<img2_blobs.size()+1; m++)
    {
        bbox_img2[m] = (int*) calloc(4, sizeof(int));
    }
    /*********************************************************************/

    /**Assign labels to image 1 and image 2, record bbox coordinates*/
    /*Labels start from 1*/
    for (size_t m=0; m<img1_blobs.size(); m++)
    {
        int x=0, y=0;
        int x1=width, y1=height, x2=0, y2=0;
        for (size_t n=0; n<img1_blobs[m].size(); n++)
        {
            x = img1_blobs[m][n].x;
            y = img1_blobs[m][n].y;

            x1 = x<x1 ? x : x1;
            y1 = y<y1 ? y : y1;
            x2 = x>x2 ? x : x2;
            y2 = y>y2 ? y : y2;

            img1_label[y][x] = (int) (m+1);
        }
        bbox_img1[m+1][0] = x1;
        bbox_img1[m+1][1] = y1;
        bbox_img1[m+1][2] = x2;
        bbox_img1[m+1][3] = y2;
    }

    /*Labels start from 1*/
    for (size_t m=0; m<img2_blobs.size(); m++)
    {
        int x=0, y=0;

        int x1=width, y1=height, x2=0, y2=0;
        for (size_t n=0; n<img2_blobs[m].size(); n++)
        {
            x = img2_blobs[m][n].x;
            y = img2_blobs[m][n].y;

            x1 = x<x1 ? x : x1;
            y1 = y<y1 ? y : y1;
            x2 = x>x2 ? x : x2;
            y2 = y>y2 ? y : y2;

            img2_label[y][x] = (int) (m+1);
        }
        bbox_img2[m+1][0] = x1;
        bbox_img2[m+1][1] = y1;
        bbox_img2[m+1][2] = x2;
        bbox_img2[m+1][3] = y2;
    }

    /*********************************************************************/
    /** Compare bboxes between img1 and img2 containing each labeled difference */
    size_t thresh = 4;
    int counter=0;  /*counter of blobs with area larger than threshold*/

    for (size_t m=0; m<blobs.size(); m++)
    {
        size_t area = blobs[m].size();
        if (area < thresh)
        {
            continue;
        }
        counter++;
    }

    /*-------------------------------------------------------------------------------------*/
    vector<int> visited_img1_label;
    vector<int> visited_img2_label;
    int current_label = 0;
    int temp_label = 0;
    bool isedge = false;
    Mat patch1_gray, patch2_gray, patch1_bw, patch2_bw;
    bbox patch_coord_temp = {0, 0, 0, 0};
    vector<bbox> patch_coord_o;

    int max_x=0, max_y=0;
    counter = 0;
    for (size_t m=0; m<blobs.size(); m++)
    {
        int x=0, y=0;
        size_t area = blobs[m].size();

        if (area < thresh)
        {
            continue;
        }
        int x1=0, y1=0, x2=0, y2=0;
        Rect bbox_patch;
        Mat patch_and, patch_or;
        int inter, uni;
        double match_ratio = 0.0;
        double ssim_score = 0.0;
        double corr_score = 0.0;

        /*----------------------------------------*/
        /*Check if the difference blob lies on the edge, if so, skip it*/
        isedge = false;

        for (size_t n=0; n<blobs[m].size(); n++)
        {
            x = blobs[m][n].x;
            y = blobs[m][n].y;
            if (x==0 || x==width-1 ||y==0 || y==height-1)
            {
                isedge = true;
                break;
            }
        }

        if (isedge)
            continue;
        /*----------------------------------------*/

        for (size_t n=0; n<blobs[m].size(); n++)
        {
            x = blobs[m][n].x;
            y = blobs[m][n].y;
            if (x>max_x)
                max_x = x;
            if (y>max_y)
                max_y = y;

            temp_label = img1_label[y][x];
            current_label = 0;

//            size_t area_t;
             /*difference is from img1_bw*/
            if (temp_label !=0)
            {
                current_label = img1_label[y][x];

                if (std::find(visited_img1_label.begin(), visited_img1_label.end(), current_label) != visited_img1_label.end()) {
                    continue;
                    }

                visited_img1_label.push_back(current_label);
                x1 = bbox_img1[current_label][0];
                y1 = bbox_img1[current_label][1];
                x2 = bbox_img1[current_label][2];
                y2 = bbox_img1[current_label][3];

//                area_t = img1_blobs[current_label-1].size();
            }
            /*difference is from img2_bw*/
            else
            {
                current_label = img2_label[y][x];
                if (std::find(visited_img2_label.begin(), visited_img2_label.end(), current_label) != visited_img2_label.end()) {
                    continue;
                    }

                visited_img2_label.push_back(current_label);
                x1 = bbox_img2[current_label][0];
                y1 = bbox_img2[current_label][1];
                x2 = bbox_img2[current_label][2];
                y2 = bbox_img2[current_label][3];

//                area_t = img2_blobs[current_label-1].size();
            }

            if (x1==x2 || y1==y2)
            {
                continue;
            }

            bbox_patch = Rect(x1, y1, x2-x1, y2-y1);
            patch1_gray = img1_gray(bbox_patch);
            patch2_gray = img2_gray(bbox_patch);
            patch1_bw = img1_bw(bbox_patch);
            patch2_bw = img2_bw(bbox_patch);


            int h = patch1_gray.rows;
            int w = patch1_gray.cols;
            CvScalar sum1, sum2;
            sum1 = sum(patch1_bw);
            sum2 = sum(patch2_bw);
            double ink_ratio1, ink_ratio2;

            ink_ratio1 = ((double) sum1.val[0]) / h / w / 255;
            ink_ratio2 = ((double) sum2.val[0]) / h / w / 255;
//            cout << "Ink ratio: " << ink_ratio1 << ", " << ink_ratio2 << endl;

            /*Cases when they almost certainly false positives*/
            /*Most likely to be just white space*/
            if (h*w > 1000 || h*w < 15)
                continue;
            if (ink_ratio1 < 0.1 && ink_ratio2 < 0.1)
                continue;
            /*Resize patches, if h or w is less than 11, ssim index is Inf*/
            if (h<11)
            {
                h = 11;
                resize(patch1_gray, patch1_gray, cvSize(w,h));
                resize(patch2_gray, patch2_gray, cvSize(w,h));
                resize(patch1_bw, patch1_bw, cvSize(w,h));
                resize(patch2_bw, patch2_bw, cvSize(w,h));

//                imshow("patch1_gray", patch1_gray);
//                imshow("patch2_gray", patch2_gray);
//                imshow("patch1_bw", patch1_bw);
//                imshow("patch2_bw", patch2_bw);
//                waitKey(0);
            }
            if (w<11)
            {
                w = 11;
                resize(patch1_gray, patch1_gray, cvSize(w,h));
                resize(patch2_gray, patch2_gray, cvSize(w,h));
                resize(patch1_bw, patch1_bw, cvSize(w,h));
                resize(patch2_bw, patch2_bw, cvSize(w,h));

//                imshow("patch1_gray", patch1_gray);
//                imshow("patch2_gray", patch2_gray);
//                imshow("patch1_bw", patch1_bw);
//                imshow("patch2_bw", patch2_bw);
//                waitKey(0);
            }

            bitwise_and(patch1_bw, patch2_bw, patch_and);
            bitwise_or(patch1_bw, patch2_bw, patch_or);
            inter = (int) sum(patch_and).val[0];
            uni = (int) sum(patch_or).val[0];
            match_ratio = ((double) inter) / uni;

            /**Create patch1 and patch2*/

            patch1_gray_ipl = patch1_gray;
            patch2_gray_ipl = patch2_gray;

//            namedWindow("patch1_gray", CV_WINDOW_AUTOSIZE);
//            cvShowImage("patch1_gray", &patch1_gray_ipl);
//            namedWindow("patch2_gray", CV_WINDOW_AUTOSIZE);
//            cvShowImage("patch2_gray", &patch2_gray_ipl);
//            waitKey(0);

            ssim_score = calc_ssim(&patch1_gray_ipl, &patch2_gray_ipl);
            corr_score = calc_corr(&patch1_gray_ipl, &patch2_gray_ipl);

            int max_vote = (ssim_score<0.5) + (corr_score<0.5) + (match_ratio<0.5);

//            if (max_vote >= 2)
//            {
//                patch_coord_temp.x1 = x1;
//                patch_coord_temp.x2 = x2;
//                patch_coord_temp.y1 = y1;
//                patch_coord_temp.y2 = y2;
//
//                patch_coord_o.push_back(patch_coord_temp);
//            }

            int bbox_range = 5;
            if (max_vote >= 2)
            {
                patch_coord_temp.x1 = (x1-bbox_range)>0 ? (x1-bbox_range) : 1;
                patch_coord_temp.x2 = (x2+bbox_range)<width ? (x2+bbox_range) : width-1;
                patch_coord_temp.y1 = (y1-bbox_range)>0 ? (y1-bbox_range) : 1;
                patch_coord_temp.y2 = (y2+bbox_range)<height ? (y2+bbox_range) : height-1;

                patch_coord_o.push_back(patch_coord_temp);
            }
            counter++;
        }
    }


    /*************************************************************************************/
    /*Merge overlapping bounding boxes*/
    int **connection_mat = (int**) calloc(patch_coord_o.size(), sizeof(int*));
    int x1a=0, y1a=0, x2a=0, y2a=0;     /*First bounding box coordinates*/
    int x1b=0, y1b=0, x2b=0, y2b=0;     /*Second bounding box coordinates*/

    for (size_t i=0; i<patch_coord_o.size(); i++)
    {
        connection_mat[i] = (int*) calloc(patch_coord_o.size(), sizeof(int));
    }

    for (size_t i=0; i<patch_coord_o.size(); i++)
    {
        x1a = patch_coord_o[i].x1;
        y1a = patch_coord_o[i].y1;
        x2a = patch_coord_o[i].x2;
        y2a = patch_coord_o[i].y2;

        for (size_t j=0; j<patch_coord_o.size(); j++)
        {
            x1b = patch_coord_o[j].x1;
            y1b = patch_coord_o[j].y1;
            x2b = patch_coord_o[j].x2;
            y2b = patch_coord_o[j].y2;

            /*Bounding box not overlapping*/
            if (x1a>x2b || x1b>x2a || y1a>y2b || y1b>y2a)
            {
                connection_mat[i][j] = 0;
            }
            else
            {
                connection_mat[i][j] = 1;
            }
        }

    }

    for (size_t i=0; i<patch_coord_o.size(); i++)
    {
        connection_mat[i][i] = 0;
    }

    vector<vector<int> > merge_list;
    vector<int> temp_list;
    vector<int> visited_list;
    int mat_size = (int) patch_coord_o.size();

    for (int i=0; i<(int) patch_coord_o.size(); i++)
    {
        temp_list.clear();
        if (find(visited_list.begin(), visited_list.end(), i) != visited_list.end())
        {
            continue;
        }
        else
        {
            temp_list.push_back(i);
            visited_list.push_back(i);
        }

        for (int j=0; j<(int) patch_coord_o.size(); j++)
        {
            if (connection_mat[i][j]==1)
            {
                merge_bbox(j, temp_list, visited_list, connection_mat, mat_size);
            }
        }

        merge_list.push_back(temp_list);
    }

    /****************************************************************************/
    /*Create new bounding boxes according to merged list*/
    int x1_min=0, x2_max=0, y1_min=0, y2_max=0;
    bbox temp = {0,0,0,0};
    int idx = 0;

    for (size_t i=0; i<merge_list.size(); i++)
    {
        for (size_t j=0; j<merge_list[i].size(); j++)
        {
            if (j==0)
            {
                idx = merge_list[i][j];
                x1_min = patch_coord_o[idx].x1;
                x2_max = patch_coord_o[idx].x2;
                y1_min = patch_coord_o[idx].y1;
                y2_max = patch_coord_o[idx].y2;
            }
            else
            {
                idx = merge_list[i][j];
                if (x1_min > patch_coord_o[idx].x1)
                    x1_min = patch_coord_o[idx].x1;
                if (x2_max < patch_coord_o[idx].x2)
                    x2_max = patch_coord_o[idx].x2;
                if (y1_min > patch_coord_o[idx].y1)
                    y1_min = patch_coord_o[idx].y1;
                if (y2_max < patch_coord_o[idx].y2)
                    y2_max = patch_coord_o[idx].y2;
            }
        }
        temp.x1 = x1_min;
        temp.x2 = x2_max;
        temp.y1 = y1_min;
        temp.y2 = y2_max;

        patch_coord.push_back(temp);
    }

    /**Free connection matrix*/
    for (size_t i=0; i<patch_coord_o.size(); i++)
    {
        free(connection_mat[i]);
    }
    free(connection_mat);
    /*************************************************************************************/

    /*********************************************************************/
    /*Free dynamic arrays*/

    for (size_t m=0; m<img1_blobs.size(); m++)
    {
        free(bbox_img1[m]);
    }
    free(bbox_img1);
    for (size_t m=0; m<img2_blobs.size(); m++)
    {
        free(bbox_img2[m]);
    }
    free(bbox_img2);

    for (int i=0; i<height; i++)
    {
        free(img1_label[i]);
        free(img2_label[i]);
    }
    free(img1_label);
    free(img2_label);

    /*********************************************************************/
//    cvReleaseImage(&patch1_gray_ipl);
//    cvReleaseImage(&patch2_gray_ipl);

	return 0;
}




/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/

/** Subroutines: FindBlobs, calc_ssim */



void FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs)
{
    blobs.clear();

    // Fill the label_image with the blobs
    // 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground

    cv::Mat label_image;
    binary.convertTo(label_image, CV_32SC1);

    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {
            if(row[x] != 255) {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 4);

            std::vector <cv::Point2i> blob;

            for(int i=rect.y; i < (rect.y+rect.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(cv::Point2i(j,i));
                }
            }

            blobs.push_back(blob);

            label_count++;
        }
    }
}


double calc_ssim (IplImage * img1_temp, IplImage * img2_temp)
{

	if (img1_temp->height != img2_temp->height || img1_temp->width != img2_temp->width)
	{
        cout << endl;
        cout << "img1_temp(width, height): " << img1_temp->width << "," << img1_temp->height;
        cout << endl;
        cout << "img2_temp(width, height): " << img2_temp->width << "," << img2_temp->height;
        cout << endl;
//        cout << "Input images dimension not equal, cannot perform correlation calculation! Please check!" << endl;
        return -1;
	}
	else
	{
//        cout << "img1_temp(width, height): " << img1_temp->width << "," << img1_temp->height;
	}

	double C1 = 6.5025, C2 = 58.5225;

	IplImage
		*img1=NULL, *img2=NULL, *img1_img2=NULL,
		*img1_sq=NULL, *img2_sq=NULL,
		*mu1=NULL, *mu2=NULL,
		*mu1_sq=NULL, *mu2_sq=NULL, *mu1_mu2=NULL,
		*sigma1_sq=NULL, *sigma2_sq=NULL, *sigma12=NULL,
		*ssim_map=NULL, *temp1=NULL, *temp2=NULL, *temp3=NULL;


	/***************************** INITS **********************************/
	int x=img1_temp->width, y=img1_temp->height;
	int nChan=img1_temp->nChannels, d=IPL_DEPTH_32F;
	CvSize size = cvSize(x, y);

	img1 = cvCreateImage( size, d, nChan);
	img2 = cvCreateImage( size, d, nChan);

	cvConvert(img1_temp, img1);
	cvConvert(img2_temp, img2);

	img1_sq = cvCreateImage( size, d, nChan);
	img2_sq = cvCreateImage( size, d, nChan);
	img1_img2 = cvCreateImage( size, d, nChan);

	cvPow( img1, img1_sq, 2 );
	cvPow( img2, img2_sq, 2 );
	cvMul( img1, img2, img1_img2, 1 );

	mu1 = cvCreateImage( size, d, nChan);
	mu2 = cvCreateImage( size, d, nChan);

	mu1_sq = cvCreateImage( size, d, nChan);
	mu2_sq = cvCreateImage( size, d, nChan);
	mu1_mu2 = cvCreateImage( size, d, nChan);


	sigma1_sq = cvCreateImage( size, d, nChan);
	sigma2_sq = cvCreateImage( size, d, nChan);
	sigma12 = cvCreateImage( size, d, nChan);

	temp1 = cvCreateImage( size, d, nChan);
	temp2 = cvCreateImage( size, d, nChan);
	temp3 = cvCreateImage( size, d, nChan);

	ssim_map = cvCreateImage( size, d, nChan);
	/*************************** END INITS **********************************/

	//////////////////////////////////////////////////////////////////////////
	// PRELIMINARY COMPUTING
	cvSmooth( img1, mu1, CV_GAUSSIAN, 11, 11, 1.5 );
	cvSmooth( img2, mu2, CV_GAUSSIAN, 11, 11, 1.5 );

	cvPow( mu1, mu1_sq, 2 );
	cvPow( mu2, mu2_sq, 2 );
	cvMul( mu1, mu2, mu1_mu2, 1 );


	cvSmooth( img1_sq, sigma1_sq, CV_GAUSSIAN, 11, 11, 1.5 );
	cvAddWeighted( sigma1_sq, 1, mu1_sq, -1, 0, sigma1_sq );

	cvSmooth( img2_sq, sigma2_sq, CV_GAUSSIAN, 11, 11, 1.5 );
	cvAddWeighted( sigma2_sq, 1, mu2_sq, -1, 0, sigma2_sq );

	cvSmooth( img1_img2, sigma12, CV_GAUSSIAN, 11, 11, 1.5 );
	cvAddWeighted( sigma12, 1, mu1_mu2, -1, 0, sigma12 );


	//////////////////////////////////////////////////////////////////////////
	// FORMULA

	// (2*mu1_mu2 + C1)
	cvScale( mu1_mu2, temp1, 2 );
	cvAddS( temp1, cvScalarAll(C1), temp1 );

	// (2*sigma12 + C2)
	cvScale( sigma12, temp2, 2 );
	cvAddS( temp2, cvScalarAll(C2), temp2 );

	// ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
	cvMul( temp1, temp2, temp3, 1 );

	// (mu1_sq + mu2_sq + C1)
	cvAdd( mu1_sq, mu2_sq, temp1 );
	cvAddS( temp1, cvScalarAll(C1), temp1 );

	// (sigma1_sq + sigma2_sq + C2)
	cvAdd( sigma1_sq, sigma2_sq, temp2 );
	cvAddS( temp2, cvScalarAll(C2), temp2 );

	// ((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2))
	cvMul( temp1, temp2, temp1, 1 );

	// ((2*mu1_mu2 + C1).*(2*sigma12 + C2))./((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2))
	cvDiv( temp3, temp1, ssim_map, 1 );


	CvScalar index_scalar = cvAvg( ssim_map );

	// through observation, there is approximately
	// 1% error max with the original matlab program

//	cout << "(R, G & B SSIM index)" << endl ;
//	cout << index_scalar.val[2] * 100 << "%" << endl ;
//	cout << index_scalar.val[1] * 100 << "%" << endl ;
//	cout << index_scalar.val[0] * 100 << "%" << endl ;

    double ssim_score = (double) index_scalar.val[0];

    cvReleaseImage(&img1);
    cvReleaseImage(&img2);
    cvReleaseImage(&img1_img2);
    cvReleaseImage(&img1_sq);
    cvReleaseImage(&img2_sq);
    cvReleaseImage(&mu1);
    cvReleaseImage(&mu2);
    cvReleaseImage(&mu1_sq);
    cvReleaseImage(&mu2_sq);
    cvReleaseImage(&mu1_mu2);
    cvReleaseImage(&sigma1_sq);
    cvReleaseImage(&sigma2_sq);
    cvReleaseImage(&sigma12);
    cvReleaseImage(&ssim_map);
    cvReleaseImage(&temp1);
    cvReleaseImage(&temp2);
    cvReleaseImage(&temp3);

	return ssim_score;
}


double calc_corr (IplImage * img1_temp, IplImage * img2_temp)
{

	if (img1_temp->height != img2_temp->height || img1_temp->width != img2_temp->width)
	{
        cout << endl;
        cout << "img1_temp(width, height): " << img1_temp->width << "," << img1_temp->height;
        cout << endl;
        cout << "img2_temp(width, height): " << img2_temp->width << "," << img2_temp->height;
        cout << endl;
//        cout << "Input images dimension not equal, cannot perform correlation calculation! Please check!" << endl;
        return -1;
	}

	double corr_score = 1.0;
	IplImage *img1=NULL, *img2=NULL, *img1_sub=NULL, *img2_sub=NULL;
	IplImage *product=NULL, *img1_sq=NULL, *img2_sq=NULL;
	CvScalar img1_avg, img2_avg;
	CvScalar product_avg;
	CvScalar img1_sq_avg, img2_sq_avg;
	double temp1=1.0, temp2=1.0, temp3=1.0;

	/***************************** INITS **********************************/
	int x=img1_temp->width, y=img1_temp->height;
	int nChan=img1_temp->nChannels, d=IPL_DEPTH_32F;
	CvSize size = cvSize(x, y);

	img1 = cvCreateImage( size, d, nChan);
	img2 = cvCreateImage( size, d, nChan);
	img1_sub = cvCreateImage( size, d, nChan);
	img2_sub = cvCreateImage( size, d, nChan);
	img1_sq = cvCreateImage(size, d, nChan);
	img2_sq = cvCreateImage(size, d, nChan);
	product = cvCreateImage(size, d, nChan);


	cvConvert(img1_temp, img1);
	cvConvert(img2_temp, img2);

	img1_avg = cvAvg(img1);
	img2_avg = cvAvg(img2);

	cvSubS(img1, img1_avg, img1_sub);
	cvSubS(img2, img2_avg, img2_sub);

	cvPow(img1_sub, img1_sq, 2);
	cvPow(img2_sub, img2_sq, 2);

	cvMul(img1_sub, img2_sub, product, 1);

	product_avg = cvAvg(product);
	img1_sq_avg = cvAvg(img1_sq);
	img2_sq_avg = cvAvg(img2_sq);

	temp1 = (double) product_avg.val[0];
	temp2 = (double) img1_sq_avg.val[0];
	temp3 = (double) img2_sq_avg.val[0];

	corr_score = temp1/ sqrt(temp2) / sqrt(temp3);
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img1_sq);
	cvReleaseImage(&img2_sq);
	cvReleaseImage(&img1_sub);
	cvReleaseImage(&img2_sub);
	cvReleaseImage(&product);

	return corr_score;
}


void merge_bbox(int j, vector<int> &temp_list, vector<int> &visited_list, int **connection_mat, int mat_size)
{

    if (find(visited_list.begin(), visited_list.end(), j) != visited_list.end())
    {
        return;
    }
    else
    {
        temp_list.push_back(j);
        visited_list.push_back(j);
    }

    for (int k=0; k<mat_size; k++)
    {
        if (connection_mat[j][k]==1)
        {
            merge_bbox(k, temp_list, visited_list, connection_mat, mat_size);
        }
    }

}


void removeBWnoise(Mat &binary)
{
    vector<vector<Point2i> > blobs;
    size_t area_thresh = 10;
    int x = 0, y = 0;

    FindBlobs(binary, blobs);

    for (size_t m=0; m<blobs.size(); m++)
    {
        if (blobs[m].size() < area_thresh)
        {
            for (size_t n=0; n<blobs[m].size(); n++)
            {
                x = blobs[m][n].x;
                y = blobs[m][n].y;

                binary.at<uchar>(y,x) = 0;
            }
        }
    }
}
