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

using namespace cv;
using namespace std;

double compute_jaccard(Mat &roi_template, Mat &roi_target)
{
    Mat roi_or, roi_and;
    int inter=0, uni=0;
    double ratio_temp=0;

    bitwise_and(roi_template, roi_target, roi_or);
    bitwise_not(roi_or, roi_or);
    bitwise_or(roi_template, roi_target, roi_and);
    bitwise_not(roi_and, roi_and);

    inter = (int) sum(roi_and).val[0];
    uni = (int) sum(roi_or).val[0];
    ratio_temp = ((double) inter)/uni;

    return ratio_temp;
}
