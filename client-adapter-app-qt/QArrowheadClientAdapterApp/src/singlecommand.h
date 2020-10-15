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
#ifndef SINGLECOMMAND_H
#define SINGLECOMMAND_H

#include <map>
#include <regex>
#include <functional>
#include <string>
#include "command.h"
#include "ParameterType.h"
#include "TextTable.h"
#include "formattedtext.h"

class ParameterInfo{
public:
    ParameterType type;
    std::string description;

    ParameterInfo(ParameterType type = Text, std::string description = "")
        : type{type}, description{description} {}

    std::string getTypeName(){
        switch(type){
            case Number: return "number";
            case Text:   return "text";
            case TextList:  return "list";
            case TextMap: return "map";
        }
    }

};

class SingleCommand : public Command{
    std::map<std::string, ParameterInfo> mandatoryParameters;
    std::map<std::string, ParameterInfo> furtherParameters;
    std::function<CommandEffect(std::map<std::string, Parameter>)> action;

    void writeFullInfo(){
        if(mandatoryParameters.size()==0 && furtherParameters.size()==0)
            std::cout << "There is no available parameters";
        else
            std::cout << "Available parameters:" << std::endl;

        TextTable info(' ');

        info.setFormat(TextTable::FormattedTable::FIRST_COLUMN, FormattedText{FormattedText::blue, FormattedText::bold});

        for(auto& mandatory : mandatoryParameters){
            info.add("*" + mandatory.first/*+"["+ mandatory.second.getTypeName() +"]:"*/);
            info.add(mandatory.second.description);
            info.endOfRow();
        }

        for(auto& further : furtherParameters){
            info.add(further.first/*+"["+ further.second.getTypeName() +"]:"*/);
            info.add(further.second.description);
            info.endOfRow();
        }

        std::cout << info;
        std::cout << FormattedText{FormattedText{FormattedText::blue, FormattedText::bold}};
    }

    void writeShortInfo(){
        if((mandatoryParameters.size()>0 || furtherParameters.size()>0))
            std::cout << ": ";

        int not_first = 0;

        std::cout << FormattedText{FormattedText{FormattedText::reset}};

        for(auto& mandatory : mandatoryParameters)
            std::cout <<(not_first++ ? ", " : "") << FormattedText{FormattedText{FormattedText::bold}} << "*" << mandatory.first << FormattedText{FormattedText{FormattedText::reset}};;

        std::cout << FormattedText{FormattedText{FormattedText::reset}};

        for(auto& further : furtherParameters)
            std::cout << (not_first++ ? ", " : "") << further.first;

        std::cout << FormattedText{FormattedText::blue, FormattedText::bold};
    }

    void writeTreeInfo(int depth_level = 1) override {
        if(depth_level == 1)
            writeFullInfo();
        else
            writeShortInfo();
    }

    std::map<std::string, Parameter> parametersConvert(std::string all_parameter){
        if(all_parameter.empty())
            return {};

        std::string parameter_syntax("([\\w-]+)((\\[[\\w-.]+:[\\w-.]+\\])+|(\\[[\\w-.:\\\\/ ]+])+)");
        std::string parameters_syntax("^(" + parameter_syntax + " *)+$");

        if(!std::regex_search(all_parameter, std::regex(parameters_syntax)))
            throw std::runtime_error("Invalid syntax of parameters!\n"
                "The correct syntax of a paramter:\n"
                " parameter-name[value],\n"
                " parameter-name[key1:value1][key2:value2]...,\n"
                " parameter-name[value1][value2]...");

        std::map<std::string, Parameter> parameters;
        std::smatch match;
        while(std::regex_search(all_parameter, match, std::regex(parameter_syntax))){
            std::string name = match[1];
            Parameter value = convertValue(match[1], match[2]);

            if(!mandatoryParameters.count(name) && !furtherParameters.count(name))
                throw std::runtime_error("Unknown parameter name: " + name + "!");


            parameters.insert({name, value});
            all_parameter = match.suffix();
        }

        return parameters;
    }

    virtual void addReadmeAllCommands(TextTable& table, std::string command = "", int depth_level = 1) override{
        table.add(" **" + command + "** ");

        std::string parameters = "";
        int not_first = 0;

        for(auto& mandatory : mandatoryParameters)
             parameters += std::string(not_first++ ? ", " : "") + "**" + mandatory.first + "**";

        for(auto& further : furtherParameters)
            parameters += std::string(not_first++ ? ", " : "") + further.first;

        table.add(" " + parameters+" ");

        table.endOfRow();

    }

    void createReadme(QFile& file, std::string command_name = "", int depth_level = 1) override{
        Command::createReadme(file, command_name, depth_level);

        if(!(mandatoryParameters.size()==0 && furtherParameters.size()==0)){
            TextTable info('-', '|', '|');

            info.add("Parameter");
            info.add("Type");
            info.add("Description");

            info.endOfRow();

            for(auto& mandatory : mandatoryParameters){
                info.add("**" + mandatory.first + "**");
                info.add(mandatory.second.getTypeName());
                info.add(mandatory.second.description);
                info.endOfRow();
            }

            for(auto& further : furtherParameters){
                info.add("**" + further.first + "**");
                info.add(further.second.getTypeName());
                info.add(further.second.description);
                info.endOfRow();
            }

            std::stringstream ss_info;
            ss_info << info;

            file.write(ss_info.str().c_str());

        }

    }

    int convertValueToNumber(std::string name, std::string value){
        std::regex rgx("^\\[(\\d+)\\]$");
        std::smatch match;

        if(!std::regex_search(value, match, rgx))
            throw std::runtime_error("The " + name + "parameter value is number!");

        return std::stoi(match[1]);
    }

    std::map<std::string, std::string> convertValueToTextMap(std::string name, std::string value){
        std::regex rgx("\\[([\\w-.]+):([\\w-.]+)\\]");
        std::smatch match;

        std::map<std::string, std::string> map;

        while(std::regex_search(value, match, rgx)){
            map.insert({match[1], match[2]});
            value = match.suffix();
        }

        if(map.size()==0)
           throw std::runtime_error("The " + name + " parameter has map type. You should use it for exapmle: " + name + "[key1:value1][key2:value2]");

        return map;
    }

    std::vector<std::string> convertValueToTextList(std::string name, std::string value){
        std::regex rgx("\\[([\\w-.:\\\\/ ]+)\\]");
        std::smatch match;

        std::vector<std::string> list;

        while(std::regex_search(value, match, rgx)){
            list.push_back(match[1]);
            value = match.suffix();
        }

        if(list.size()==0)
           throw std::runtime_error("The " + name + " parameter has list type. You should use it exapmle: " + name + "[element1][element2]");

        return list;
    }

    Parameter convertValue(std::string name, std::string value){
        ParameterType type;

        if(mandatoryParameters.count(name))
            type = mandatoryParameters[name].type;
        else
            type = furtherParameters[name].type;

        switch(type){
            case Text:       return convertValueToTextList(name, value)[0];
            case Number:     return convertValueToNumber(name, value);
            case TextMap:    return convertValueToTextMap(name, value);
            case TextList:   return convertValueToTextList(name, value);
        }

        return value;
    }

public:
    SingleCommand(
            std::string description,
            std::map<std::string, ParameterInfo> mandatoryParameters,
            std::map<std::string, ParameterInfo> furtherParameters,
            std::function<CommandEffect(std::map<std::string, Parameter>)> action)
        : Command(description),
          mandatoryParameters(mandatoryParameters),
          furtherParameters(furtherParameters),
          action{action}{}

    CommandEffect exec(std::vector<std::string>::iterator& iter, std::vector<std::string>::iterator& end) override{
        //Write help
        if(Command::exec(iter, end) == CommandEffect::Exit)
            return CommandEffect::Continue;

        std::string all_parameters;

        //To one string (ther are some parameters which includes spaces)
        for(;iter!=end; iter++){
            all_parameters += all_parameters.empty() ? "" : " ";
            all_parameters += *iter;
        }

        //Eval parameters
        std::map<std::string, Parameter> parameters = parametersConvert(all_parameters);

        //Mandatory parameters are given
        int counter = 0;
        std::string missing_parameters = "";
        for(auto& mandatory_parameter : mandatoryParameters)
            if(!parameters.count(mandatory_parameter.first))
               missing_parameters += (counter++ == 0 ? "" : ", ") + mandatory_parameter.first;



        if(counter > 0)
            throw std::runtime_error("The " + missing_parameters + " mandatory parameter" +(counter==1?" is" : "s are") + " missing!");

        //Call the function
        return action(parameters);
    }

};
#endif // SINGLECOMMAND_H
