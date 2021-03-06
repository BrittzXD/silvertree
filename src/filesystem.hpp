
/*
   Copyright (C) 2007 by David White <dave.net>
   Part of the Silver Tree Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 or later.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef FILESYSTEM_HPP_INCLUDED
#define FILESYSTEM_HPP_INCLUDED

#include <string>
#include <vector>
#include <stdexcept>
#include <cerrno>

#include <string.h>

namespace sys
{

class filesystem_error : public std::runtime_error
{
	public:
	filesystem_error(const std::string& msg) : std::runtime_error(msg + std::string(strerror(errno)) + ".\n") {};
};

enum FILE_NAME_MODE { ENTIRE_FILE_PATH, FILE_NAME_ONLY };

//! Populates 'files' with all the files and
//! 'dirs' with all the directories in dir.
//! If files or dirs are NULL they will not be used.
//!
//! Mode determines whether the entire path or just the filename is retrieved.
void get_files_in_dir(const std::string& dir,
                      std::vector<std::string>* files,
                      std::vector<std::string>* dirs=NULL,
                      FILE_NAME_MODE mode=FILE_NAME_ONLY);

//creates a dir if it doesn't exist and returns the path
std::string get_dir(const std::string& dir);
std::string get_user_data_dir();
std::string get_saves_dir();

std::string read_file(const std::string& fname);
void write_file(const std::string& fname, const std::string& data);

bool file_exists(const std::string& fname);
std::string find_file(const std::string& name);

}

#endif
