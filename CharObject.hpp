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

#ifndef CHAROBJECT_H
#define CHAROBJECT_H

#include <QChar>

#include "PaintableObject.hpp"

class CharObject: public PaintableObject
{
protected:
  float _width;
  QChar _char;
  QColor _background;
  QColor _foreground;
  QPixmap _pixmap;
  QFont _font;

  CharObject(int width, QChar char_, float x = 0, float y = 0,
             QColor background = QColor(195, 195, 195),
             QColor foreground =  QColor(0, 0, 0), QFont f = QFont());
public:
  float width()
  {
    return this->_width;
  }

  QChar character()
  {
    return this->_char;
  }
};

#endif
