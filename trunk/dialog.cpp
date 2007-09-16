
/*
   Copyright (C) 2007 by David White <dave.net>
   Part of the Silver Tree Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 or later.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#include <gl.h>
#include "SDL.h"

#include <iostream>

#include "dialog.hpp"
#include "font.hpp"
#include "foreach.hpp"
#include "raster.hpp"
#include "surface_cache.hpp"
#include "texture.hpp"
#include "tooltip.hpp"

namespace gui {

dialog::dialog(int x, int y, int w, int h)
  : opened_(false), clear_bg_(true), padding_(10), 
    add_x_(0), add_y_(0)
{
	set_loc(x,y);
	set_dim(w,h);
}

dialog& dialog::add_widget(widget_ptr w, dialog::MOVE_DIRECTION dir)
{
	add_widget(w, add_x_, add_y_, dir);
	return *this;
}

dialog& dialog::add_widget(widget_ptr w, int x, int y,
                           dialog::MOVE_DIRECTION dir)
{
	w->set_loc(x,y);
	widgets_.push_back(w);
	switch(dir) {
	case MOVE_DOWN:
		add_x_ = x;
		add_y_ = y + w->height() + padding_;
		break;
	case MOVE_RIGHT:
		add_x_ = x + w->width() + padding_;
		add_y_ = y;
		break;
	}
	return *this;
}

void dialog::remove_widget(widget_ptr w)
{
	widgets_.erase(std::remove(widgets_.begin(),widgets_.end(),w),
	               widgets_.end());
}

void dialog::replace_widget(widget_ptr w_old, widget_ptr w_new) 
{
	int x = w_old->x();
	int y = w_old->y();
	int w = w_old->width();
	int h = w_old->height();

	std::replace(widgets_.begin(), widgets_.end(), w_old, w_new);

	w_new->set_loc(x,y);
	w_new->set_dim(w,h);
}

void dialog::show() {
	opened_ = true;
	set_visible(true);
}

void dialog::show_modal()
{
	opened_ = true;
	while(opened_) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			process_event(event);
		}

		prepare_draw();
		draw();
		gui::draw_tooltip();
		complete_draw();
	}
}

void dialog::prepare_draw()
{
	graphics::prepare_raster();
	if(clear_bg()) {
		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void dialog::complete_draw()
{
	SDL_GL_SwapBuffers();
	SDL_Delay(1);
}

void dialog::handle_draw_children() const {
	glPushMatrix();
	glTranslatef(x(),y(),0.0);
	foreach(const widget_ptr& w, widgets_) {
		w->draw();
	}
	glPopMatrix();
}

void dialog::handle_draw() const
{
	if(clear_bg()) {
		SDL_Rect rect = {x(),y(),width(),height()};
		SDL_Color col = {0,0,0,0};
		graphics::draw_rect(rect,col);
	}
	handle_draw_children();
}

bool dialog::handle_event_children(const SDL_Event &event) {
	SDL_Event ev = event;
	normalize_event(&ev);
	std::vector<widget_ptr> widgets = widgets_;
	bool claimed = false;
	foreach(const widget_ptr& w, widgets) {
		if(w->process_event(ev)) {
			claimed = true;
			break;
		}
	}
	return claimed;
}

bool dialog::handle_event(const SDL_Event& event)
{
	bool claimed = false;
	if(event.type == SDL_KEYDOWN &&
	   (event.key.keysym.sym == SDLK_SPACE ||
	    event.key.keysym.sym == SDLK_RETURN)) {
		close();
		claimed = true;
	}
	if(!claimed) {
		claimed = handle_event_children(event);
	}
	return claimed;
}

		
}
