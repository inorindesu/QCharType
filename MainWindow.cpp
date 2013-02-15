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
  for(int i = 0; i < this->_charSprites.length(); i++)
    {
      this->_charSprites.at(i)->paint(p);
    }
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
  this->_shield = this->_settings->shieldStrength();
  this->_score = 0;
  this->_font = QFont(this->_settings->fontName(), this->_settings->fontSize());
  this->loadTextDb();
  this->_fallSpeed = this->_main->height() / this->_settings->secsToGround();
  qDebug() << "[MW] falling speed is set to" << this->_fallSpeed;
  // lockdown menuitems
  this->setMenuAsPlaying();
  // start timer
  this->_timerId = this->startTimer(25);  //40fps
  // start receiving input
  this->_playing = true;
  this->_paused = false;
  qsrand(QTime::currentTime().msecsTo(QTime(0, 0)));
  this->_lastHitRecorded = QTime();
  this->_lastGenerated = QTime();
  this->_lastFrame = QTime::currentTime();
}

void MainWindow::endGame(bool showResult)
{
  qDebug() << "[MW] Game ended.";
  // stop timer
  this->killTimer(this->_timerId);
  // stop receiving input
  this->_playing = false;
  this->_paused = false;
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
  if (this->_playing == false)
    {
      return;
    }
  // check if some blocks were shot down
  //qDebug("[Loop] Hit checker");
  for(int i = this->_charSprites.length() - 1; i >= 0; i--)
    {
      if (this->_playing == false)
        return;

      NormalCharBlock* block = this->_charSprites.at(i);
      QChar charInBlock = block->character();
      int idx = this->_commitedChars.indexOf(charInBlock);
      if (idx != -1)
        {
          this->_commitedChars.remove(idx, 1);
          this->_charSprites.removeAt(i);
          delete block;
          qDebug() << "[LOOP] character shotdown. (" << charInBlock << ")";
        }
    }

  if (this->_commitedChars.isEmpty() == false)
    {
      qDebug() << "[LOOP] The following chars were not in the screen:" << this->_commitedChars;
      this->_commitedChars = QString("");
    }

  //qDebug() << "[Loop] Block changer";
  float fallingDelta = this->_fallSpeed * this->_lastFrame.msecsTo(QTime::currentTime()) / 1000.0f;
  for (int i = this->_charSprites.length() - 1; i >= 0; i--)
    {
      if (this->_playing == false)
        return;

      // check if some blocks were touched the ground
      // if shield cannot hold, the game is end immediately
      NormalCharBlock* block = this->_charSprites.at(i);
      if (block->y() + block->width() >= this->_main->height())
        {
          this->_charSprites.removeAt(i);
          delete block;
          this->_shield -= 1;
          
          if (this->_shield <= 0)
            {
              this->endGame();
            }
        }
      // calculate new positions for rest of blocks
      //qDebug() << "[LOOP] delta:" << this->_fallSpeed * this->_lastFrame.msecsTo(QTime::currentTime()) / 1000.0f;
      block->changeY(fallingDelta);
    }

  if(this->_playing == false)
    {
      return;
    }

  // generate new blocks
  //qDebug() << "[Loop] Block Generator";
  if(this->haveToGenerateBlock())
    {
      this->_charSprites.prepend(generateCharBlock());
    }

  // redraw
  this->_lblShield->setText(QString::number(this->_shield, 'f', 1));
  this->_lblScore->setText(QString::number(this->_hitCount));
  this->_main->update();
  this->_lastFrame = QTime::currentTime();
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
              this->_commitedChars.append(this->_im->selectCharFromCurrentPage(key));
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
      qDebug() << candidates << endl << selectionKeys << iT;
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

void MainWindow::enumAllTextDb()
{
  this->_textDbList.clear();
  QDir txtDbDir = this->getDataDir();
  if (txtDbDir.cd("charDb") == false)
    {
      qWarning() << "Warning: cannot find character database dir.";
      return;
    }
  QStringList fileNames = txtDbDir.entryList(QDir::Files);
  for(int i = 0; i < fileNames.length(); i++)
    {
      QString fullFilePath = txtDbDir.filePath(fileNames.at(i));
      qDebug() << "[TDB] Loading" << fullFilePath;

      QFile f(fullFilePath);
      f.open(QIODevice::ReadOnly | QIODevice::Text);
      QTextStream stream(&f);
      while(true)
        {
          QString line = stream.readLine();
          if(line.at(0) == '#')
            continue;
          this->_textDbList.insert(line, fullFilePath);
          break;
        }
      f.close();
    }
  
  QList<QString> textDbKeys = this->_textDbList.keys();
  qDebug() << "[TDB] TextDBs found:";
  for(int i = 0; i < textDbKeys.length(); i++)
    {
      qDebug() << "[TDB]" << textDbKeys.at(i) << "in" << this->_textDbList.value(textDbKeys.at(i));
    }
}

void MainWindow::loadTextDb()
{
  this->_textDb = "";
  QDir txtDbDir = this->getDataDir();
  if (txtDbDir.cd("charDb") == false)
    {
      qWarning() << "Warning: cannot find character database dir.";
      return;
    }

  // list all files and get their corresponding names
  if (this->_textDbList.count() == 0)
    this->enumAllTextDb();

  // load textDb according to settings
  QString dbPath = this->_textDbList.value(this->_settings->textDatabaseName(), QString());
  if (dbPath.isNull())
    {
      qWarning() << "Warning: cannot find text database for" << this->_settings->textDatabaseName();
      return;
    }
  QFile f(dbPath);
  f.open(QIODevice::Text | QIODevice::ReadOnly);
  bool gotHeaderLine = false;
  QTextStream stream(&f);
  while(stream.atEnd() == false)
    {
      QString line = stream.readLine();
      if (line.at(0) == '#')
        {
          continue;
        }

      if (gotHeaderLine == false)
        {
          gotHeaderLine = true;
          continue;
        }
      this->_textDb.append(line);
    }
  f.close();
}

bool MainWindow::haveToGenerateBlock()
{
  if (this->_lastGenerated.isNull())
    return true;
  
  int msecs = this->_lastGenerated.msecsTo(QTime::currentTime());
  if (msecs >= 1000)
    return true;
  return false;
}

NormalCharBlock* MainWindow::generateCharBlock()
{
  int charIdx = qrand() % this->_textDb.length();
  QChar c = this->_textDb.at(charIdx);

  GameSettings* settings = this->_settings;
  float blockSize = settings->fontSize() * 2.0f;

  float x = qrand() % (this->_main->width() - (int)blockSize);
  this->_lastGenerated = QTime::currentTime();
  return new NormalCharBlock(blockSize, c, x, 0,
                             settings->backgroundColor(),
                             settings->foregroundColor(),
                             this->_font);
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
  if(this->_playing)
    {
      this->_fallSpeed = this->_main->height() / this->_settings->secsToGround();
    }
}
