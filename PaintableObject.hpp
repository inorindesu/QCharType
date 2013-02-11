// 
// Copyright (C) 2013  Inori Sakura <inorindesu@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 

#ifndef PAINTABLEOBJECT_H
#define PAINTABLEOBJECT_H

#include <QPainter>

class PaintableObject
{
protected:
  float _x;
  float _y;

public:
  PaintableObject(float x = 0.0, float y = 0.0)
  {
    this->_x = x;
    this->_y = y;
  }

  float x()
  {
    return this->_x;
  }

  void changeX(float delta)
  {
    this->_x += delta;
  }

  float y()
  {
    return this->_y;
  }

  void changeY(float delta)
  {
    this->_y += delta;
  }

  void setX(float v)
  {
    this->_x = v;
  }

  void setY(float v)
  {
    this->_y = v;
  }

  virtual void paint(QPainter*) = 0;
};

#endif
