import asyncio
import random

async def produce(queue, n):
    for item in range(n):
        # 生产一个项目，使用sleep模拟I/O操作
        print(f'producing item {item} ->') 
        await asyncio.sleep(random.random())
        # 将项目放入队列
        await queue.put(item)
    # 指示生产完毕
    await queue.put(None)

async def consume(queue):
    while True:
        # 等待来自生产者的项目
        item = await queue.get()
        if item is None:
            break
        # 消费这个项目，使用sleep模拟I/O操作
        print(f'consuming item {item} <-')
        await asyncio.sleep(random.random()) 

async def main():
    queue = asyncio.Queue()
    task1 = asyncio.create_task(produce(queue, 10))
    task2 = asyncio.create_task(consume(queue))
    await task1
    await task2

asyncio.run(main())