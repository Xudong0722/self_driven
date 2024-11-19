/*
 * @Author: Xudong0722 
 * @Date: 2024-11-19 20:22:16 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-20 00:00:12
 */
package elvistest

import "testing"

func CreateSource(len int) []int {
	nums := make([]int, 0, len)

	for i := 0; i < len; i++  {
		nums = append(nums, i)
	}

	return nums
}

func Loop(nums []int, step int) {
	l := len(nums) 
	for i := 0; i < step; i++ {
		for j := i; j < l; j += step {
			nums[j] = 4
		}
	}
}

func BenchmarkLoopStep1(b *testing.B) {
	src := CreateSource(10000)

	b.ResetTimer();
	for i:=0; i < b.N; i++ {
		Loop(src, 1)
	}
}

func BenchmarkLoopStep2(b *testing.B) {
	src := CreateSource(10000)

	b.ResetTimer();
	for i:=0; i < b.N; i++ {
		Loop(src, 2)
	}
}

func BenchmarkLoopStep3(b *testing.B) {
	src := CreateSource(10000)

	b.ResetTimer();
	for i:=0; i < b.N; i++ {
		Loop(src, 3)
	}
}

func BenchmarkLoopStep4(b *testing.B) {
	src := CreateSource(10000)

	b.ResetTimer();
	for i:=0; i < b.N; i++ {
		Loop(src, 4)
	}
}

func BenchmarkLoopStep5(b *testing.B) {
	src := CreateSource(10000)

	b.ResetTimer();
	for i:=0; i < b.N; i++ {
		Loop(src, 5)
	}
}

func BenchmarkLoopStep6(b *testing.B) {
	src := CreateSource(10000)

	b.ResetTimer();
	for i:=0; i < b.N; i++ {
		Loop(src, 6)
	}
}

func BenchmarkLoopStep12(b *testing.B) {
	src := CreateSource(10000)

	b.ResetTimer();
	for i:=0; i < b.N; i++ {
		Loop(src, 12)
	}
}

func BenchmarkLoopStep16(b *testing.B) {
	src := CreateSource(10000)

	b.ResetTimer();
	for i:=0; i < b.N; i++ {
		Loop(src, 16)
	}
}