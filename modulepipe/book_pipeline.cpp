#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <time.h>

#include "pm_headers.h"

using namespace std;

int book_pipeline (char img1_file[512], char img2_file[512], Mat &img_template_new, Mat &registered, vector<bbox> &patch_coord)
{
    cout << img1_file << endl;
    cout << img2_file << endl;

    vector<Point> feat_template_f, feat_target_f;
    Mat img_target_new;

    int feat_num_max = 500;
    int status = 0; // Indicating if there is an error or not, patch_match
    int status_diff = 0; // Indicating if there is an error or not, find_diff


/***************************************************************/
    status = patch_match(img1_file, img2_file, feat_template_f, feat_target_f, feat_num_max, img_template_new, img_target_new);

    if (status == 3)
    {
        return 3;
    }

    if (status!=0)
    {
        return -1;
    }


/***************************************************************/
    thin_plate_spline(img_template_new, img_target_new, registered, feat_template_f, feat_target_f);


/***************************************************************/
    status_diff = find_diff(img_template_new, registered, patch_coord);
    if (status_diff!=0)
    {
        return -1;
    }

    cout << "Collation complete..." << endl;





    return 0;
}
