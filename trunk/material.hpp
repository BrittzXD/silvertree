
/*
   Copyright (C) 2007 by David White <dave.net>
   Part of the Silver Tree Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 or later.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef MATERIAL_HPP_INCLUDED
#define MATERIAL_HPP_INCLUDED

#include <string>

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

#include "texture.hpp"

namespace graphics
{

class material
{
public:
	material();
	void set_texture(const std::string& fname);
	void set_ambient(const GLfloat* v);
	void set_diffuse(const GLfloat* v);
	void set_specular(const GLfloat* v);

	void set_as_current_material() const;
private:
	texture tex_;

	typedef boost::array<GLfloat,4> color;
	color ambient_, diffuse_, specular_;
};

typedef boost::shared_ptr<material> material_ptr;
typedef boost::shared_ptr<const material> const_material_ptr;
		
}

#endif
