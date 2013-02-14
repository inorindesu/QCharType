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

#include <QPainter>
#include <QPalette>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QtDebug>

#include "MainWindow.hpp"
#include "InputMethodLoader.hpp"

MainWindow::MainWindow()
{
  //widget 
  QWidget* central = new QWidget();
  QVBoxLayout* mainLayout = new QVBoxLayout();

  this->_main = new PaintableWidget();
  mainLayout->addWidget(this->_main, 1);
  this->connect(this->_main, SIGNAL(paint(QPainter*)), this, SLOT(paintCenterWidget(QPainter*)));

  QHBoxLayout* statusLayout = new QHBoxLayout();
  
  this->_lblInput = new QLabel(tr("Ready"));
  this->_lblInput->setAlignment(Qt::AlignLeft);
  statusLayout->addWidget(this->_lblInput, 1);
  this->_lblShield = new QLabel("");
  this->_lblShield->setAlignment(Qt::AlignRight);
  statusLayout->addWidget(this->_lblShield);
  this->_lblScore = new QLabel("");
  this->_lblScore->setAlignment(Qt::AlignRight);
  statusLayout->addWidget(this->_lblScore);

  mainLayout->addLayout(statusLayout);

  central->setLayout(mainLayout);
  this->setCentralWidget(central);
  this->setupMenubar();
  this->resize(640, 480);

  // game setup
  this->_settings = new GameSettings();
  QDir userDir = this->getUserDir();
  QFileInfo userSettings(userDir, "settings.txt");
  if (userSettings.exists())
    {
      QFile* f = new QFile(userSettings.absoluteFilePath());
      if(f->open(QIODevice::ReadOnly | QIODevice::Text))
        {
          QTextStream* s = new QTextStream(f);
          this->_settings->load(*s);
          f->close();
          delete s;
        }
      else
        {
          qWarning() << "Warning: cannot open user settings (in" << userSettings.absoluteFilePath() << ")";
          qWarning() << "Default settings will be used";
        }
      delete f;
    }
  
  // some setup should be done according to this->_settings;
  setAccordingToSettings();

  this->_charBackground = QPixmap(32, 32);
  this->_charBackground.fill(QColor(0, 0, 0, 0));
  QPainter p(&this->_charBackground);
  p.setRenderHint(QPainter::Antialiasing, true);
  p.drawEllipse(0, 0, 32, 32);

  this->_im = NULL;
  this->_playing = false;
}

void MainWindow::paintCenterWidget(QPainter* p)
{
  p->drawPixmap(0, 0, this->_charBackground);
}

QDir MainWindow::getDataDir()
{
  QDir dataDir = QDir(QCoreApplication::applicationDirPath());
  if(dataDir.cd("../share/") == false)
    {
      qWarning() << "Warning: data directory cannot be found. The application may be crashed soon.";
    }
  return dataDir;
}

QDir MainWindow::getUserDir()
{
  QDir appDir = QCoreApplication::applicationDirPath();
  QFileInfo portableMark(appDir, "PORTABLE");
  if (portableMark.exists())
    {
      qDebug() << "Portable mark found!";
      if (appDir.exists("userData") == false)
        {
          if (appDir.mkdir("userData") == false)
            {
              qWarning() << "Warning: cannot make directory";
            }
        }
      if (appDir.cd("userData") == false)
        {
          qWarning() << "Warning: cannot CD to portable storage!";
        }
      return appDir;
    }
  else
    {
      QDir userDir = QDir::home();
#ifdef Q_WS_WIN
      QString dir = "./QCharType";
#else
      QString dir = "./.config/QCharType";
#endif
      qDebug() << "Related to dir " << userDir.absolutePath();

      if (userDir.exists(dir) == false)
        {
          qDebug() << "Making directory:" <<  dir << endl;
          if (userDir.mkpath(dir) == false)
            {
              qWarning() << "Warning: cannot make directory!";
            }
        }

      if (userDir.cd(dir) == false)
        {
          qWarning() << "Warning:" << dir  << "exists but cannot CD to the dir";
        }
      return userDir;
    }
  return QDir();
}

void MainWindow::closeEvent(QCloseEvent* e)
{
  QDir userDir = this->getUserDir();
  if (userDir.exists() == false)
    {
      qWarning() << "Warning: The dir should be created but not in place. Settings saving is aborted!";
      return;
    }
  QFileInfo userSettings(userDir, "settings.txt");
  QFile f(userDir.filePath("settings.txt"));
  if (f.open(QIODevice::WriteOnly | QIODevice::Text) == false)
    {
      qWarning() << "Warning: Cannot open settings file for writing!";
      return;
    }
  QTextStream s(&f);
  this->_settings->save(s);
  f.close();
}

void MainWindow::setAccordingToSettings()
{
  QPalette pal = this->_main->palette();
  pal.setColor(QPalette::Background, this->_settings->playgroundColor());
  this->_main->setPalette(pal);
  this->_main->setAutoFillBackground(true);
}

void MainWindow::startGame()
{
  qDebug() << "[MW] Game started";
  // reset game-related objects (IME .. etc)
  QString imName = this->_settings->inputMethodName();
  this->_im = InputMethod::loadInputMethodByName(this->getDataDir().absolutePath(), imName);
  if (this->_im == NULL)
    {
      qWarning() << "[MW] Warning: Game cannot be started due to inability of loading IM";
      return;
    }
  this->_commitedChars = "";
  // lockdown menuitems
  this->setMenuAsPlaying();
  // start timer
  this->_timerId = this->startTimer(25);  //40fps
  // start receiving input
  this->_playing = true;
  this->_paused = false;
}

void MainWindow::endGame(bool showResult)
{
  qDebug() << "[MW] Game ended.";
  // stop receiving input
  this->_playing = false;
  this->_paused = false;
  // stop timer
  this->killTimer(this->_timerId);
  // cleanup
  delete this->_im;
  this->_im = NULL;
  this->_charSprites.clear();
  // reopen menuitems
  this->setMenuAsStandingBy();
  // show game result
  if(showResult == true)
    {
    }
}

void MainWindow::timerEvent(QTimerEvent* ev)
{
  // check if some blocks were shot down
  for(int i = 0; i < this->_charSprites.length(); i++)
    {
      NormalCharBlock* b = this->_charSprites.at(i);
      int idx = this->_commitedChars.indexOf(b->character());
      if (idx != -1)
        {
          this->_commitedChars.remove(idx, 1);
          qDebug() << "[LOOP] character shotdown. (" << b->character() << ")";
        }
    }

  if (this->_commitedChars.isEmpty() == false)
    {
      qDebug() << "[LOOP] The following chars were not in the screen:" << this->_commitedChars;
      this->_commitedChars = QString("");
    }

  // check if some blocks were touched the ground
  // if shield cannot hold, the game is end immediately

  // calculate new positions for rest of blocks

  // generate new blocks

  // redraw
  this->_main->update();
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
  if (this->_playing)
    {
      int key = e->key();
      if (key == Qt::Key_Escape)
        {
          this->_im->clearStats();
        }

      if (this->_im->hasCharacter() == false)
        {
          if (key >= 'A' && key <= 'Z')
            {
              key += 'a' - 'A';
            }
      
          if ((key >= 'a' && key <= 'z') ||
              (key >= '0' && key <= '9') ||
              key == '-' || key == '=' || key == '[' || key == ']' ||
              key == '\\'  || key == '\'' || key == ';' || key == ',' ||
              key == '.' || key == '/' || key == '`' || key == ' ')
            {
              this->_im->input(key);
            }
        }
      else
        {
          if (key == ' ')
            {
              this->_im->input(' ');
            }
          else if(this->_im->isSelectionKey(key))
            {
              this->_im->selectCharFromCurrentPage(key);
            }
        }
      // update candidate status bar
      this->updateInputStatus();
    }
  QMainWindow::keyPressEvent(e);
}

void MainWindow::updateInputStatus()
{
  QString str = "";
  if(this->_im->hasCharacter())
    {
      // listing candidates
      QString candidates = this->_im->getCandidatesOfCurrentPage();
      QList<QChar> selectionKeys = this->_im->selectionKeys();
      int iT = qMin(candidates.length(), selectionKeys.length());
      for(int i = 0; i < iT; i++)
        {
          str += QString("%1:%2  ").arg(selectionKeys.at(i)).arg(candidates.at(i));
        }
    }
  else
    {
      // still input phoneic elements
      str += this->_im->getInputBuffer();
    }
  this->_lblInput->setText(str);
}

void MainWindow::setupMenubar()
{
  QMenuBar* menu = this->menuBar();
  QMenu* mGame = menu->addMenu(tr("Game"));
  this->_aNewGame = mGame->addAction(tr("New Game"), this, SLOT(menuNewGame()));
  this->_aStopGame = mGame->addAction(tr("Stop Game"), this, SLOT(menuStopGame()));
  this->_aPauseGame = mGame->addAction(tr("Pause Game"), this, SLOT(menuPauseGame()));
  this->_aExit = mGame->addAction(tr("Exit"), this, SLOT(menuExit()));
  this->_aSettings = menu->addAction(tr("Settings"), this, SLOT(menuSettings()));

  this->setMenuAsStandingBy();
  this->_aPauseGame->setVisible(false);
}

void MainWindow::setMenuAsStandingBy()
{
  this->_aPauseGame->setDisabled(true);
  this->_aStopGame->setDisabled(true);
  this->_aNewGame->setEnabled(true);
  this->_aExit->setEnabled(true);
  this->_aSettings->setEnabled(true);
}

void MainWindow::setMenuAsPlaying()
{
  this->_aPauseGame->setDisabled(false);
  this->_aStopGame->setDisabled(false);
  this->_aNewGame->setEnabled(false);
  this->_aExit->setEnabled(false);
  this->_aSettings->setEnabled(false);
}

void MainWindow::pause()
{
  // the time control when pause should be considered carefully!
}

void MainWindow::menuPauseGame()
{
  this->pause();
}

void MainWindow::menuNewGame()
{
  this->startGame();
}

void MainWindow::menuStopGame()
{
  this->endGame();
}

void MainWindow::menuExit()
{
  this->close();
}

void MainWindow::menuSettings()
{
  // call settings dialog.
  // need not to worry about the running game,
  // since settings menuitem can be only used when
  // the game is not running.
}
