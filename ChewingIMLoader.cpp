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

#include "ChewingIMLoader.hpp"
#include <QtDebug>
#include <QStringList>

void ChewingIMLoader::loadData(QTextStream& s)
{
  bool elemMode = false;
  bool charMode = false;
  bool groupMode = false;
  this->_name = "新酷音輸入法（精簡）";
  this->_hasGrouping = true;
  while(s.atEnd() == false)
    {
      QString line = s.readLine();
      if (line.startsWith("#"))
        continue;
      
      if (line.startsWith("%keyname"))
        {
          if (line.endsWith("begin"))
            {
              elemMode = true;
            }
          else if(line.endsWith("end"))
            {
              elemMode = false;
            }
        }
      else if(line.startsWith("%chardef"))
        {
          if (line.endsWith("begin"))
            {
              charMode = true;
            }
          else if(line.endsWith("end"))
            {
              charMode = false;
            }
        }
      else if(line.startsWith("%groupdef"))
        {
          if (line.endsWith("begin"))
            {
              groupMode = true;
            }
          else if (line.endsWith("end"))
            {
              groupMode = false;
              this->_loaded = true;
              return;
            }
        }
      else if(line.startsWith("%selkey"))
        {
          QString keys = line.split(' ', QString::SkipEmptyParts)[1];
          for (int i = 0; i < keys.length(); i++)
            {
              this->_selKey << keys.at(i);
            }
        }
      else if(line.startsWith("%endkey"))
        {
          QString keys = line.split(' ', QString::SkipEmptyParts)[1];
          for (int i = 0; i < keys.length(); i++)
            {
              this->_commitKey << keys.at(i);
            }
        }
      else
        {
          QList<QString> items = line.split(' ', QString::SkipEmptyParts);
          if (elemMode == true)
            {
              QChar elemKey = items[0][0];
              if (this->_elemMap.contains(elemKey) == false)
                {
                  this->_elemMap.insert(elemKey, items[1]);
                }
              else
                {
                  qWarning() << "GG: repeated element in elemMap. Key: " << elemKey << "Value:" << items[1]
                             << "Old value:" << this->_elemMap.value(elemKey);
                }
            }
          else if(charMode == true)
            {
              QString charKey = items[0];
              QString buffer = this->_charMap.value(charKey, QString(""));
              // QHash::insert would replace old items with new ones.
              this->_charMap.insert(charKey, buffer.append(items[1][0]));
            }
          else if(groupMode == true)
            {
              // the key is in phoneic symbol
              QString charKey = items[0];
              bool ok;
              int group = items[1].toInt(&ok);
              if (ok == false)
                {
                  qWarning() << "Warning : the group" << items[1] << "cannot be converted to an integer";
                  group = -1;
                }
              // get the corresponding key
              QChar key = this->_elemMap.key(charKey);
              this->_grouping.insert(key, group);
            }
        }
    }
}
