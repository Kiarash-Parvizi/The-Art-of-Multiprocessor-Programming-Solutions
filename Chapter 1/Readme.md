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
import time
from multiprocessing import Process, Semaphore
n = 5  # number of philosophers
m = 10 # number of times each philosopher eats
# make semaphores
sticks = [Semaphore(1) for _ in range(n)]
procs = []
# making each philosopher hungry m times
for i in range(n):
    def f(i):
        stick1, stick2 = (sticks[i],sticks[0]) if i == n-1 else (sticks[i+1], sticks[i])
        for _ in range(m):
            stick1.acquire()
            print("pickup stick1 | done by:", i)
            stick2.acquire()
            print("pickup stick2 | done by:", i)
            print("Philosopher", i, "is eating")
            stick1.release()
            time.sleep(0.005)
            stick2.release()
    # create and start a new thread
    procs.append(Process(target = f, args = [i], daemon=True))
    procs[-1].start()

for proc in procs: proc.join()
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
const int n = 5;  // number of philosophers
const int m = 10; // number of times each philosopher eats
// make semaphores
sem_t* sticks = new sem_t[n];
for (int i = 0; i < n; i++) {
    sem_init(&sticks[i], 0, 1);
}
std::vector<std::thread> trds;
// making each philosopher hungry m times
for (int i = 0; i < n; i++) {
    // create, start and add a new thread
    trds.push_back(std::thread([m, i, sticks]() {
        int idx1 = i+1, idx2 = i;
        if (i == n-1) idx1 = i, idx2 = 0;
        auto &stick1 = sticks[idx1], &stick2 = sticks[idx2];
        for (int M = m; M--; ) {
            sem_wait(&stick1);
            printf("pickup stick1 | done by: %d\n", i);
            sem_wait(&stick2);
            printf("pickup stick2 | done by: %d\n", i);
            printf("Philosopher %d is eating\n", i);
            usleep(1);
            sem_post(&stick1);
            usleep(1);
            sem_post(&stick2);
        }
    }));
}
// join threads
for (auto& trd: trds) trd.join();
```
---

## Exercise 2:
For each of the following, state whether it is a safety or liveness property. Identify the bad or good thing of interest.
1. Patrons are served in the order they arrive: _safety_
- bad thing that will never happen: Patrons are not served in the order they arrive
2. What goes up must come down: _liveness_
- good thing to happen: It will eventually come down
3. If two or more processes are waiting to enter their critical sections, at least one succeeds: _liveness_
- good thing to happen: At least one process succeeds entering and makes progress
4. If an interrupt occurs, then a message is printed within one second: _safety_
- bad thing that will never happen: An occurrence that is not recorded in one second.
5. If an interrupt occurs, then a message is printed: _safety_
- bad thing that will never happen: An occurrence that is not recorded.
6. The cost of living never decreases: _safety_
- bad thing that will never happen: cost of living decreases
7. Two things are certain: death and taxes: _liveness_
- good thing to happen: taxes and dead
8. You can always tell a Harvard man: _safety_
- bad thing that will never happen: You can't tell a Harvard man
---

## Exercise 3:
In the producer–consumer fable, we assumed that Bob can see whether the can on Alice’s windowsill is up or down. Design a producer–consumer protocol using cans and strings that works even if Bob cannot see the state of Alice’s can (this is how real-world interrupt bits work).

### Solution:
Bob places a can on Alice's windowsill, ties one end of the string around the can and puts the other end in his house. Alice does the same thing so that she'll be able to notify Bob when pets finish eating and came back inside. Initially both cans are standing up, then Bob places some food in the yard and pulls his string.
<br/>
- From now on, when Alice wants to release the pets:
1. She waits until the can on her side is down
2. She releases the pets and resets the can on her windowsill
3. When the pets finish the food and return, she pulls her string, knocking down the can on Bob's windowsill.
<br/>
- Bob does the following:
1. He waits until the can on his side is down
2. He puts food in the yard and resets the can on his windowsill
3. He then pulls his string to notify Alice that there's food in the yard

---

## Exercise 4:
You are one of P recently arrested prisoners. The warden, a deranged computer scientist, makes the following announcement:
> You may meet together today and plan a strategy, but after today you will be in isolated cells and have no communication with one another.

> I have set up a “switch room” which contains a light switch, which is either on or off. The switch is not connected to anything.

> Every now and then, I will select one prisoner at random to enter the “switch room.” This prisoner may throw the switch (from on to off, or vice-versa), or may leave the switch unchanged. Nobody else will ever enter this room.

> Each prisoner will visit the switch room arbitrarily often. More precisely, for any N, eventually each of you will visit the switch room at least N times.

> At any time, any of you may declare: “we have all visited the switch room at least once.” If the claim is correct, I will set you free. If the claim is incorrect, I will feed all of you to the crocodiles. Choose wisely!

### Solution:
### <span style="color:#58CF8F">_The initial state of the switch is off_:</span>
The idea is to select one of them as consumer and the others as producers. They have to follow the following protocol to ensure that they'll be free:<br/>

Any producer when entering the room should:
* Turn on the light if {It was off & He hasn't turned it on before }
* Do nothing if {The lamp is already turned on}

The consumer when entering the room should:
* Turn off the light and count if { The lamp is on }, then if his mental number is equal to P-1, he should declare "all of us have been in the room at least once"
* Do nothing if {The lamp is off}

### <span style="color:#58CF8F">_You don't know the initial state of the switch_:</span>
Any producer when entering the room should:
* Turn on the light if {It was off & He has turned it on at most once before }
* Do nothing if {The lamp is already turned on}

The consumer when entering the room should:
* Turn off the light and count if { The lamp is on }, then if his mental number is equal to 2*(P-1), he should declare "all of us have been in the room at least once"
* Do nothing if {The lamp is off}

Why 2*(P-1)?<br/>
Because once he's reached this number he has either:
- counted all other prisoners twice
- or counted all other prisoners twice except for one which he's counted once. And the initial state of the lamp was 'on'

---
