
/*
   Copyright (C) 2007 by David White <dave.net>
   Part of the Silver Tree Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 or later.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef MODEL_HPP_INCLUDED
#define MODEL_HPP_INCLUDED

#include <gl.h>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "material.hpp"
#include "model_fwd.hpp"

namespace graphics
{

class model
{
public:

	static const_model_ptr get_model(const std::string& key);
	struct bone;
		
	struct vertex {
		vertex() : uvmap_valid(false), bone_num(-1) {}
		boost::array<GLfloat,3> point;
		boost::array<GLfloat,3> normal;
		boost::array<GLfloat,2> uvmap;
		bool uvmap_valid;
		int bone_num;
	};
	typedef boost::shared_ptr<vertex> vertex_ptr;

	struct face {
		face() {}
		std::vector<vertex_ptr> vertices;
		std::string material_name;
		const_material_ptr mat;
	};

	struct bone {
		bone() : parent(-1) {}
		std::string name;
		int parent;
		boost::array<GLfloat,3> default_pos;
		boost::array<GLfloat,3> default_rot;
	};

	explicit model(const std::vector<face>& faces);
	model(const std::vector<face>& faces, const std::vector<bone>& bones);

	void draw() const;
	void draw_material(const const_material_ptr& mat) const;

	void get_materials(std::vector<const_material_ptr>* mats) const;

private:
	void optimize();
	void init_normals();
	boost::array<GLfloat,3> face_normal(const face& f, const vertex_ptr& v) const;
	boost::array<GLfloat,3> face_normal(const face& f, int n) const;
	void draw_face(const face& f, bool& in_triangles) const;
	std::vector<face> faces_;
	std::vector<bone> bones_;
};
		
}

#endif
