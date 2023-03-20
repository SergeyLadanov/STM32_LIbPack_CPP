#include "CallBack.hpp"

class A
{
private:
    GenericCallback<int16_t> *tesCallback = nullptr;
public:
    void SetCallBack(GenericCallback<int16_t> &in)
    {
        tesCallback = &in;
    }

    void Execute(void)
    {
        if (tesCallback)
        {
            tesCallback->execute(35);
        }   
    }
};



class B
{
public:
    A a;
    Callback<B, int16_t> itemSelectedCallBack;
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

    void TestHandler(int16_t a)
    {
        printf("Success!!\r\n");
    }
};


B test;


/*************************/

test.Proccess();