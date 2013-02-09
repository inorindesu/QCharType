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

#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
  this->_lblScore = new QLabel(QString::number(0));
  this->_lblScore->setAlignment(Qt::AlignRight);
  statusLayout->addWidget(this->_lblScore);

  mainLayout->addLayout(statusLayout);

  central->setLayout(mainLayout);
  this->setCentralWidget(central);
  this->resize(640, 480);

  // game setup
  this->_charBackground = QPixmap(32, 32);
  this->_charBackground.fill();
  QPainter p(&this->_charBackground);
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setRenderHint(QPainter::TextAntialiasing, true);
  p.drawEllipse(0, 0, 32, 32);

  this->_im = NULL;
}

void MainWindow::paintCenterWidget(QPainter* p)
{
  p->drawPixmap(0, 0, this->_charBackground);
}
