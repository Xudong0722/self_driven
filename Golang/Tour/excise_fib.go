package main

import "fmt"

// fibonacci 是返回一个「返回一个 int 的函数」的函数
func fibonacci() func() int {
	var x, y int = 0, 1
	return func() int{  //bind x and y(store them? How to imple?)
		tmp := x
		x, y = y, (tmp+y)
		return tmp
	}
}

func main() {
	f := fibonacci()
	for i := 0; i < 10; i++ {
		fmt.Println(f())
	}
}
