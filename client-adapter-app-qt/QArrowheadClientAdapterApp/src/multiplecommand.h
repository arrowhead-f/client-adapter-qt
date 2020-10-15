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
#ifndef MULTIPLECOMMAND_H
#define MULTIPLECOMMAND_H

#include <formattedtext.h>
#include <map>
#include "command.h"
#include <iostream>
#include <memory>

class MultipleCommand : public Command{
    std::map<std::string, std::shared_ptr<Command>> commands;


    void writeTreeInfo(int depth_level = 1) override {
        if(depth_level == 1){
            std::cout << FormattedText{FormattedText::blue, FormattedText::bold};
            std::cout << "Avilable commands:";
        }

        for(auto& command : commands){
            std::cout << "\n" << std::string(depth_level*3, ' ') /*<< FormattedText::formatText({FormattedText::blue, FormattedText::bold}, "❯ "+command.first)*/ << command.first;
            command.second->writeTreeInfo(depth_level+1);
        }

        if(depth_level == 1){
            std::cout << std::endl;
            std::cout << FormattedText{FormattedText::reset};
        }
    }

public:
    virtual void addReadmeAllCommands(TextTable& table, std::string command = "", int depth_level = 1) override{
        for(auto& c : commands){
            c.second->addReadmeAllCommands(table, command + (command.empty() ? "" : " ") + c.first, depth_level + 1);
        }

    }

    void createReadme(QFile& file, std::string command_name = "", int depth_level = 1) override{
        Command::createReadme(file, command_name, depth_level);

        for(auto& command : commands){
            file.write("\n");

            std::string subcommand = command.first;

            if(!command_name.empty()){
                command_name = (char)std::toupper(command_name[0]) + command_name.substr(1);
            } else if(!subcommand.empty()){
                subcommand = (char)std::toupper(subcommand[0]) + subcommand.substr(1);
            }

            file.write((std::string(depth_level+3, '#') + " " + command_name + (command_name.empty() ? "" : " ")+ subcommand + "\n").c_str());
            command.second->createReadme(file, command_name + (command_name.empty() ? "" : " ") + subcommand, depth_level +1);
        }
    }

    MultipleCommand(
            std::string description,
            std::map<std::string, std::shared_ptr<Command>> commands)
        : Command(description),
          commands(commands) {}


    CommandEffect exec(std::vector<std::string>::iterator& iter, std::vector<std::string>::iterator& end) override{
        if(iter == end){
            writeTreeInfo();
            return CommandEffect::Continue;
        }

        if(Command::exec(iter, end) == CommandEffect::Exit){
            return CommandEffect::Continue;
        }

        std::string name = *iter;

        for(auto& command : commands){
            if(command.first == name){
                return command.second->exec(++iter, end);
            }
        }

        throw std::runtime_error("Unkown command (" + name + ")!");
    }

};

#endif // MULTIPLECOMMAND_H
