
## co_yield
在协程中生成一个值并暂停协程，等待调用方获取并恢复协程

## co_await 

该操作的对象是一个可等待的对象，该对象可能包含如下几个函数：

  1. bool await_ready(), 在执行co_await的时候会判断，如果返回true则会继续执行，不会等待。否则会调用await_suspend
  2. ? await_suspend() , 在协程暂停的时候会调用这个函数，该函数接受一个std::coroutine_handle<>对象，在完成某个任务之后通过此对象调用resume
  3. ? await_resumt(), 当协程不需要执行或者恢复执行的时候，会调用此函数，返回值和co_await的返回值一致