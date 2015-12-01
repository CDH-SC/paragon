#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <time.h>

#include "pm_headers.h"

static PyObject *patchsingmodError;

static PyObject* patchsingmod_say_hello(PyObject* self, PyObject *args){
        char* msg1;
	char* msg2;
        char* msg3;
        char* msg4;
	  
	  if(!PyArg_ParseTuple(args, "ssss",&msg1,&msg2,&msg3,&msg4)){
	       return NULL;
		   }
		   
	  else{

          Mat img_template_new, img_target_new;
          vector<bbox> patch_coord;
          vector<Point> feat_template_f, feat_target_f;
          int status = book_pipeline_single(msg1, msg2, img_template_new, img_target_new, feat_template_f, feat_target_f);

          //char file_path_img1[512] = "./img_template_new.jpg";
          //char file_path_reg[512]  = "./registered.jpg";
         

         imwrite(msg3, img_template_new);
         imwrite(msg4, img_target_new);
         cout<<"reached here"<<endl;
          int x1, y1, x2, y2;
          PyObject* tuple1= PyTuple_New(feat_template_f.size());   
	  PyObject* tuple2= PyTuple_New(feat_template_f.size());	
          PyObject* tuple3= PyTuple_New(feat_template_f.size());   
	  PyObject* tuple4= PyTuple_New(feat_template_f.size());	   
          
          for (size_t idx=0; idx<feat_template_f.size(); idx++)
          {
           x1 = feat_template_f[idx].x;
           y1 = feat_template_f[idx].y;
           x2 = feat_target_f[idx].x;
           y2 = feat_target_f[idx].y;
           PyTuple_SetItem(tuple1,idx,Py_BuildValue("i",x1));
           PyTuple_SetItem(tuple2,idx,Py_BuildValue("i",y1));
           PyTuple_SetItem(tuple3,idx,Py_BuildValue("i",x2));
           PyTuple_SetItem(tuple4,idx,Py_BuildValue("i",y2));
        
          }

           Py_INCREF(tuple1);
           Py_INCREF(tuple2);
           Py_INCREF(tuple3);
           Py_INCREF(tuple4);
           cout<<"reached here too"<<endl;
           return Py_BuildValue("OOOO",tuple1,tuple2,tuple3,tuple4);




              } 
         return NULL;
}



static PyMethodDef patchsingmod_methods[] = {

{"say_hello", patchsingmod_say_hello, METH_VARARGS, "say hello from c"},
{NULL,NULL,0,NULL}
};	  
	  
	  
PyMODINIT_FUNC initpatchsingmod(void){
 
    	PyObject *m;
	m = Py_InitModule("patchsingmod", patchsingmod_methods);
	//import_array();
	if( m == NULL) return;
	
	patchsingmodError = PyErr_NewException("patchsingmod.error", NULL, NULL);
	Py_INCREF(patchsingmodError);
	
	PyModule_AddObject(m,"error",patchsingmodError);

}	  
