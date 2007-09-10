
/*
   Copyright (C) 2007 by David White <dave.net>
   Part of the Silver Tree Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 or later.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef DIALOG_HPP_INCLUDED
#define DIALOG_HPP_INCLUDED

#include "callback.hpp"
#include "widget.hpp"

#include <string>
#include <vector>

namespace gui {

class dialog : public widget
{
public:
	typedef std::vector<widget_ptr>::const_iterator child_iterator;

	virtual ~dialog() {}
	virtual void show_modal();
	void show();

	enum MOVE_DIRECTION { MOVE_DOWN, MOVE_RIGHT };
	dialog& add_widget(widget_ptr w, MOVE_DIRECTION dir=MOVE_DOWN);
	dialog& add_widget(widget_ptr w, int x, int y,
	                MOVE_DIRECTION dir=MOVE_DOWN);
	void remove_widget(widget_ptr w);
	void replace_widget(widget_ptr w_old, widget_ptr w_new);
	void clear() { widgets_.clear(); }
	void set_padding(int pad) { padding_ = pad; }
	void close() { opened_ = false; }

	bool closed() { return !opened_; }
	void set_cursor(int x, int y) { add_x_ = x; add_y_ = y; }
	int cursor_x() const { return add_x_; }
	int cursor_y() const { return add_y_; }
	child_iterator begin_children() { return widgets_.begin(); }
	child_iterator end_children() { return widgets_.end(); }
protected:
	dialog(int x, int y, int w, int h);
	virtual void handle_event(const SDL_Event& event);
	virtual void handle_draw() const;
	virtual void handle_draw_children() const;
	void prepare_draw();
	void complete_draw();
	void set_clear_bg(bool clear) { clear_bg_ = clear; };
	bool clear_bg() const { return clear_bg_; };
private:
	std::vector<widget_ptr> widgets_;
	bool opened_;
	bool clear_bg_;

	//default padding between widgets
	int padding_;

	//where the next widget will be placed by default
	int add_x_, add_y_;
};

DEFINE_CALLBACK(close_dialog_callback, dialog, close);

}

#endif
