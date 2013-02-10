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

#include "InputMethodLoader.hpp"

// implementations
#include "ChewingIMLoader.hpp"

InputMethodLoader::InputMethodLoader()
{
}

QString InputMethodLoader::name()
{
  return this->_name;
}

QHash<QChar, QString> InputMethodLoader::elemMap()
{
  return this->_elemMap;
}

QHash<QString, QString> InputMethodLoader::charMap()
{
  return this->_charMap;
}

QSet<QChar> InputMethodLoader::commitKey()
{
  return this->_commitKey;
}

QSet<QChar> InputMethodLoader::selKey()
{
  return this->_selKey;
}

InputMethodLoader* InputMethodLoader::getLoaderByName(QString name)
{
  if(name == "ChewingIMLoader")
    {
      return new ChewingIMLoader();
    }
  return NULL;
}
