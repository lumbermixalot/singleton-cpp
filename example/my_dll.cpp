#include "my_dll.h"
#include <singleton-cpp/singleton.h>

MY_DLL_API void setValue() {

    IMyObject0* obj0 = Singleton<IMyObject0>::Get();
    if (!obj0)
    {
        printf("WTF0 at %s\n", __FUNCTION__);
        return;
    }
    obj0->SetValues(3, 4);

    IMyObject1* obj1 = Singleton<IMyObject1>::Get();
    if (!obj1)
    {
        printf("WTF1 at %s\n", __FUNCTION__);
        return;
    }
    obj1->SetString("test1234");
}
