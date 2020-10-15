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
#ifndef COMMAND_H
#define COMMAND_H

#include "formattedtext.h"

#include <string>
#include <map>
#include <variant>
#include <vector>
#include <iostream>
#include <QFile>
#include <TextTable.h>
#include <QString>




enum class CommandEffect{Exit, Continue};


class Command
{
    std::string description;
public:
    Command(std::string description) : description(std::move(description)) {}

    virtual void writeTreeInfo(int depth_level = 1) = 0;

    virtual void addReadmeAllCommands(TextTable& table, std::string command = "", int depth_level = 1) = 0;

    virtual void createReadme(QFile& file, std::string command_name = "", int depth_level = 1) {
        if(depth_level == 1){
            file.write("# client-adapter-app-qt\n");
            file.write("# QArrowheadClientAdapterApp\n");
            file.write("## User manual\n");

            file.write((QString(description.c_str()).replace("❯", ">").replace("<", "\\<").replace(">", "\\>").toStdString() + "\n\n").c_str());

            file.write("### Commands\n");

            TextTable table('-','|','|');
            table.add("Command Name");
            table.add("Parameters");
            table.endOfRow();

            addReadmeAllCommands(table);

            std::stringstream ss_table;
            ss_table << table;

            file.write(ss_table.str().c_str());

            file.write("\n:point_up: **Mandatory commands/parameters are bold**\n");
        } else {
            file.write((QString(description.c_str()).replace("❯", ">").replace("<", "\\<").replace(">", "\\>").toStdString() + "\n\n").c_str());
        }
    }

    virtual CommandEffect exec(std::vector<std::string>::iterator& iter, std::vector<std::string>::iterator& end){
        if(iter == end){
            return CommandEffect::Continue;
        }

        if(iter[0] == "?" || iter[0] == "help"){
            std::cout << description << std::endl;

            //Write all commands with parameters
            std::cout << std::endl;

            writeTreeInfo();

            return CommandEffect::Exit;
        }

        return CommandEffect::Continue;
    }
};

#endif // COMMAND_H
