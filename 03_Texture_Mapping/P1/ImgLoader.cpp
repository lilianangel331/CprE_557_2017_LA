#include "ImgLoader.h"




// a pointer for the texture data. 
unsigned char * g_image_data;







/*!
Verifies wheterh a file [name] exits
@param name - the path and the name of the file.
*/
bool ImgLoader::SearchFile(const std::string& name, string& new_name)
{
	new_name = "";

	bool exits = false;

	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		new_name = name;
		exits = true;
		return exits;
	}
	else {
		f.close();
		exits = false;
	}



	int idx = name.find_first_of("/");
	string newstring = name.substr(idx + 1, name.length() - 3);


	ifstream f2(newstring.c_str());
	if (f2.good()) {
		f2.close();
		exits = true;
	}
	else {
		f2.close();
		exits = false;
	}


	if (exits)
	{
		new_name = newstring;
	}


	return exits;
}



/*!
Loads a texture from a file and creates the necessary texture objects
@param path_and_file to the texture object
@return unsigned char - a pointer to the texture data
*/
unsigned char* ImgLoader::Load(string path_and_file)
{
	// Check whether we load a bitmap file

	int idx = path_and_file.find_last_of(".");
	string suffix = path_and_file.substr(idx + 1, 3);

	if (suffix.compare("bmp") != 0) {
		cerr << "[ERROR] Filetype " << suffix << " is currently not supported. This example only support bitmap files. " << endl;
		return false;
	}


	string checked_path_and_file;
	bool ret = SearchFile(path_and_file, checked_path_and_file);

	if (!ret)
	{
		cerr << "[ERROR] Cannot find the file " << path_and_file << "." << endl;
		return false;
	}

	//**********************************************************************************************
	// Loads the file content

	int channels = 3;
	
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;

	// This opens a file
	FILE * file;
	file = fopen(checked_path_and_file.c_str(), "rb");

	if (file == NULL) return 0;

	// This reads the header of the file and checks the length.
	if (fread(header, 1, 54, file) != 54)
	{
		// If not 54 bytes read, this is not a bmp.
		// Only a bmp has a header of length 54
		printf("Not a correct BMP file\n");
		return false;
	}

	// Read the start position of the data, the size, the width, and height.
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	channels = imageSize / (width * height);

	// remove all the data, which is stored behind g_image_data
	delete (g_image_data);

	// Create memory for this texture
	g_image_data = (unsigned char *)malloc(width * height * channels);

	// Read the data from a file.
	fread(g_image_data, width * height * channels, 1, file);

	// Release the file.
	fclose(file);


	// Return the texture.
	return g_image_data;

}
