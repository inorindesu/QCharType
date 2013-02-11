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

#include "CharObject.hpp"

CharObject::CharObject(int width, QChar char_,
                       float x, float y,
                       QColor background, QColor foreground , QFont f)
  : PaintableObject(x, y)
{
  this->_background = background;
  this->_foreground = foreground;
  this->_font = f;
  int sizeThreshold = f.pointSizeF() * 1.5;
  if (width > 0)
    {
      this->_width = sizeThreshold;
    }
  else
    {
      this->_width = f.pointSizeF() * 1.5;
    }
  this->_char = char_;
  this->_pixmap = QPixmap(this->_width, this->_width);
  this->_pixmap.fill(QColor(0, 0, 0, 0));
}
