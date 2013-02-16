#!/bin/bash
# 
# Copyright (C) 2013  Inori Sakura <inorindesu@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or (at
# your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

# Lazy script for updating translation files.

export LUPDATE=lupdate-qt4
export LRELEASE=lrelease-qt4

if [ ! -f "QCharType.pro" ]; then
    # I guess QCharType.pro is in the upper level?
    cd ..
    if [ ! -f "QCharType.pro" ]; then
        echo "Cannot lock on QCharType.pro!"
        exit
    fi
fi
${LUPDATE} ./QCharType.pro
${LRELEASE} ./QCharType.pro
mkdir -p ./share/translations
mv *.qm ./share/translations

