package main

import(
	"sync/atomic"
)

var flag int

func Reader(){
	for {
		if atomic.CompareAndSwap(&flag, 0, 1){
			atomic.Store(&flag, 0)
			return 
		}
	}
}