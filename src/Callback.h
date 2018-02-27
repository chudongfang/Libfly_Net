


#include"routine.h"
#include<functional>

namespace libfly
{

class Routine;

//协程函数
typedef void *(*Routine_Function)( void * );


//定时器函数
typedef void *(*TimerCallback)( Routine * );

//EventLoop中执行的函数
typedef int (*EventLoop_Function)(void *);

typedef void *(* EpollCallback)(void *);



//-----------Net Library------------------------
typedef std::function<void (void *)> ConnectionCallback;
typedef std::function<void (const char* data,
                              ssize_t len)> MessageCallback;


}

