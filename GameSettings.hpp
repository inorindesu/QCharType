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

#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QString>
#include <QTextStream>
#include <QColor>

class GameSettings
{
private:
  // IM & text generator behavior
  QString _inputMethod;
  QString _textDatabaseName;

  // appearance
  QColor _backColor;
  QColor _foreColor;
  QString _fontName;
  float _fontSize;
  int _windowWidth;
  int _windowHeight;
  QColor _playgroundColor;

  // difficulty
  double _secsToGround;
  double _shieldStrength;
  double _shieldRegen;

  void fillDefaultValue();
public:
  GameSettings();
  void load(QTextStream& s);
  void save(QTextStream& s);

public:
  QColor playgroundColor()
  {
    return this->_playgroundColor;
  }

  void setPlaygroundColor(QColor c)
  {
    this->_playgroundColor = c;
  }

  float fontSize()
  {
    return this->_fontSize;
  }
  
  void setFontSize(float f)
  {
    if (f > 0)
      this->_fontSize = f;
  }

  QString textDatabaseName()
  {
    return this->_textDatabaseName;
  }

  void setTextDatabaseName(QString s)
  {
    if(s.isNull() == false && s.isEmpty() == false)
      {
        this->_textDatabaseName = s;
      }
  }
  
  double shieldRegen()
  {
    return this->_shieldRegen;
  }
  
  void setShieldRegen(double d)
  {
    if (d > 0)
      {
        this->_shieldRegen = d;
      }
  }
  
  double shieldStrength()
  {
    return this->_shieldStrength;
  }
  
  void setShieldStrength(double d)
  {
    if (d > 0)
      {
        this->_shieldStrength = d;
      }
  }

  double secsToGround()
  {
    return this->_secsToGround;
  }

  void setSecsToGround(double d)
  {
    if (d > 0)
      {
        this->_secsToGround = d;
      }
  }
  
  QString fontName()
  {
    return this->_fontName;
  }
  
  void setFontName(QString name)
  {
    if (name.isNull() == false && name.isEmpty() == false)
      {
        this->_fontName = name;
      }
  }
  
  void setBackgroundColor(QColor c)
  {
    this->_backColor = c;
  }

  void setForegroundColor(QColor c)
  {
    this->_foreColor = c;
  }

  QColor foregroundColor()
  {
    return this->_foreColor;
  }
  
  QColor backgroundColor()
  {
    return this->_backColor;
  }
  
  void setInputMethodName(QString name)
  {
    if (name.isNull() == false && name.isEmpty() == false)
      {
        this->_inputMethod = name;
      }
  }

  QString inputMethodName()
  {
    return this->_inputMethod;
  }

  void setWindowWidth(int w)
  {
    if (w > 0)
      {
        this->_windowWidth = w;
      }
  }

  int windowWidth()
  {
    return this->_windowWidth;
  }

  void setWindowHeight(int h)
  {
    if(h > 0)
      {
        this->_windowHeight = h;
      }
  }

  int windowHeight()
  {
    return this->_windowHeight;
  }
};

#endif
