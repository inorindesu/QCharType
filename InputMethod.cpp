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

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

#include "InputMethod.hpp"

void InputMethod::defaultSetup()
{
  for(int i = 0; i < 10; i++)
    {
      this->_selectionKeys << '0' + i;
    }
  this->_hasGrouping = false;
}

InputMethod::InputMethod(InputMethodLoader& loader)
{
  this->defaultSetup();
  if(loader.loaded())
    {
      this->_elementMap = loader.elemMap();
      this->_charMap = loader.charMap();
      this->_commitKeys = loader.commitKey();
      this->_selectionKeys = loader.selKey();
      this->_hasGrouping = loader.hasGrouping();
      this->_grouping = loader.groupingInfo();
    }
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

          // for example, ㄅ and ㄆ are in the same groups.
          // they cannot be in _inputBuffer at the same time
          // so, if the buffer is : ㄅㄧㄆ, it should be changed to ㄆㄧ
          if (this->_hasGrouping == true)
            {
              this->processGrouping();
            }

          if (this->_commitKeys.contains(c))
            {
              this->_candidates = this->_charMap.value(this->_inputBuffer, QString());
            }
        }
    }
  else // ready for selection. Handles page fliping only
    {
      if (c == ' ')
        {
          this->_currentPage += 1;
          if (this->_currentPage > this->candidatePageCount())
            {
              this->_currentPage = 0;
            }
        }
    }
}

const QString InputMethod::getCandidatesOfCurrentPage() const
{
  int c = this->candidatesPerPage();
  return this->_candidates.mid(this->_currentPage * c, c);
}

const QString InputMethod::getInputBuffer() const
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
  this->_currentPage = 0;
}

const QChar InputMethod::selectCharFromCandidates(int index)
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

const QChar InputMethod::selectCharFromCurrentPage(QChar key)
{
  int idx = this->_selectionKeys.indexOf(key);
  return this->selectCharFromCandidates(this->_currentPage * this->candidatesPerPage() + idx);
}

static QList<QStringList> loadIMListFile(QString dataDir, bool* error)
{
  *error = false;
  QList<QStringList> ret;

  QFileInfo imList(QDir(dataDir), "IMList.txt");
  if (imList.exists() == false)
    {
      qWarning() << "Wanrning: cannot find IMList.txt";
      *error = true;
      return ret;
    }

  QFile imListFile(imList.absoluteFilePath());
  if(imListFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
    {
      qWarning() << "Warning: cannot open IMList.txt";
      *error = true;
      return ret;
    }
  
  QTextStream s(&imListFile);
  int lineno = 0;
  while(s.atEnd() == false)
    {
      lineno += 1;
      QString line = s.readLine();
      
      // ignoring comment
      if (line.at(0) == '#')
        continue;
      
      QStringList items = line.split('\t');
      if (items.length() < 4)
        {
          qWarning() << "Warning: format error. The rest of data would be discarded.";
          *error = true;
          return ret;
        }
      ret.append(items);
    }

  imListFile.close();
  return ret;
}

QStringList InputMethod::getIMNameList(QString dataDir)
{
  QStringList ret;
  bool error;
  QList<QStringList> items = loadIMListFile(dataDir, &error);
  if (error)
    {
      qWarning() << "Warning: error occured while loading IMList.txt";
      return ret;
    }
  
  for(int i = 0; i < items.length(); i++)
    {
      ret.append(items.at(i)[0]);
    }
  return ret;
}

QHash<QString, QString> InputMethod::getShownameNamePairs(QString dataDir)
{
  QHash<QString, QString> ret;
  bool error;
  QList<QStringList> items = loadIMListFile(dataDir, &error);
  if (error)
    {
      qWarning() << "Warning: error occured while loading IMList.txt";
      return ret;
    }
  
  for(int i = 0; i < items.length(); i++)
    {
      QStringList item = items.at(i);
      ret.insert(item[3], item[0]);
    }
  return ret;
}

InputMethod* InputMethod::loadInputMethodByName(QString dataDir, QString s)
{
  bool error;
  QList<QStringList> items = loadIMListFile(dataDir, &error);
  for(int i = 0; i < items.length(); i++)
    {
      QStringList item = items.at(i);
      if (item[0] == s)
        {
          // IM found. Prepare to load it
          qDebug() << "IM" << item[0] << "found in IMList.txt";

          InputMethodLoader* loader = InputMethodLoader::getLoaderByName(item[2]);
          QString tableFilePath = item[1];
          QDir imeDataDir = QDir(dataDir);
          if(imeDataDir.cd("IME") == false)
            {
              qWarning() << "IME dir cannot be CDed.";
              return NULL;
            }
          QFileInfo tableFileInfo(imeDataDir, tableFilePath);
          if (tableFileInfo.exists() == false)
            {
              qWarning() << "Warning: cannot find designated IM file." << endl
                         << "(Designated path is:" << tableFileInfo.absoluteFilePath();
              return NULL;
            }

          QFile tableFile(tableFileInfo.absoluteFilePath());
          if (tableFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
            {
              qWarning() << "Warning: cannot open designated IM file";
              return NULL;
            }

          QTextStream stream(&tableFile);
          loader->loadData(stream);
          InputMethod* im = new InputMethod(*loader);
          tableFile.close();
          delete loader;
          return im;
        }
    }
  return NULL;
}

int maxLength(QList<QString> items)
{
  int m = -1;
  for(int i = 0; i < items.length(); i++)
    {
      int l = items.at(i).length();
      if (l > m)
        m = l;
    }
  return m;
}

void InputMethod::processGrouping()
{
  // key: group, value: key
  QHash<int, QString> objects;
  int maxKeyLength = maxLength(this->_grouping.keys());
  QString currentKey = QString(this->_inputBuffer.at(0));
  QString last = this->_inputBuffer.right(this->_inputBuffer.length() - 1);
  
  qDebug() << "[IM-G] Grouping started.";
  while(true)
    {
      if(this->_grouping.contains(currentKey))
        {
          int g = this->_grouping.value(currentKey, -1);
          qDebug() << "[IM-G] The key" << currentKey << "is in group" << g;
          objects.insert(g, currentKey);
          currentKey = QString(last.at(0));
        }
      else
        {
          qDebug() << "[IM-G] Key" << currentKey << "is not found";
          if (last.isEmpty())
            break;

          if (currentKey.length() >= maxKeyLength)
            {
              currentKey = QString(last.at(0));
            }
          else
            {
              currentKey += last.at(0);
            }
        }

      last = last.right(last.length() - 1);      
      if (last.isEmpty() && currentKey.isEmpty())
        break;
    }

  // reconstruct _inputBuffer
  QList<int> keys = objects.keys();
  qSort(keys);
  QString buf = "";
  for(int i = 0; i < keys.count(); i++)
    {
      int k = keys.at(i);
      if (k < 0)
        continue;
      buf.append(objects[k]);
    }
  this->_inputBuffer = buf;
}
