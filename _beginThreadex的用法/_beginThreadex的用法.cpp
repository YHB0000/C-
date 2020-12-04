// crt_begthrdex.cpp
// compile with: /MT
/*
    以下示例代码演示了如何通过同步API WaitForSingleObject使用_beginthreadex返回的线程句柄。 
    主线程在继续之前等待第二个线程终止。当第二个线程调用_endthreadex时，它将导致其线程对象进入
    信号状态。这允许主线程继续运行。不能使用_beginthread和_endthread来完成此操作，
    因为_endthread调用CloseHandle，它会在可以将其设置为信号状态之前破坏线程对象。
*/
#include <windows.h>
#include <stdio.h>
#include <process.h>

unsigned Counter;
unsigned __stdcall SecondThreadFunc(void* pArguments)
{
    printf("In second thread...\n");

    while (Counter < 1000000)
        Counter++;

    _endthreadex(0);
    return 0;
}

int main()
{
    HANDLE hThread;
    unsigned threadID;

    printf("Creating second thread...\n");

    // Create the second thread.
    hThread = (HANDLE)_beginthreadex(NULL, 0, &SecondThreadFunc, NULL, 0, &threadID);

    /*
        等待第二个线程终止。如果注释掉下面的行，Counter将不正确，
        因为线程尚未终止，而且Counter很可能还没有增加到1000000
    */
    WaitForSingleObject(hThread, INFINITE);
    printf("Counter should be 1000000; it is-> %d\n", Counter);
    // Destroy the thread object.
    CloseHandle(hThread);
}