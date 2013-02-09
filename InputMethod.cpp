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

#include "InputMethod.hpp"

void InputMethod::defaultSetup()
{
  for(int i = 0; i < 10; i++)
    {
      this->_selectionKeys << '0' + i;
    }
}

InputMethod::InputMethod(InputMethodLoader& loader)
{
  this->defaultSetup();
  this->_elementMap = loader.elemMap();
  this->_charMap = loader.charMap();
  this->_commitKeys = loader.commitKey();
  this->_selectionKeys = loader.selKey();
}

void InputMethod::input(QChar c)
{
  if (this->hasCharacter() == false)
    {
      if (c == ' ')
        {
          this->_candidates = this->_charMap.value(this->_inputBuffer, QString());
        }
      else
        {
          this->_inputBuffer.append(c);
          if (this->_commitKeys.contains(c))
            {
              this->_candidates = this->_charMap.value(this->_inputBuffer, QString());
            }
        }
    }
}

bool InputMethod::hasCharacter()
{
  return this->_candidates.isNull() == false;
}

QString InputMethod::getCandidates()
{
  return this->_candidates;
}

QString InputMethod::getInputBuffer()
{
  // convert from key to char
  QString buf = "";
  for(int i = 0; i < this->_inputBuffer.length(); i++)
    {
      buf.append( this->_elementMap.value(this->_inputBuffer.at(i), QChar()) );
    }
  return buf;
}

void InputMethod::clearStats()
{
  this->_candidates = QString();
  this->_inputBuffer = QString();
}

QChar InputMethod::selectCharFromCandidates(int index)
{
  if (index >= this->_candidates.length())
    {
      return QChar();
    }
  else
    {
      QChar ret = this->_candidates.at(index);
      this->clearStats();
      return ret;
    }
}
