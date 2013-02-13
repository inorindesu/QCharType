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

#ifndef INPUTMETHODLOADER_H
#define INPUTMETHODLOADER_H

#include <QString>
#include <QHash>
#include <QSet>
#include <QChar>
#include <QTextStream>

// InputMethodLoader class
// Provide an interface for input method table loading

class InputMethodLoader
{
protected:
  QString _name;
  QHash<QChar, QString> _elemMap;
  QHash<QString, QString> _charMap;
  QSet<QChar> _commitKey;
  QList<QChar> _selKey;
  QHash<QString, int> _grouping;
  bool _loaded;
  bool _hasGrouping;
public:
  static InputMethodLoader* getLoaderByName(QString name);

  // implementer of loadData function should set
  // this->_loaded to true
  virtual void loadData(QTextStream& s) = 0;

  InputMethodLoader()
  {
    this->_loaded = false;
  }
  
  bool hasGrouping()
  {
    return this->_hasGrouping;
  }

  QHash<QString, int> groupingInfo()
  {
    return this->_grouping;
  }

  QString name()
  {
    return this->_name;
  }

  QHash<QChar, QString> elemMap()
  {
    return this->_elemMap;
  }

  QHash<QString, QString> charMap()
  {
    return this->_charMap;
  }

  QSet<QChar> commitKey()
  {
    return this->_commitKey;
  }

  QList<QChar> selKey()
  {
    return this->_selKey;
  }
  
  bool loaded()
  {
    return this->_loaded;
  }

  virtual ~InputMethodLoader()
  {}
  
};

#endif
