#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;


struct bbox
{
    /*Bounding box coordinates*/
	int x1;
	int y1;
	int x2;
	int y2;
};


int book_pipeline (char img1_file[512], char img2_file[512], Mat &img_template_new, Mat &registered, vector<bbox> &patch_coord);

int book_pipeline_single (char img1_file[512], char img2_file[512], Mat &img_template_new, Mat &registered, vector<Point> &feat_template_f, vector<Point> &feat_target_f);

/** Registration module */
int patch_match (char img1_file[512], char img2_file[512], vector<Point> &feat_template_f, vector<Point> &feat_target_f, int feat_num_max, Mat &img_template_new, Mat &img_target_new);

void thin_plate_spline(Mat img1, Mat img2, Mat &registered, vector<Point> feat_template_f, vector<Point> feat_target_f);

void refine_match(vector<Point> init_feat_template, vector<Point> init_feat_target, Mat img_template, Mat img_target, vector<Point> &feat_template_f, vector<Point> &feat_target_f);

double compute_jaccard(Mat &roi_template, Mat &roi_target);

void read_array (char filename[512], vector<vector<int> > &feat);

void plot_matches(Mat img1, Mat img2, vector<Point2f> feat1, vector<Point2f> feat2);


/** Difference classifier module */
int find_diff(Mat img1, Mat img2, vector<bbox> &patch_coord);
