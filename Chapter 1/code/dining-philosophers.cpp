#include<bits/stdc++.h>
#include<semaphore.h> // POSIX semaphore

int main() {
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
}

