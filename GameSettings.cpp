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

#include <QApplication>
#include <QFont>

#include "GameSettings.hpp"

GameSettings::GameSettings()
{
  this->_backColor = QColor(195, 195, 195);
  this->_foreColor = QColor(0, 0, 0);
  this->_inputMethod = "Chewing";
  this->_textDatabaseName = QString(); // TODO:TBD
  this->_fontName = QApplication::font().family();
  this->_fontSize = QApplication::font().pointSizeF();
  this->_windowWidth = 640;
  this->_windowHeight = 480;
  this->_secsToGround = 4.0f;
  this->_shieldStrength = 5.0f;
  this->_shieldRegen = 0.01f;
}

void GameSettings::load(QTextStream& s)
{
}

void GameSettings::save(QTextStream& s)
{
}
