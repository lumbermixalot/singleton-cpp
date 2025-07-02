#include "my_dll.h"
#include "singleton-cpp/singleton.h"


class MyObject0 : public Singleton<IMyObject0>::Registrar {
public:
    void SetValues(int a, int b) override { m_a = a; m_b = b; }
    int GetA() const override { return m_a; }
    int GetB() const override { return m_b; }
private:
    int m_a = 0;
    int m_b = 0;
};

class MyObject1 : public Singleton<IMyObject1>::Registrar {
public:
    void SetString(const std::string& str) override { c = str; }
    const std::string& GetString() const override { return c; }
private:
    std::string c;
};


int main() {
    {
        MyObject0 obj0;
        MyObject1 obj1;

        // set value in dll
        setValue();

        // get value from exe
        IMyObject0* obj01 = Singleton<IMyObject0>::Get();
        if (obj01)
        {
            printf("Obj0 %d %d\n", obj01->GetA(), obj01->GetB());
        }
        else
        {
            printf("WTF 0!!\n");
        }

        IMyObject1* obj11 = Singleton<IMyObject1>::Get();
        if (obj11)
        {
            printf("Obj1 %s\n", obj11->GetString().c_str());
        }
        else
        {
            printf("WTF 1!!\n");
        }
    }

    // Test Unregister.
    {
        IMyObject0* obj01 = Singleton<IMyObject0>::Get();
        if (obj01)
        {
            printf("BAD Obj0 %d %d\n", obj01->GetA(), obj01->GetB());
        }
        else
        {
            printf("Good WTF 0!!\n");
        }

        IMyObject1* obj11 = Singleton<IMyObject1>::Get();
        if (obj11)
        {
            printf("BAD Obj1 %s\n", obj11->GetString().c_str());
        }
        else
        {
            printf("Good WTF 1!!\n");
        }
    }

    return 0;
}
