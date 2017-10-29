/*****************************************************
 This class provides a function to load an Bitmap image 
 from a file. 
 It returns the pointer to this bitmap image. 

 Usage:
	char* data = ImgLoader::Load("[path\\file.bmp]");

Note, the code stores the image in a global variable. 
Thus, you MUST use the image data and create a texture before
you can load the next image file. 

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


class ImgLoader
{
private:
	/*!
	Verifies wheterh a file [name] exits
	@param name - the path and the name of the file.
	*/
	static bool ImgLoader::SearchFile(const std::string& name, string& new_name);

public:

	/*!
	Loads a texture from a file and creates the necessary texture objects
	@param path_and_file to the texture object
	@return unsigned char - a pointer to the texture data
	*/
	static unsigned char* Load(string path_and_file);
};

