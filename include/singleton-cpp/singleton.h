#pragma once

#include <shared_mutex>
#include <memory>
#include <string>
#include <sstream>

#include <entt/core/type_info.hpp>

#include "singleton_api.h"

// Singleton mode for cpp
// 
// Features --
//   1. Works for both dynamical library and executable.
//   2. Multithread safe
// @param typeIndex Used as the unique identifier in the database.
// @param overWrite If true, @instancePtr will overwrite the stored instance pointer in the database.
//                  If false, it will behave as a read operation, regardless of the value of @instancePtr.
// @param instancePtr The instance pointer.
// @return A pointer/address to the unique instance.           
SINGLETON_API void* GetSharedInstance(entt::id_type typeIndex, bool overWrite, void* instancePtr);

template<typename T>
class Singleton final{
public:

    // A successul registration returns an empty string.
    // Otherwise returns an error message.
    static std::string Register(T* object);

    // Unregisters from the interface. An unregister must occur in the same module as the original Register
    //call; the pointer must match the original call to Register.
    static std::string Unregister(T* object);

    // Returns the registered interface pointer, if it exists.This method is thread - safe in that you can
    // call @ref Get from multiple threads.Likewise, calls to Register / Unregister are guarded, but it
    // is highly recommended to restrict Register / Unregister to a dedicated serial code path.
    static T* Get();

    /**
     * A helper utility RAII mixin class that will register / unregister within the constructor / destructor, respectively.
     *
     * Example Usage:
     * @code{.cpp}
     *      class System
     *          : public Singleton<ISystem>::Registrar
     *      {
     *      };
     * @endcode
     */
    class Registrar
        : public T
    {
    public:
        Registrar();
        virtual ~Registrar();
    };

private:
    static T*& GetInstance() {
        // Module-specific static pointer variable. This will require a map search operation
        // when invoked for the first time in a new module. There is one of these per module, but they
        // all point to the same internal pointer.
        static T* t;
        return t;
    }

    static std::shared_mutex s_mutex;
    static bool s_instanceAssigned;
};

template <typename T>
std::shared_mutex Singleton<T>::s_mutex;

template <typename T>
bool Singleton<T>::s_instanceAssigned = false;

template <typename T>
std::string Singleton<T>::Register(T* object)
{
    if (!object)
    {
        std::stringstream ss;
        ss << "Singleton '" << entt::type_name<T>::value() << "' is registering a null pointer!";
        return ss.str();
    }

    if (T* foundObject = Get())
    {
        std::stringstream ss;
        ss << "Singleton '" << entt::type_name<T>::value() << "' is already registered! [Found Ptr: 0x" << foundObject << "]!";
        return ss.str();
    }

    std::unique_lock<std::shared_mutex> lock(s_mutex);
    GetInstance() = reinterpret_cast<T*>(GetSharedInstance(entt::type_index<T>::value(), true, object));
    s_instanceAssigned = true;
    return "";
}

template <typename T>
std::string Singleton<T>::Unregister(T* object)
{
    if (!s_instanceAssigned)
    {
        std::stringstream ss;
        ss << "Singleton '" << entt::type_name<T>::value() << "' is not registered on this module!";
        return ss.str();
    }

    if (GetInstance() != object)
    {
        std::stringstream ss;
        ss << "Singleton '" << entt::type_name<T>::value() << "' is not the same instance that was registered! "
            << "[Expected '" << object << "', Found '" << GetInstance() << "']";
        return ss.str();
    }

    // Assign the internal pointer to null.
    std::unique_lock<std::shared_mutex> lock(s_mutex);
    GetInstance() = reinterpret_cast<T*>(GetSharedInstance(entt::type_index<T>::value(), true, nullptr));
    s_instanceAssigned = false;
    return "";
}

template <typename T>
T* Singleton<T>::Get()
{
    // First attempt to use the module-static reference; take a read lock to check it.
    // This is the fast path which won't block.
    {
        std::shared_lock<std::shared_mutex> lock(s_mutex);
        if (s_instanceAssigned)
        {
            return GetInstance();
        }
    }

    // If the instance doesn't exist (which means we could be in a different module),
    // take the full lock and request it.
    std::unique_lock<std::shared_mutex> lock(s_mutex);
    GetInstance() = reinterpret_cast<T*>(GetSharedInstance(entt::type_index<T>::value(), false, nullptr));
    s_instanceAssigned = static_cast<bool>(GetInstance());
    return GetInstance();
}

template <typename T>
Singleton<T>::Registrar::Registrar()
{
    Singleton<T>::Register(this);
}

template <typename T>
Singleton<T>::Registrar::~Registrar()
{
    Singleton<T>::Unregister(this);
}

