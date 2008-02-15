
/*
   Copyright (C) 2007 by David White <dave.net>
   Part of the Silver Tree Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 or later.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#include "tooltip.hpp"
#include "translate.hpp"
#include "widget.hpp"

#include <iostream>

namespace gui {

widget::~widget()
{
	if(tooltip_displayed_) {
		gui::remove_tooltip(tooltip_);
	}
}

void widget::normalize_event(SDL_Event* event)
{
	switch(event->type) {
	case SDL_MOUSEMOTION:
		event->motion.x -= x();
		event->motion.y -= y();
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		event->button.x -= x();
		event->button.y -= y();
		break;
	default:
		break;
	}
}

void widget::set_tooltip(const std::string& str)
{
	if(tooltip_displayed_) {
		gui::remove_tooltip(tooltip_);
		tooltip_displayed_ = false;
	}
	tooltip_.reset(new std::string(i18n::translate(str)));
}

bool widget::process_event(const SDL_Event& event, bool claimed)
{
    if(!claimed) {
        if(tooltip_ && event.type == SDL_MOUSEMOTION) {
            if(event.motion.x >= x() && event.motion.x <= x()+width() &&
               event.motion.y >= y() && event.motion.y <= y()+height()) {
                if(!tooltip_displayed_) {
                    gui::set_tooltip(tooltip_);
                    tooltip_displayed_ = true;
                }
            } else {
                if(tooltip_displayed_) {
                    gui::remove_tooltip(tooltip_);
                    tooltip_displayed_ = false;
                }
            }
        }
    }

	return handle_event(event, claimed);
}

void widget::draw() const
{
	if(visible_) {
		handle_draw();
	}
}

}
