/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleDeflateZipper.cpp
*/

#include <cstdio>
#include <iostream>
#include <sstream> 
#include <string>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include "ModuleDeflateZipper.hpp"

dems::CodeStatus ModuleDeflateZipper(dems::Context &context)
{
    auto &request = context.request;
    if (request.headers->getHeader("accept-encoding").find("deflate") != 0)
        return dems::CodeStatus::DECLINED;
    else
    {
        context.request.headers->setHeader("content-encoding", "deflate");

        std::stringstream compressed;
        std::stringstream new_body;
        new_body << request.body;
        boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
        out.push(boost::iostreams::zlib_compressor());
        out.push(new_body);
        boost::iostreams::copy(out, compressed);
        request.body = compressed.str();
        dprintf(context.socketFd, "%s", request.body.c_str());
    }
    return dems::CodeStatus::OK;
}

extern "C" {
    std::string registerHooks(dems::StageManager &manager)
    {
        manager.request().hookToEnd(10, "DeflateZipper", ModuleDeflateZipper);
        return "DeflateZipper";
    }
}