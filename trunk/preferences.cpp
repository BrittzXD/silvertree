#include <iostream>
#include <string>

#include "filesystem.hpp"
#include "preferences.hpp"
#include "string_utils.hpp"

namespace {

bool nocombat = false;
bool maxfps = false;

bool mipmapping = true;
GLenum mipmap_min = GL_NEAREST_MIPMAP_LINEAR, mipmap_max = GL_LINEAR;
std::string save_file;

GLenum mipmap_arg_to_type(const std::string& arg) {
	if(arg == "l") {
		return GL_LINEAR;
	} else if(arg == "n") {
		return GL_NEAREST;
	} else if(arg == "nn") {
		return GL_NEAREST_MIPMAP_NEAREST;
	} else if(arg == "ll") {
		return GL_LINEAR_MIPMAP_LINEAR;
	} else if(arg == "ln") {
		return GL_LINEAR_MIPMAP_NEAREST;
	} else if(arg == "nl") {
		return GL_NEAREST_MIPMAP_LINEAR;
	}
	std::cerr << "unknown mipmap type \""<<arg<<"\": set to \"n\"\n";
	return GL_NEAREST;
}

bool parse_arg(const std::string& arg)
{
	if(arg == "--nocombat") {
		nocombat = true;
	} else if(arg == "--maxfps") {
		maxfps = true;
	} else if(arg == "--disable-mipmapping") {
		mipmapping = false;
	} else if(util::string_starts_with(arg, "--mipmapmin")) {
		std::string rest = util::strip_string_prefix(arg, "--mipmapmin");
		if(rest == "" || rest=="=" || rest.substr(0,1) != "=") {
			std::cerr << "format: mipmapmin={n,l,nn,nl,ln,ll}\n";
			return false;
		} else {
			mipmap_min = mipmap_arg_to_type(rest.substr(1));
		}
	} else if(util::string_starts_with(arg, "--mipmapmax")) {
		std::string rest = util::strip_string_prefix(arg, "--mipmapmax");
		if(rest == "" || rest == "=" || rest.substr(0,1) != "=") {
			std::cerr << "format: mipmapmax={n,l,nn,nl,ln,ll}\n";
			return false;
		} else {
			mipmap_max = mipmap_arg_to_type(rest.substr(1));
		}
	} else if(util::string_starts_with(arg, "--save=")) {
		save_file = sys::get_saves_dir() + "/" + util::strip_string_prefix(arg, "--save=");
	} else {
		std::cerr << "unrecognized argument: '" << arg << "'\n";
		return false;
	}
	
	return true;
}

}

bool preference_mipmapping() {
	return mipmapping;
}

GLenum preference_mipmap_min() {
	return mipmap_min;
}

GLenum preference_mipmap_max() {
	return mipmap_max;
}

bool preference_nocombat()
{
	return nocombat;
}

bool preference_maxfps()
{
	return maxfps;
}

const std::string& preference_save_file()
{
	return save_file;
}

bool parse_args(int argc, char** argv)
{
	for(int n = 1; n < argc; ++n) {
		if(!parse_arg(argv[n])) {
			return false;
		}
	}

	return true;
}
