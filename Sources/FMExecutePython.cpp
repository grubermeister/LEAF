//////////////////////////////////////////////////////////////////////////////////////
//  
//  FMPluginVersion.cpp - Default external function for returning plug-in version
//
//  Version 3.1, Copyright ©2002-2012 by 24U Software. All rights reserved.
//
//  Written by Tomas Zahradnicky & HOnza Koudelka
//
////////////////////////////////////////////////////////////////////////////////
//
//  The latest version of 24U Plug-In Template is available for download from:
//
//  http://www.24uSoftware.com/PlugInTemplate 
//
////////////////////////////////////////////////////////////////////////////////
//
//  24U Sample Code - Terms of use
//
//  Under the terms of the 24U Software License Agreement, 24U s.r.o. grants
//  you a non-exclusive royalty-free Developer License (3.2) to use 24U Plug-In
//  Template solely to compile plug-ins for use with FileMaker products.
//
//  Redistribution of 24U Sample Code in the source form, as part of
//  an open-source plug-in project is permitted provided that the following
//  conditions are met:
//
//  * Redistributions of source code must retain the copyright notice, the terms
//    of use, and the "24U Software License Agreement" document.
//
//  * We require that you make it clear in the source wherever modified that the
//    code was descended from 24U Sample Code, but that you've made changes.
//
//  See the "License.txt" and "24U Software License Agreement.pdf" files
//  for detailed terms of use and licensing conditions.
//
////////////////////////////////////////////////////////////////////////

//
//  This source file contains code for the plug-in's Version function. Since
//  it is recommended for every FileMaker Plug-in to have a function to check
//  the plug-in's version, we have written a simple version function for you.
//  When en empty string is passed to this function, it simply returns the
//  version of this plug-in, as defined in the FMTConfig.h header file.
//
//  As a bonus for you, if you pass the string "platform" as a parameter to this
//  function, it will respond with the information about which code is being
//  used, whether it is Mac OS X code, or Windows code. If you pass the string
//  "long" to this function, a long version string consisting of the plug-in's
//  name and version is returned.
//
////////////////////////////////////////////////////////////////////////////////

#include <CPython/Python.h>
#include <iostream>
#include <sstream>
#include <string>

#include "FMPlugin.h"
#include "FMTemplate/FMTemplate.h"

#define BUFFER_SIZE 256

#pragma mark LEAF_ExecutePython
FMX_PROC(fmx::errcode) LEAF_ExecutePython(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
    fmx::errcode        err = 0;
	fmx::LocaleAutoPtr  locale;
	fmx::TextAutoPtr    resultText;

	std::string stdOutErr = 
"import sys\n\
class CatchOutErr:\n\
	def __init__(self):\n\
		self.value = ''\n\
	def write(self, txt):\n\
		self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n\
FMargs = []\n";

	//Fire up the Python interpreter, and run our little script to redirect sys.stdout properly.
	Py_Initialize();
	PyObject *pModule = PyImport_AddModule("__main__");
	PyRun_SimpleString(stdOutErr.c_str());

	if( dataVect.Size() > 0 ){
		//We have at least one parameter. Read it and return correct information.
        const fmx::Text& parameter1 = dataVect.AtAsText(0);
		char paramContents[BUFFER_SIZE];

		//if we have any further inputs to this expression, they need to be passed to Python as arguments.
		if( (dataVect.Size() - 1) > 0 ){
			for(int i = 1; i <= (dataVect.Size() - 1); i++){
				const fmx::Text& parameter = dataVect.AtAsText(i);
				char argTemp[BUFFER_SIZE];

				//Filemaker requires you to jump through all these hoops for simple type conversion.
				parameter.GetBytes(argTemp, BUFFER_SIZE);
				std::string pyLine(argTemp);
				pyLine = "FMargs.append(\"" + pyLine + "\")\n";
				PyRun_SimpleString(pyLine.c_str());
			} 
		}

		//First parameter is what external script to run. 
		//Convert from Filemaker to Native datatype, so CPython can process it.
		parameter1.GetBytes(paramContents, BUFFER_SIZE);
		PyObject* PyFileObject = PyFile_FromString(paramContents, "r");
		PyRun_SimpleFileEx(PyFile_AsFile(PyFileObject), paramContents, 1);
	}
	
	//If I still want to simply run static code, I've kept the following little snippet commented out as a reminder.
	//PyRun_SimpleString("print 'Hello World!'");

	//OK, we've run our code, now actually redirect the result into our function output for Filemaker to use.
	PyObject *catcher = PyObject_GetAttrString(pModule,"catchOutErr");
	PyErr_Print();
	PyObject *output = PyObject_GetAttrString(catcher, "value");
	const char * pyResult = PyString_AsString(output);
	Py_Finalize();

	resultText->Assign(pyResult);
	err = result.SetAsText(*resultText, *locale);


    return err;
}
