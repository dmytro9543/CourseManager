#pragma once
#include "teacher.h"

Teacher::Teacher(int id, const std::string& name)
    : id(id), name(name) {}

int Teacher::getId()
{
    return id;
}

const std::string& Teacher::getName()
{
    return name;
}
