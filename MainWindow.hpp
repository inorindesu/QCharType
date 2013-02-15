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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimerEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QPixmap>
#include <QLabel>
#include <QDir>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTime>

#include "InputMethod.hpp"
#include "PaintableWidget.hpp"
#include "GameSettings.hpp"
#include "PaintableObject.hpp"
#include "NormalCharBlock.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  GameSettings* _settings;
  QPixmap _charBackground;
  PaintableWidget* _main;
  QLabel* _lblInput;
  QLabel* _lblScore;
  QLabel* _lblShield;
  
  QAction* _aNewGame;
  QAction* _aStopGame;
  QAction* _aExit;
  QAction* _aSettings;
  QAction* _aPauseGame;
  // game-related
  InputMethod* _im;
  bool _playing;
  int _timerId;
  bool _paused;
  QList<NormalCharBlock*> _charSprites;
  QString _commitedChars;
  QString _textDb;
  QHash<QString, QString> _textDbList; // name to path
  int _score;
  double _shield;
  int _hitCount;
  int _intervalHitCount;
  int _generationSpeed;
  float _fallSpeed;
  QFont _font;
  QTime _lastHitRecorded;
  QTime _lastGenerated;
  
  QDir getDataDir();
  QDir getUserDir();
  void setAccordingToSettings();
  void startGame();
  void endGame(bool showResult = true);
  void updateInputStatus();
  void setupMenubar();
  void setMenuAsPlaying();
  void setMenuAsStandingBy();
  void pause();
  void loadTextDb();
  void enumAllTextDb();
  bool haveToGenerateBlock();
  NormalCharBlock* generateCharBlock();
private slots:
  void paintCenterWidget(QPainter* p);
  void menuNewGame();
  void menuStopGame();
  void menuSettings();
  void menuExit();
  void menuPauseGame();
protected:
  void keyPressEvent(QKeyEvent* e);
  void timerEvent(QTimerEvent* e);
  void closeEvent(QCloseEvent* e);
public:
  MainWindow();
};

#endif
