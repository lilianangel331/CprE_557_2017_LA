/*****************************************************
This class provides a function to load shader code
from a file.
It returns the code as std::string

Usage:
string shader_code = ShaderFileUtils::LoadFromFile("[path\\file.vs or .fs]");

Rafael Radkowski
Iowa State University
rafael@iastate.edu
August 2015
All rights reserverd.

*****************************************************/

#pragma once


//STL
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class ShaderFileUtils
{
private:

	/*!
	Verifies wheterh a file [name] exits
	@param name - the path and the name of the file.
	*/
	static bool Exists(const std::string& name);



	/*!
	Searches for the file in name.
	Returns the new path in new_path.
	*/
	static bool Search(const std::string& name, std::string& new_path);

public:

	/*
	Load shader code from a text file and return it as string. 
	@param path_and_file - the path and file which contains the shader code as string. 
	@return - shader code as string. 
	*/
	static string LoadFromFile(string path_and_file);

};

