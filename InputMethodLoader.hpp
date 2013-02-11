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
  QSet<QChar> _selKey;
public:
  InputMethodLoader();
  virtual void loadData(QTextStream& s) = 0;
  QString name();
  QHash<QChar, QString> elemMap();
  QHash<QString, QString> charMap();
  QSet<QChar> commitKey();
  QSet<QChar> selKey();
  static InputMethodLoader* getLoaderByName(QString name);

  virtual ~InputMethodLoader()
  {}
};

#endif
