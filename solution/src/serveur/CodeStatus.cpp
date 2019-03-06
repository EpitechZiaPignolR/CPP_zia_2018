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
    dems::header::IHeaders::getWholeHeaders &head;
    auto &res = std::get<dems::header::Response>(code.response);

    std::string status_line = res.httpVersion + ' ' + res.message + ' ' + res.statusCode + "\r\n";
    status_line = 
    std::cout << status_line << std::endl;
}

int main(void)
{
}