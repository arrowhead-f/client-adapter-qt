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
#ifndef FORMATTED_H
#define FORMATTED_H

#include <ostream>

struct FormattedText{
    enum Color{
        black      =  30,
        red        =  31,
        green      =  32,
        yellow     =  33,
        blue       =  34,
        magenta    =  35,
        cyan       =  36,
        white      =  37,
        not_change_color
    };

    enum Format{
         reset           =  0,
         bold            =  1,
         underline       =  4,
         inverse         =  7,
         bold_off        = 21,
         underline_off   = 24,
         inverse_off     = 27,
         not_change_format
    };

    Color text_color;
    Color background;
    Format format;

    static bool formatting_texts;

    FormattedText(Color text_color, Color background = not_change_color, Format format = not_change_format) :
             text_color{text_color},
             background{background},
             format{format}
    {

    }

    FormattedText(Color text_color, Format format):
        text_color{text_color},
        background{not_change_color},
        format{format} {}

    FormattedText(Format format = bold):
        text_color{not_change_color},
        background{not_change_color},
        format{format} {}

    operator std::string() const{
#if defined(__unix__) || defined(__unix)
        if(formatting_texts){
            std::string formatted;

            formatted += (text_color == FormattedText::not_change_color  ? "" : std::to_string(text_color));
            formatted += (background == FormattedText::not_change_color  ? "" : (formatted.empty() ? "" : ";") + std::to_string(background + 10));
            formatted += (format     == FormattedText::not_change_format ? "" : (formatted.empty() ? "" : ";") + std::to_string(format));

            return "\033[" + formatted + "m";
        } else {
            return "";
        }
#else
        //Other
        return "";
#endif
    }

    static std::string formatText(FormattedText format, std::string text){
        return (std::string)format + text + (std::string)FormattedText{reset};
    }

    static void setUsingFormattedTexts(bool usingFormattedText){
        formatting_texts = usingFormattedText;
    }
};


std::ostream& operator<<(std::ostream& os, const FormattedText& format);


#endif // FORMATTED_H
