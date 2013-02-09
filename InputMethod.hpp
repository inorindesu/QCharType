// 
// Copyright (C) 2013  Inori Sakura <inorindesu@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or (at
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
//
// InputMethod class
// Receives key input series and transform them to text.
// This class inplements a non-inteligent input method

#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QString>
#include <QHash>
#include <QList>
#include <QChar>

#include "InputMethodLoader.hpp"

class InputMethod
{
private:
  // key to character-element map,
  // Ex: a -> ㄇ
  QHash<QChar, QString> _elementMap;
  // key series to resulting characters
  // Ex: su3 -> 你妳擬..
  QHash<QString, QString> _charMap;
  // list of commit key
  // default one is <space> (space key)
  QSet<QChar> _commitKeys;
  // list of selection key
  // default is 1~9 keys of the main keyboard
  QSet<QChar> _selectionKeys;
  QString _inputBuffer;
  QString _candidates;
  void defaultSetup();
public:
  InputMethod(InputMethodLoader& l);
  // receive an input character
  void input(QChar c);
  // check if candidate selection is available
  bool hasCharacter();
  // get the list of candidate
  QString getCandidates();
  // clear input buffer
  void clearStats();
  // get symbols in the buffer （Ex: ㄅ一）
  QString getInputBuffer();
  // get character from the candidates
  QChar selectCharFromCandidates(int index);
};

#endif
