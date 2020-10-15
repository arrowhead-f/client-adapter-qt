/****************************************************************************
**
** Copyright 2020 evopro Innovation Kft.
**
** This file is part of arrowhead-f/client-adapter-qt.
**
** arrowhead-f/client-adapter-qt is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** arrowhead-f/client-adapter-qt is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with arrowhead-f/client-adapter-qt. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/
#ifndef PARAMETERTYPE_H
#define PARAMETERTYPE_H

#include <variant>
#include <string>
#include <vector>
#include <map>

enum ParameterType{Number=0, Text=1, TextMap=2, TextList=3};

using Parameter = std::variant<
    int,
    std::string,
    std::map<std::string, std::string>,
    std::vector<std::string>
>;

#endif // PARAMETERTYPE_H
