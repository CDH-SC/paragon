#include <Python.h>
#include "numpy/arrayobject.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "pm_headers.h"

using namespace cv;
using namespace std;

static PyObject *collatemodError;




static PyObject* collatemod_say_hello(PyObject* self, PyObject *args){

 
      char* msg1;
      char* msg2;
      char* msg3;
      PyObject *output1,*output2,*item1,*item2,*insideitem11,*insideitem12,*insideitem21,*insideitem22;
       double ditem11,ditem12,ditem21,ditem22;
	  int i1,n1,m1,i2,n2,m2;
      Mat img_temp_src,img_targ_src,registered;

      if(!PyArg_ParseTuple(args, "sssOO",&msg1,&msg2,&msg3,&output1,&output2)){
	       return NULL;
		   }
		   
	  else{
                  //char img1_file[512] = "/home/salvi/newdjang/homework/fastcollate/FQ190_WU_1_D2.jpg";
    //char img2_file[512] = "/home/salvi/newdjang/homework/fastcollate/FQ190_HRH_2_D2.jpg";
    n1 = PySequence_Length(output1);
    n2 = PySequence_Length(output2);
    //vector<Point> feat_template_f, feat_target_f;
    std::vector<cv::Point> iP,iiP;

    for(i1 = 0; i1 < n1; i1++) {
             /* PySequence_GetItem INCREFs item. */
         		item1 = PySequence_GetItem(output1, i1);
			m1=PySequence_Length(output1);
			//if(m < 0)
			//return -1;             		
			//if (item == NULL)
                 	//return -1; /* Not a sequence, or other failure */
             		//for(j = 0; j<m; j++){
								
				insideitem11 = PySequence_GetItem(item1, 0);
				insideitem12 = PySequence_GetItem(item1, 1);
				ditem11 = PyFloat_AsDouble(insideitem11);
				ditem12 = PyFloat_AsDouble(insideitem12);				
				//ditem11 = PyInt_AsLong(insideitem11);
				//ditem12 = PyInt_AsLong(insideitem12);				
				iP.push_back(cv::Point(ditem11,ditem12));				
				std::cout<<iP[i1]<<iP.size()<<std::endl;
		          // }
             		//Py_DECREF(item);
         }

		for(i2 = 0; i2 < n2; i2++) {
             /* PySequence_GetItem INCREFs item. */
         		item2 = PySequence_GetItem(output2, i2);
			m2=PySequence_Length(output2);
			//if(m < 0)
			//return -1;             		
			//if (item == NULL)
                 	//return -1; /* Not a sequence, or other failure */
             		//for(j = 0; j<m; j++){
								
				insideitem21 = PySequence_GetItem(item2, 0);
				insideitem22 = PySequence_GetItem(item2, 1);
				ditem21 = PyFloat_AsDouble(insideitem21);
				ditem22 = PyFloat_AsDouble(insideitem22);				
				iiP.push_back(cv::Point(ditem21,ditem22));				
				std::cout<<iiP[i2]<<iiP.size()<<std::endl;
		          // }
             		//Py_DECREF(item);
         }








    //size_t feat_num_max = 1000;
    //size_t feat_num;
    //int x1, y1, x2, y2;
    //int status = 0; // Indicating if there is an error or not
   // int width = 570, height = 881;

























   // status = patch_match(msg1, msg2, width, height, feat_template_f, feat_target_f, feat_num_max);
   // if (status!=0)
   // {
     //   return NULL;
   // }
   // feat_num = feat_template_f.size();

    cout << "Finishing finding feat points!!!" << endl;

/***************************************************************/
    /*Write to files*/
//    FILE *ft_template_file = fopen("/home/salvi/newdjang/homework/fastcollate/ft1.txt", "w");
//    FILE *ft_target_file = fopen("/home/salvi/newdjang/homework/fastcollate/ft2.txt", "w");

  //  for (size_t i=0; i<feat_num; i++)
   // {
     //   x1 = feat_template_f[i].x;
      //  y1 = feat_template_f[i].y;
       // x2 = feat_target_f[i].x;
       // y2 = feat_target_f[i].y;

        //fprintf(ft_template_file, "%d, %d\n", x1, y1);
       // fprintf(ft_target_file, "%d, %d\n", x2, y2);
    //}
   // fclose(ft_template_file);
   // fclose(ft_target_file);

/***************************************************************/
    /*TPS transform*/

    //char file_path_reg[512] = "/home/salvi/newdjang/homework/fastcollate/registered.jpg";
    img_temp_src = imread(msg1, 1);
    img_targ_src = imread(msg2, 1); 
    thin_plate_spline(img_temp_src, img_targ_src, registered, iP, iiP);
    imwrite(msg3, registered); 
		    //patch_coord_file_new = fopen("patch_coord.txt", "w");
                    return Py_BuildValue("");
		   // return 0;
	     }
         return NULL;
}
     

static PyMethodDef collatemod_methods[] = {

{"say_hello", collatemod_say_hello, METH_VARARGS, "say hello from c"},
{NULL,NULL,0,NULL}
};	  
	  
	  
PyMODINIT_FUNC initcollatemod(void){
 
    	PyObject *m;
	m = Py_InitModule("collatemod", collatemod_methods);
	import_array();
	if( m == NULL) return;
	
	collatemodError = PyErr_NewException("collatemod.error", NULL, NULL);
	Py_INCREF(collatemodError);
	
	PyModule_AddObject(m,"error",collatemodError);

}


