#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <time.h>

#include "pm_headers.h"

using namespace std;

int main(int argc, char** argv)
{

    char img1_file[512] = "/home/kang/paragon/significant_differences/batches_crop/4/16/FQ190_UIUC_1_Bb7.jpg";
    char img2_file[512] = "/home/kang/paragon/significant_differences/batches_crop/4/16/FQ190_WU_1_Bb7.jpg";
//    char img2_file[512] = "/home/kang/paragon/significant_differences/batches_crop/2/3/Theatre_Bod_1_E4v.jpg";

//    char img1_file[512] = "/home/kang/Dropbox/paragon/demo_Aug_2014/im1.jpg";
//    char img2_file[512] = "/home/kang/Dropbox/paragon/demo_Aug_2014/im5.jpg";

//    char img1_file[512] = "/home/kang/paragon/data/wustl-crop/5_Ff2/5_Ff2_a_FQ190_UIUC_1.jpeg";
//    char img2_file[512] = "/home/kang/paragon/data/wustl-crop/5_Ff2/5_Ff2_b_FQ190_HRH_1.jpeg";

    Mat img_template_new, registered;
    vector<bbox> patch_coord;

    int status = book_pipeline(img1_file, img2_file, img_template_new, registered, patch_coord);

    if (status == 3)
        cout << "Different document..." << endl;

    if (status != 0)
    {
        cout << "Process terminated abnormally.." << endl;
        return -1;
    }

/***************************************************************/
    /** Write to files */

    char file_path_img1[512] = "/home/kang/Desktop/tmp/img_template_new.jpg";
    char file_path_reg[512]  = "/home/kang/Desktop/tmp/registered.jpg";
    char patch_coord_file[512] = "/home/kang/Desktop/tmp/patch_coord_L.txt";

    imwrite(file_path_img1, img_template_new);
    imwrite(file_path_reg, registered);


    FILE * patch_coord_file_new;
    int x1, y1, x2, y2;

    patch_coord_file_new = fopen(patch_coord_file, "w");
    for (size_t idx=0; idx<patch_coord.size(); idx++)
    {
        x1 = patch_coord[idx].x1;
        y1 = patch_coord[idx].y1;
        x2 = patch_coord[idx].x2;
        y2 = patch_coord[idx].y2;

        fprintf(patch_coord_file_new, "%d, %d, %d, %d\n", x1, y1, x2, y2);
    }
    fclose(patch_coord_file_new);

    return 0;
}
