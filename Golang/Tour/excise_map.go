package main

import (
	"golang.org/x/tour/wc"
)

func WordCount(s string) map[string]int {
	//define result
	var result = make(map[string]int)
	//pass previous space
	var i, last int = 0, 0
	for ; i<len(s); i++{
		if s[i] != ' '{
			break	
		}
	}
	//cal
	last = i
	for ; i<len(s); i++{
		if s[i] == ' '{
			var tmp string = s[last:i]
			elem, ok := result[tmp]
			if ok {
				result[tmp] = elem + 1
			}else{
				result[tmp] = 1	
			}
			last = i + 1
		}
	}
	//Add the last one
	var last_str = s[last:len(s)]
	elem, ok := result[last_str]
	if ok {
		result[last_str] = elem + 1
	}else{
		result[last_str] = 1	
	}
	return result
}

func main() {
	wc.Test(WordCount)
}
