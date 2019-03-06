/*
** EPITECH PROJECT, 2019
** zia
** File description:
** code status
*/

#include <iostream>
#include <vector>
#include <variant>
#include "api/Stage.hpp"
#include "api/Heading.hpp"

dems::CodeStatus CodeStatu(dems::Context &code)
{
    std::string tmp;
    auto &res = std::get<dems::header::Response>(code.response);

    std::string status_line = res.httpVersion + ' ' + res.message + ' ' + res.statusCode + "\r\n";
    status_line += code.response.headers->getWholeHeaders() + code.response.body;

    for (auto &it : status_line)
        code.rawData.push_back(it);
}