#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <time.h>

#include "pm_headers.h"

static PyObject *patchmodError;

static PyObject* patchmod_say_hello(PyObject* self, PyObject *args){
        char* msg1;
	char* msg2;
        char* msg3;
        char* msg4;
	  
	  if(!PyArg_ParseTuple(args, "ssss",&msg1,&msg2,&msg3,&msg4)){
	       return NULL;
		   }
		   
	  else{

          Mat img_template_new, registered;
          vector<bbox> patch_coord;

          int status = book_pipeline(msg1, msg2, img_template_new, registered, patch_coord);

          //char file_path_img1[512] = "./img_template_new.jpg";
          //char file_path_reg[512]  = "./registered.jpg";
         

         imwrite(msg3, img_template_new);
         imwrite(msg4, registered);
         cout<<"reached here"<<endl;
          int x1, y1, x2, y2;
          PyObject* tuple1= PyTuple_New(patch_coord.size());   
	  PyObject* tuple2= PyTuple_New(patch_coord.size());	
          PyObject* tuple3= PyTuple_New(patch_coord.size());   
	  PyObject* tuple4= PyTuple_New(patch_coord.size());	   
          
          for (size_t idx=0; idx<patch_coord.size(); idx++)
          {
           x1 = patch_coord[idx].x1;
           y1 = patch_coord[idx].y1;
           x2 = patch_coord[idx].x2;
           y2 = patch_coord[idx].y2;
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



static PyMethodDef patchmod_methods[] = {

{"say_hello", patchmod_say_hello, METH_VARARGS, "say hello from c"},
{NULL,NULL,0,NULL}
};	  
	  
	  
PyMODINIT_FUNC initpatchmod(void){
 
    	PyObject *m;
	m = Py_InitModule("patchmod", patchmod_methods);
	//import_array();
	if( m == NULL) return;
	
	patchmodError = PyErr_NewException("patchmod.error", NULL, NULL);
	Py_INCREF(patchmodError);
	
	PyModule_AddObject(m,"error",patchmodError);

}	  
