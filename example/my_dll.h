#pragma once
#ifndef INC_MY_DLL_H_
#define INC_MY_DLL_H_

#include "my_dll_api.h"
#include <string>

class IMyObject0 {
public:
    virtual ~IMyObject0() = default;
    virtual void SetValues(int a, int b) = 0;
    virtual int GetA() const = 0;
    virtual int GetB() const = 0;
};

class IMyObject1 {
public:
    virtual ~IMyObject1() = default;
    virtual void SetString(const std::string& str) = 0;
    virtual const std::string& GetString() const = 0;
};

MY_DLL_API void setValue();


#endif
