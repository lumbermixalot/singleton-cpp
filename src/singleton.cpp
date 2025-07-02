#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <mutex>

#include <singleton-cpp/singleton.h>

SINGLETON_API void* GetSharedInstance(
    const std::type_index& typeIndex,
    bool overWrite, void* instancePtr)
{
    static std::mutex s_databaseMutex;
    static std::unordered_map<std::type_index, void*> s_pointersDatabase;

    std::lock_guard<std::mutex> myLock(s_databaseMutex);

    if (!overWrite)
    {
        auto itor = s_pointersDatabase.find(typeIndex);
        if (itor != s_pointersDatabase.end())
            return itor->second;
        return nullptr;
    }

    if (instancePtr == nullptr)
    {
        auto itor = s_pointersDatabase.find(typeIndex);
        if (itor != s_pointersDatabase.end())
        {
            s_pointersDatabase.erase(itor);
        }
        return nullptr;
    }

    s_pointersDatabase[typeIndex] = instancePtr;
    return instancePtr;
}

