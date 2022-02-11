# Chapter 1

## Exercise 1:
The dining philosophers problem was invented by E. W. Dijkstra, a concurrency pioneer, to clarify the notions of deadlock and starvation freedom. Imagine ﬁve philosophers who spend their lives just thinking and feasting. They sit around a circular table with ﬁve chairs. The table has a big plate of rice. However, there are only ﬁve chopsticks (in the original formulation forks) available, as shown in Fig. 1.5. Each philosopher thinks. When he gets hungry, he sits down and picks up the two chopsticks that are closest to him. If a philosopher can pick up both chopsticks, he can eat for a while. After a philosopher ﬁnishes eating, he puts down the chopsticks and again starts to think.


## (Deadlock & Starvation)-free Solution:
### Algorithm1:
Each philosopher attempts to hold his right-side stick. If successful, he then attempts to do the same for the left-side stick
### Problem with this approach:
If all philosophers are holding their right-side stick, then no one can make another move, hence a deadlock occured.
### Algorithm2:
Each philosopher attempts to hold his right stick first, except for the last philosopher, he attempts to hold his left stick first. Then they attempt to pick their other stick.
### Deadlock-freedom proof
#### set of moves for each philosopher = {eating, picking up stick}<br>
suppose that no philosopher can make any move. Each one has picked at most 1 stick and can't pick any other. Then for the last philosopher there are 2 cases:
- case-1: he has picked no stick:<br/>
In this case the first philosopher is free to pick his left stick if he already has his right stick and since the second philosopher has picked at most one stick (stick to his right-side), he does have his right stick in hand, hence a contradiction
- case-2: he has picked his left stick<br/>
In this case he is free to pick his right stick, since the first philosopher has picked his right stick or nothing, hence a contradiction<br/>

## Implementation:

### Python
```py
```

### Go
```go
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
        stick1, stick2 := func() (chan struct{},chan struct{}) {
            if i == n-1 {
                return sticks[n-1], sticks[0]
            } else {
                return sticks[i+1], sticks[i]
            }
        }()
        for j := 0; j < m; j++ {
            <- stick1
            fmt.Println("pickup stick1 | done by:", i)
            <- stick2
            fmt.Println("pickup stick2 | done by:", i)
            fmt.Println("Philosopher", i, "is eating")
            time.Sleep(1*time.Millisecond)
            stick1 <- struct{}{}
            stick2 <- struct{}{}
        }
    }(i)
}
// didn't use waitgroup in order to keep it simple
time.Sleep(100*time.Millisecond)
```

### C++
```cpp
```
---

## Exercise 2:
