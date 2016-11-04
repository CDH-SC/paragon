#include <Python.h>
#include "cv.h"
#include "highgui.h"
#include <opencv2/highgui/highgui.hpp>
#include "numpy/arrayobject.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include <iostream>

#include "pm_headers.h"


using namespace cv;
using namespace std;

static PyObject *classmodError;

static PyObject* classmod_say_hello(PyObject* self, PyObject *args){

    //char img1_file[512] = "im11.bmp";
    //char img2_file[512] = "im22.bmp";

         char* msg1;
	char* msg2;
        Mat img_temp_src,reg_src;
	//int w,h;
       //PyArrayObject *output;
       //PyObject *output1,*output2,*item1,*item2,*insideitem11,*insideitem12,*insideitem21,*insideitem22;
      // double ditem11,ditem12,ditem21,ditem22;
	//  int i1,j1,n1,m1,i2,j2,n2,m2;

	  if(!PyArg_ParseTuple(args, "ss",&msg1,&msg2)){
	       return NULL;
		   }

	  else{
                   vector <bbox> patch_coord;
                   img_temp_src = imread(msg1, 1);
                   reg_src = imread(msg2, 1);
                   int status = find_diff(img_temp_src, reg_src, patch_coord);

    /*************************************************************************************/
    /*Write patch coordinates into txt file*/
		    if (status!=0)
		    {
        /*File not found*/
		        return Py_BuildValue("");
    		    }

		    //FILE * patch_coord_file_new;
		    int x1, y1, x2, y2;

		    //patch_coord_file_new = fopen("patch_coord.txt", "w");
                    int featnum=patch_coord.size();
                    PyObject* tuple1= PyTuple_New(featnum);
		    for (size_t idx=0; idx<patch_coord.size(); idx++)
		    {
		        x1 = patch_coord[idx].x1;
		        y1 = patch_coord[idx].y1;
		        x2 = patch_coord[idx].x2;
		        y2 = patch_coord[idx].y2;
                        cout<<"x1:"<<x1<<" y1:"<<y1<<" x2:"<<x2<<" y2:"<<y2<<endl;
                        PyTuple_SetItem(tuple1,idx,Py_BuildValue("iiii",x1,y1,x2,y2));
		       // fprintf(patch_coord_file_new, "%d, %d, %d, %d\n", x1, y1, x2, y2);
		    }
		  //  fclose(patch_coord_file_new);
                    Py_INCREF(tuple1);
                    return Py_BuildValue("O",tuple1);
		   // return 0;
	     }
         return NULL;
}

static PyMethodDef classmod_methods[] = {

{"say_hello", classmod_say_hello, METH_VARARGS, "say hello from c"},
{NULL,NULL,0,NULL}
};


PyMODINIT_FUNC initclassmod(void){

    	PyObject *m;
	m = Py_InitModule("classmod", classmod_methods);
	import_array();
	if( m == NULL) return;

	classmodError = PyErr_NewException("classmod.error", NULL, NULL);
	Py_INCREF(classmodError);

	PyModule_AddObject(m,"error",classmodError);

}
