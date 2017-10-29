#include "ShaderFileUtils.h"





/*!
Verifies wheterh a file [name] exits
@param name - the path and the name of the file.
*/
bool ShaderFileUtils::Exists(const std::string& name)
{
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}


/*!
Searches for the file in name.
Returns the new path in new_path.
*/
bool ShaderFileUtils::Search(const std::string& name, std::string& new_path)
{

	new_path = "";

	bool exits = false;

	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		new_path = name;
		exits = true;
		return exits;
	}
	else {
		f.close();
		exits = false;
	}

	// Check the folder one level up. 
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
		new_path = newstring;
	}

	return exits;
}


/*!
Opens a file and loads the code from this file.
*/
string ShaderFileUtils::LoadFromFile(string path_and_file)
{
	string new_path_and_file;
	if (!Search(path_and_file, new_path_and_file)) {

		cerr << "[ERROR] Cannot find shader program " << path_and_file << "." << endl;
		return "";
	}


	ifstream in(new_path_and_file);

	if (in.good())
	{

		std::string str;

		in.seekg(0, std::ios::end);
		str.reserve(in.tellg());
		in.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());

		return str;
	}
	return "";
}