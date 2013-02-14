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
  QList<QChar> _selectionKeys;
  QString _inputBuffer;
  QString _candidates;
  QHash<QString, int> _grouping;
  bool _hasGrouping;
  int _currentPage;
  
  void defaultSetup();
  void processGrouping();
public:
  InputMethod(InputMethodLoader& l);
  // receive an input character
  void input(QChar c);
  // clear input buffer
  void clearStats();
  // get symbols in the buffer （Ex: ㄅ一）
  const QString getInputBuffer() const;
  // get character from the candidates
  const QChar selectCharFromCandidates(int index);
  //const QChar selectCharFromCandidates(int page, QChar key);
  const QChar selectCharFromCurrentPage(QChar key);
  const QString getCandidatesOfCurrentPage() const;

  // check if candidate selection is available
  bool hasCharacter() const
  {
    return this->_candidates.isNull() == false;
  }

  // get the list of candidate
  const QString getCandidates() const
  {
    return this->_candidates;
  }

  // get number of candidates per page

  int candidatesPerPage() const
  {
    return this->_selectionKeys.count();
  }

  bool isSelectionKey(QChar key) const
  {
    return this->_selectionKeys.contains(key);
  }

  const QList<QChar> selectionKeys() const
  {
    return this->_selectionKeys;
  }

  int getCurrentPage() const
  {
    return this->_currentPage;
  }

  int candidatePageCount() const
  {
    int n = this->getCandidates().count();
    int p = this->candidatesPerPage();
    int a = n / p;
    if (n % p != 0)
      a += 1;
    return a;
  }

  static InputMethod* loadInputMethodByName(QString dataDir, QString s);
  static QStringList getIMNameList(QString dataDir);
  static QHash<QString, QString> getShownameNamePairs(QString dataDir);
};

#endif
