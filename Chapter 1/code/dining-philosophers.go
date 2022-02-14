package main

import (
	"fmt"
	"time"
)

func main() {
	const n = 5  // number of philosophers
	const m = 10 // number of times each philosopher eats
	// make channels
	sticks := make([]chan struct{}, n)
	for i := 0; i < n; i++ {
		sticks[i] = make(chan struct{}, 1)
		sticks[i] <- struct{}{}
	}
	// making each philosopher hungry m times
	for i := 0; i < n; i++ {
		go func(i int) {
			stick1, stick2 := func() (chan struct{}, chan struct{}) {
				if i == n-1 {
					return sticks[n-1], sticks[0]
				} else {
					return sticks[i+1], sticks[i]
				}
			}()
			for j := 0; j < m; j++ {
				<-stick1
				fmt.Println("pickup stick1 | done by:", i)
				<-stick2
				fmt.Println("pickup stick2 | done by:", i)
				fmt.Println("Philosopher", i, "is eating")
				time.Sleep(1 * time.Millisecond)
				stick1 <- struct{}{}
				stick2 <- struct{}{}
			}
		}(i)
	}
	// didn't use waitgroup in order to keep it simple
	time.Sleep(100 * time.Millisecond)
}
