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

#include "MainWindow.hpp"
#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QtDebug>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  
  QDir sharedDir = QDir(QCoreApplication::applicationDirPath());
  QString trDirPath = "../share/translations/";
  if (sharedDir.exists(trDirPath) == false)
    {
      qWarning() << "[main] translation is missing.";
    }
  else
    {
      if (sharedDir.cd(trDirPath) == false)
        {
          qWarning() << "[main] translation directory cannot be CDed into.";
        }
      else
        {
          QTranslator translator;
      
          if (translator.load("tr_" + QLocale::system().name(), sharedDir.absolutePath()) == false)
            {
              qWarning() << "[main] translation file cannot be loaded";
            }
          else
            {
              app.installTranslator(&translator);
            }
        }
    }

  MainWindow mw;
  mw.show();
  return app.exec();
}
