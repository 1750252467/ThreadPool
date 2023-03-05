#include "ThreadPool.h"
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class MyTask
{
public:
    void process()
    {
        //具体的业务逻辑随便定
        ::srand(::clock());
        int number = rand() %100;
        cout << ">>bo_threadPool.number = " << number << endl;
    }
};

void test()
{
    MyTask task;
    ThreadPool threadPool(5, 10);
    threadPool.start();

    int cnt = 20;
    while(cnt--> 0)
    {
        threadPool.addTask(std::bind(&MyTask::process, &task));
        cout << "cnt = " << cnt << endl;
    }
    threadPool.stop();
}

int main(int argc, char **argv)
{
    test();
    return 0;
}

