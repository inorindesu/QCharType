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

#include "NormalCharBlock.hpp"

NormalCharBlock::NormalCharBlock(float width, QChar char_, float x, float y,
                                 QColor background, QColor foreground, 
                                 QFont font)
  : CharObject(width, char_, x, y, background, foreground, font)
{
  // prepare the pixmap
  // pixmap is already initialized in CharObject ctor.
  // size is this->_width * this->_width
  
  QPainter p(&this->_pixmap);
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setRenderHint(QPainter::TextAntialiasing, true);
  p.setBrush(QBrush(this->_background));
  p.drawEllipse(this->_pixmap.rect());
  p.setBrush(QBrush(this->_foreground));
  p.drawText(this->_pixmap.rect(), Qt::AlignCenter, QString(this->_char));
}
