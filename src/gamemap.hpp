
/*
   Copyright (C) 2007 by David White <dave.net>
   Part of the Silver Tree Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 or later.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef GAMEMAP_HPP_INCLUDED
#define GAMEMAP_HPP_INCLUDED

#include <string>
#include <vector>
#include <GL/glew.h>

#include "tile.hpp"
#include "tile_logic.hpp"

namespace hex
{

class gamemap
{
public:
    explicit gamemap(const std::string& data);
    gamemap(const std::vector<tile>& tiles, const location& dim);
    void copy_from(const gamemap& m);
    
    std::string write() const;
    
    const location& size() const { return dim_; }
    const tile& get_tile(const location& loc) const;
    tile& get_tile(const location& loc);
    bool is_loc_on_map(const location& loc) const;

    const tile* closest_tile(GLfloat* x, GLfloat* y, bool return_null_outside_border=true) const;
    
    struct parse_error {
        parse_error(const std::string& msg)
        {}
	};
    
    void draw() const;
    void draw_grid() const;
    
    const std::vector<tile>& tiles() const { return map_; }
    
    //map mutation functions
    void adjust_height(const hex::location& loc, int adjust);
    void set_terrain(const hex::location& loc, const std::string& terrain_id);
    void set_feature(const hex::location& loc, const std::string& feature_id);

    bool has_line_of_sight(const location& a, const location& b,
                           std::vector<location>* tiles=NULL,
                           int range=-1) const;
    location tile_in_the_way(const location& a, const location& b,
                             std::vector<location>* tiles=NULL,
                             int range=-1) const;
private:
	gamemap(const gamemap&);
	void operator=(const gamemap&);

	void parse(const std::string& data);
	void init_tiles();

	std::vector<tile> map_;
	location dim_;
};

typedef boost::shared_ptr<gamemap> gamemap_ptr;
typedef boost::shared_ptr<const gamemap> const_gamemap_ptr;

}

#endif
