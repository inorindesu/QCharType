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

#include "ResultDialog.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>

ResultDialog::ResultDialog(int score, int charCount, QList<QPair<QChar, QString> > missed)
{
  this->setWindowTitle(tr("Game ended"));
  this->setModal(true);

  QVBoxLayout* mainLayout = new QVBoxLayout();
  QLabel* lblCharCount = new QLabel();
  lblCharCount->setText(tr("Character cleared: %1").arg(charCount));
  mainLayout->addWidget(lblCharCount);
  
  QLabel* lblScore = new QLabel();
  lblScore->setText(tr("Score: %1").arg(score));
  mainLayout->addWidget(lblScore);

  QTableWidget* table = new QTableWidget(missed.length(), 2, this);
  table->setHorizontalHeaderLabels(
                                   QStringList()
                                   << tr("Character")
                                   << tr("Input sequence")
                                   );
  
  for(int i = 0; i < missed.length(); i++)
    {
      QPair<QChar, QString> item = missed.at(i);
      QTableWidgetItem* charItem = new QTableWidgetItem(QString(item.first));
      table->setItem(i, 0, charItem);
      QTableWidgetItem* seqItem = new QTableWidgetItem(item.second);
      table->setItem(i, 1, seqItem);
    }
  mainLayout->addWidget(table, 1);
  
  QHBoxLayout* closeButtonLayout = new QHBoxLayout();
  QPushButton* btnClose = new QPushButton(tr("Close"));
  QObject::connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
  closeButtonLayout->addStretch(1);
  closeButtonLayout->addWidget(btnClose);
  mainLayout->addLayout(closeButtonLayout);
  
  this->setLayout(mainLayout);
}

void ResultDialog::showResult(int score, int charCount, QList<QPair<QChar, QString> > missed)
{
  ResultDialog* di = new ResultDialog(score, charCount, missed);
  di->show();
}
