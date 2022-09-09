#pragma once
#include <string>
#include <vector>

std::vector<std::string> Split(const std::string& string, char delimiter)
{
    std::vector<std::string> elements;
    std::string s(string);
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        elements.push_back(token);
        s.erase(0, pos+=1);
    }
    elements.push_back(s);
    return elements;
}

unsigned int StringToInt(std::string& in)
{
    unsigned int total = 0;
    for (int i = 0; i < in.length(); i++)
    {
        switch (in[i])
        {
        case '1':
            total += 1;
            break;
        case '2':
            total += 2;
            break;
        case '3':
            total += 3;
            break;
        case '4':
            total += 4;
            break;
        case '5':
            total += 5;
            break;
        case '6':
            total += 6;
            break;
        case '7':
            total += 7;
            break;
        case '8':
            total += 8;
            break;
        case '9':
            total += 9;
            break;
        case 'a':
            total += 10;
            break;
        case 'b':
            total += 11;
            break;
        case 'c':
            total += 12;
            break;
        case 'd':
            total += 13;
            break;
        case 'e':
            total += 14;
            break;
        case 'f':
            total += 15;
            break;
        }
    }
    return total;
}