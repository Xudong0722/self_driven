package main

import "golang.org/x/tour/pic"
import "math/rand"

func Pic(dx, dy int) [][]uint8 {
	var result = make([][]uint8, 0, 0)  //Need make to define
	for  i := 0; i<dy; i++ {  //short define
		var tmp = make([]uint8, 0, 0)
		for j := 0; j<dx; j++ {
			x := rand.Intn(255)
			y := rand.Intn(255)
			tmp = append(tmp, uint8(x^y))
		}
		result = append(result, tmp)
	}
	return result
}

func main() {
	pic.Show(Pic)
}
