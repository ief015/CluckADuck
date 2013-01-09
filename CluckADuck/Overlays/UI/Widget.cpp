#include "Widget.hpp"


Widget::Widget()
: parent(NULL)
, hover(NULL)
, down(false)
{
	x = 0.; y = 0.;
	width = 0.; height = 0.;
	mouseX = 0; mouseY = 0;
}


Widget::~Widget()
{
}


void Widget::addWidget(Widget* widget)
{
	// Push the new child to the back. (It will become the focused widget)
	widgets.push_back(widget);
	widget->parent = this;
}


void Widget::removeWidget(Widget* widget)
{
	// Find widget.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
	{
		if (widgets[i] == widget)
		{
			// Remove it from children.
			widgets.erase(widgets.begin() + i);
			widgets[i]->parent = NULL;

			break;
		}
	}
}

bool Widget::hasWidget(const Widget* widget) const
{
	// Find widget.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
	{
		if (widgets[i] == widget)
			return true;
	}

	return false;
}


const Widget* Widget::getFocused() const
{
	// Calling std::vector::back causes an undefined behaviour on an empty container.
	// Return NULL if empty.
	if (widgets.empty())
		return NULL;

	// The back widget is the focused component.
	return widgets.back();
}


bool Widget::isFocused() const
{
	// Get the focused component.
	if (parent)
		return parent->getFocused() == this;

	// Always return true if no parent.
	return true;
}

bool Widget::isMouseHovering() const
{
	// Check if the hovering component is this widget.
	if (parent)
		return parent->hover == this;

	// Always return false if no parent.
	return false;
}


void Widget::update(float dt)
{
	this->onUpdate(dt);
}


void Widget::draw(sf::RenderTarget& rt)
{
	this->onDraw(rt, this->x, this->y);
}


void Widget::onMouseDown(int x, int y, unsigned b)
{
	// Send mouse-down signal to all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
		widgets[i]->onMouseDown(static_cast<int>(x - this->x), static_cast<int>(y - this->y), b);

	// Check for any mouse-downs inside of an object.
	// When found, push to the back, making it the focused widget.
	for (size_t i = widgets.size(); i--;)
	{
		Widget* widget = widgets[i];

		if (static_cast<double>(x) >= widget->x && static_cast<double>(x) <= widget->x + widget->width &&
			static_cast<double>(y) >= widget->y && static_cast<double>(y) <= widget->y + widget->height )
		{
			// Widget is being held down.
			widget->down = true;

			// Don't make it focused if it's already focused on.
			if (widget->isFocused())
				break;

			// Make it the focused object.
			widgets.push_back(widget);
			widgets.erase(widgets.begin()+i);

			// Call lost/gained focus events.
			widgets[widgets.size()-1]->onFocusLost();
			widget->onFocusGained();

			break;
		}
	}
}


void Widget::onMouseUp(int x, int y, unsigned b)
{
	// Send mouse-up signal to all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
	{
		Widget* widget = widgets[i];

		widget->onMouseUp(static_cast<int>(x - this->x), static_cast<int>(y - this->y), b);

		if (widget->down)
		{
			// No longer held down.
			widget->down = false;

			// Check if mouse was inside.
			if (static_cast<double>(x) >= widget->x && static_cast<double>(x) <= widget->x + widget->width &&
				static_cast<double>(y) >= widget->y && static_cast<double>(y) <= widget->y + widget->height )
			{
				// Widget was clicked.
				widget->onClicked(static_cast<int>(x - widget->x), static_cast<int>(y - widget->y), b);
			}
		}
	}
}


void Widget::onMouseWheel(int x, int y, int d)
{
	// Send mouse-wheel signal to all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
		widgets[i]->onMouseWheel(static_cast<int>(x - this->x), static_cast<int>(y - this->y), d);
}


void Widget::onMouseMove(int x, int y, int dx, int dy)
{
	// Update mouse positions.
	mouseX = x;
	mouseY = y;

	// Send mouse-move signal to all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
		widgets[i]->onMouseMove(static_cast<int>(x - this->x), static_cast<int>(y - this->y), dx, dy);
}


void Widget::onKeyDown(int key)
{
	// Send key-down signal to all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
		widgets[i]->onKeyDown(key);
}


void Widget::onKeyUp(int key)
{
	// Send key-up signal to all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
		widgets[i]->onKeyUp(key);
}


void Widget::onKeyText(int ch)
{
	// Send text signal to all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
		widgets[i]->onKeyText(ch);
}


void Widget::onUpdate(float dt)
{
	// Check for mouse entering/leaving.
	bool hovering = false;
	for (size_t i = widgets.size(); i--;)
	{
		Widget* widget = widgets[i];

		if (static_cast<double>(mouseX) >= widget->x && static_cast<double>(mouseX) <= widget->x + widget->width &&
			static_cast<double>(mouseY) >= widget->y && static_cast<double>(mouseY) <= widget->y + widget->height )
		{
			hovering = true;

			// If mouse is already hovering, no need to do anything.
			if (this->hover == widget)
				break;

			// Change mouse hover to new widget.
			Widget* oldHover = this->hover;
			this->hover = widget;

			// Call widget events.
			if (oldHover)
				oldHover->onMouseLeave();
			widget->onMouseEnter();

			break;
		}
	}

	// If not hovering over anything, leave old hover widget (if any.)
	if (!hovering && this->hover)
	{
		this->hover->onMouseLeave();
		this->hover = NULL;
	}

	// Update all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
		widgets[i]->onUpdate(dt);
}


void Widget::onDraw(sf::RenderTarget& rt, double scrx, double scry)
{
	// Draw all children.
	for (size_t i = 0, sz = widgets.size(); i < sz; ++i)
		widgets[i]->onDraw(rt, widgets[i]->x + scrx, widgets[i]->y + scry);
}


void Widget::onClicked(int x, int y, int b)
{

}


void Widget::onFocusGained()
{

}


void Widget::onFocusLost()
{

}

void Widget::onMouseEnter()
{

}


void Widget::onMouseLeave()
{

}
