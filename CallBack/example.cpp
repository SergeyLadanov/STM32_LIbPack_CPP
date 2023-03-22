#include "CallBack.hpp"

class A
{
private:
    GenericCallback<uint32_t, int16_t> *tesCallback = nullptr;
public:
    void SetCallBack(GenericCallback<uint32_t, int16_t> &in)
    {
        tesCallback = &in;
    }

    void Execute(void)
    {
        if (tesCallback)
        {
            if (tesCallback->isValid())
            {
                tesCallback->execute(79);
            }
        }   
    }
};



class B
{
public:
    A a;
    Callback<B, uint32_t, int16_t> itemSelectedCallBack;
public:
    B()
    :itemSelectedCallBack(this, &TestHandler)
    {
        a.SetCallBack(itemSelectedCallBack);
    }

    void Proccess(void)
    {
        a.Execute();
    }

    uint32_t TestHandler(int16_t a)
    {
        printf("Success!!\r\n");

        return 0;
    }
};




class A_Static
{
private:
    GenericCallback<uint32_t, int16_t> *tesCallback = nullptr;
public:
    void SetCallBack(GenericCallback<uint32_t, int16_t> &in)
    {
        tesCallback = &in;
    }

    void Execute(void)
    {
        if (tesCallback)
        {
            if (tesCallback->isValid())
            {
                tesCallback->execute(79);
            }
        }   
    }
};


B b;
A_Static a_static;


uint32_t TestHandler(int16_t a)
{
    printf("Callback from static!!\r\n");
    return 0;
}

CallbackStatic<uint32_t, int16_t> itemSelectedCallBack(TestHandler);
/*************************/

void TestCallBack(void)
{
    a_static.SetCallBack(itemSelectedCallBack);
    a_static.Execute();
    b.Proccess();
}
