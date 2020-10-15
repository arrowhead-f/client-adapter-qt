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
#ifndef CONSOLAPP_H
#define CONSOLAPP_H

#include "command.h"
#include "singlecommand.h"
#include "multiplecommand.h"
#include "controller.h"
#include "formattedtext.h"

#include <QObject>
#include <any>
#include <iostream>
#include <functional>
#include <sstream>
#include <memory>
#include <tuple>
#include <variant>
#include <arrowheadserverexception.h>

class ConsoleApp : public QObject
{
    Q_OBJECT

    Controller controller;
    FormattedText highlighted_text;
    std::string welcome_message;
    MultipleCommand commands;


    std::string getContext(std::map<std::string, Parameter> parameters){
        std::string on = "";
        if(parameters.count(std::string("on")))
            on = std::get<Text>(parameters["on"]);
        return on;
    }

    template<typename T>
    Record<T> getRecordParameters(std::map<std::string, Parameter> parameters){
        Record<T> params;
        for(auto& parameter : parameters){
            if(parameter.first != "on"){
                switch(parameter.second.index()){
                    case Number: params.addField(T::fromString(parameter.first), std::get<Number>(parameter.second)); break;
                    case Text: params.addField(T::fromString(parameter.first), std::get<Text>(parameter.second)); break;
                    case TextList: params.addField(T::fromString(parameter.first), std::get<TextList>(parameter.second)); break;
                    case TextMap: params.addField(T::fromString(parameter.first), std::get<TextMap>(parameter.second)); break;
                }
            }
        }
        return params;
    }

    template<typename T>
    void showRecord(Record<T> record){
        TextTable table(' ');

        table.setFormat(TextTable::FormattedTable::FIRST_COLUMN, highlighted_text);

        for(T parameter : record.getParameters()){
            table.add(parameter.toString());
            table.add(record.toString(parameter));
            table.endOfRow();
        }

        std::cout << table;
    }

    template<typename T>
    void showRecords(std::vector<Record<T>> records){
        TextTable table('|');

        table.setFormat(TextTable::FormattedTable::FIRST_ROW, highlighted_text);

        if(records.size() == 0)
            throw std::runtime_error("0 match, please try an other regex!");

        //Write the header
        for(T parameter : records[0].getParameters())
            table.add(parameter.toString());

        table.endOfRow();

        //Write the values
        for(Record<T> record : records){
            for(T parameter : record.getParameters()){
                table.add(record.toString(parameter));
            }
            table.endOfRow();
        }

        std::cout << table;
    }

    template<typename T>
    std::vector<T> toParameterList(std::vector<std::string> list){
        std::vector<T> out;
        for(auto& parameter : list){
            out.push_back(T::fromString(parameter));
        }
        return out;
    }

    void writeContext(){
        std::cout <<
          FormattedText{FormattedText::blue, FormattedText::not_change_color, FormattedText::bold}
                  << controller.getContext()
                  << "❯ "
                  << FormattedText{FormattedText::reset};
    }

    void writeError(std::string error){
        std::cout
                << FormattedText{FormattedText::red, FormattedText::bold}
                << error
                << FormattedText{FormattedText::reset} << std::endl;
    }

    std::vector<std::string> ss;
public:
    explicit ConsoleApp(int argc, char* argv[], QObject *parent = nullptr);

    void start(){
        //FROM ARGS
        if(ss.size()>1){
            try{
                auto begin = ss.begin() + 1;
                auto end = ss.end();

                commands.exec(begin, end);
            } catch (const std::runtime_error& ex){
                writeError(ex.what());
            } catch (const ArrowheadServerException& server_ex){
                writeError(std::string("server: ") + server_ex.what());
            }
            return;
        }

        //CLI
        std::cout << welcome_message << std::endl;

        try {
            std::cout << "\n" << controller.getStatusMessage() << std::endl;
        } catch (const std::runtime_error& ex) {
            writeError(ex.what());
        } catch (const ArrowheadServerException& server_ex){
            writeError(std::string("server: ") + server_ex.what());
        }

        writeContext();

        std::string line;
        while(std::getline(std::cin, line)){
            try{
                std::istringstream iss(line);

                std::string command_word;
                std::vector<std::string> command_words;
                while(iss>>command_word)
                    command_words.push_back(command_word);

                auto begin = command_words.begin();
                auto end = command_words.end();

                if(commands.exec(begin, end) == CommandEffect::Exit)
                    break;

            } catch (const std::runtime_error& ex){
                writeError(ex.what());
            } catch (const ArrowheadServerException& server_ex){
                writeError(std::string("server: ") + server_ex.what());
            }

            writeContext();
        }
    }

#ifdef CREATE_DOCUMENTATION
    void createUserDocumentation(){
        QFile file((DOC_PATH + std::string("user_documentation.md")).c_str());

        file.open(QFile::WriteOnly | QFile::Truncate);
        commands.createReadme(file);
        file.close();

        std::cout << "The documentation file create at:" << std::endl;
        std::cout << file.fileName().toStdString() << std::endl;
    }
#endif

signals:

};

#endif // CONSOLAPP_H
