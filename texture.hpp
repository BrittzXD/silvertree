
/*
   Copyright (C) 2007 by David White <dave.net>
   Part of the Silver Tree Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 or later.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <string>
#include <boost/shared_ptr.hpp>
#include <vector>

#include <gl.h>

#include "surface.hpp"

namespace graphics
{

class texture
{
public:
	texture() : width_(0), height_(0) {}

	typedef std::vector<surface> key;

	void set_as_current_texture() const;
	bool valid() const { return id_; }

	static texture get(const std::string& str);
	static texture get(const key& k);
	static texture get(const surface& surf);
	static texture get_no_cache(const key& k);
	static texture get_no_cache(const surface& surf);
	static void set_current_texture(const key& k);
	static void set_coord(GLfloat x, GLfloat y);

	unsigned int width() const { return width_; }
	unsigned int height() const { return height_; }

	friend bool operator==(const texture&, const texture&);
	friend bool operator<(const texture&, const texture&);

private:
	explicit texture(const key& surfs);

	struct ID {
		explicit ID(GLuint id) : id(id) {
		}

		~ID();

		GLuint id;
	};
	boost::shared_ptr<ID> id_;
	unsigned int width_, height_;
	GLfloat ratio_w_, ratio_h_;
};

inline bool operator==(const texture& a, const texture& b)
{
	return a.id_ == b.id_;
}

inline bool operator!=(const texture& a, const texture& b)
{
	return !operator==(a, b);
}

inline bool operator<(const texture& a, const texture& b)
{
	return a.id_ < b.id_;
}

}

#endif