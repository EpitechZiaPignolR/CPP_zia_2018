/*
** EPITECH PROJECT, 2021
** zia
** File description:
** php.c
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <experimental/filesystem>
#include "api/Stage.hpp"
#include <iostream>

#define PROG "/usr/bin/php"
extern char **environ;

std::string     GetFilePath(dems::Context &context)
{
    namespace fs = std::experimental::filesystem;
    std::string requestFile;
    fs::path path;
    try {
        auto &requestFirstLine = std::get<dems::header::Request>(context.request.firstLine);
        requestFile = requestFirstLine.path;
        auto &serverConfig = std::get<dems::config::ConfigObject>(context.config["server"].v);
        fs::path root = std::get<std::string>(serverConfig["root"].v);
        if (root.is_absolute()) {
            path = root;
        } else {
            path = fs::current_path();
            path /= root;
        }
        if (!fs::exists(path))
            path = fs::current_path();
    }
    catch (std::exception &) {
        path = fs::current_path();
    }
    path += requestFile;
    if (!fs::is_regular_file(path)) {
        path /= "index.html";
        if (!fs::exists(path) || !fs::is_regular_file(path))
            return "";
    }
    return path;
}

int phpCaller(dems::Context &context)
{
    int pipefd[2];
    pipe(pipefd);
    dems::config::ConfigObject phpConf;
    std::string phpPath = PROG;
    std::string phpIniPath;

try  {
    auto &confModule = std::get<dems::config::ConfigObject>(context.config["modules"].v);
    phpConf = std::get<dems::config::ConfigObject>(context.config["PHP"].v);
    try {
        phpPath = std::get<std::string>(context.config["EXE"].v);
    } catch (std::exception &) {}
} catch (std::exception &) {}

    auto file = GetFilePath(context);
    char* a[] = { phpPath.data(), file.data(), nullptr };

    std::cout << a[0] << std::endl
    << a[1] << std::endl;

    int forkId = fork();
    if (forkId == -1)
    {
        printf("Fork failed\n");
        return (1);
    }
    else if (forkId == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        dup2(pipefd[1], 2);
        close(pipefd[1]);
        if (execve(phpPath.c_str(), a, environ) == -1)
          {
             printf("Execve failed: %s\n", strerror(errno));
             exit(1);
          }
    }
    else
    {
      uint8_t buffer[1024];

      close(pipefd[1]);
      int size = read(pipefd[0], buffer, sizeof(buffer));
      while (size > 0)
      {
          for (int i = 0; i < size; ++i)
            context.rawData.push_back(buffer[i]);
         size = read(pipefd[0], buffer, sizeof(buffer));
      }
    }
}

// std::vector<std::string>  GetEnvForPhp(dems::Context &context)
// {
//     std::vector<std::string> tmp_env(std::begin(environ), std::end(environ));
//
//     return tmp_env;
// }


dems::CodeStatus ModulePhp(dems::Context &context)
{
    phpCaller(context);
    return dems::CodeStatus::OK;
}

extern "C" {
std::string registerHooks(dems::StageManager &manager)
{
    manager.request().hookToMiddle(0, "php module", ModulePhp);
    return "PHP";
}
}
