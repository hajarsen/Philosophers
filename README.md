*This project has been created as part of the 42 curriculum by hsennane.*

# Philosophers

## Description

**Philosophers** is a C project based on the *Dining Philosophers Problem*, a classic concurrency problem. Philosophers sit around a table, alternating between **thinking**, **eating**, and **sleeping**. Each philosopher needs two forks to eat, but adjacent philosophers share forks.

The challenge is to coordinate concurrent threads, protect shared data, and track deadlines while avoiding deadlocks and unnecessary contention.

This implementation uses **POSIX threads (`pthread`)**, mutexes, a dedicated monitoring thread, and millisecond-based timing.

## Instructions

### Building the Project

Compile the program using the included Makefile:

```bash
make
```

This builds the `philo` executable. Other available commands are `make clean`, `make fclean`, and `make re`.

### Running the Simulation

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

All time values are in **milliseconds**. The final argument is optional.

| Argument | Meaning |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) |
| `time_to_die` | Maximum interval between meals before a philosopher is considered dead |
| `time_to_eat` | Time spent eating |
| `time_to_sleep` | Time spent sleeping |
| `number_of_times_each_philosopher_must_eat` | Optional meal target for all philosophers |

Example with five philosophers:

```bash
./philo 5 800 200 200
```

Example with a meal limit:

```bash
./philo 5 800 200 200 7
```

Each status message includes the elapsed time, philosopher ID, and activity. The simulation stops when a philosopher dies or, when a meal target is specified, all philosophers reach that target.

The implementation accepts up to **200 philosophers** and requires the three timing arguments to be at least **60 ms**.

### Testing

An external testing tool is available at [dantonik/42-philosophers-tester](https://github.com/dantonik/42-philosophers-tester). To use it locally without adding its files to this repository:

```bash
make
git clone https://github.com/dantonik/42-philosophers-tester.git
cd 42-philosophers-tester
./test.sh
```

The tester accepts optional settings, including `-i` to choose iteration count. Failed cases are saved in its `fails/` directory. Refer to the [tester README](https://github.com/dantonik/42-philosophers-tester#readme) for additional options and its Docker-based Valgrind setup.

The presence of this testing reference does not imply that all tests have been passed.

## Resources

### The Dining Philosophers Problem

Each philosopher repeatedly follows the same cycle:

1. Take two forks.
2. Eat for `time_to_eat` milliseconds.
3. Release both forks.
4. Sleep for `time_to_sleep` milliseconds.
5. Think before attempting the next meal.

Two neighboring philosophers cannot use the same fork simultaneously. If every philosopher holds one fork while waiting for another, the system can enter a **deadlock**. A philosopher can also experience **starvation** if they cannot obtain forks before their deadline.

### Threads and Mutexes

Each philosopher runs in a separate `pthread` thread. A **mutex** protects each fork so that only one philosopher holds it at a time.

The implementation also uses mutexes to coordinate printing, meal-related state, and the shared termination flag. Fork acquisition order varies with philosopher ID to reduce simultaneous contention.

### Monitoring and Timing

A separate monitoring thread regularly checks when each philosopher last ate and whether the optional meal target has been reached.

Time is measured with `gettimeofday()`, and a custom millisecond sleep function combines elapsed-time checks with short `usleep()` intervals. Simulation output reports elapsed milliseconds from the start time.

### Shared State

The simulation stores configuration and shared synchronization objects in a common data structure. Each philosopher has its own ID, thread handle, meal count, last-meal timestamp, and references to its two forks.

Careful synchronization is necessary because multiple threads can read or modify related state concurrently.

### References

- [POSIX threads — Linux manual](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [pthread_mutex_lock(3p) — POSIX manual](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [gettimeofday(2) — Linux manual](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)
- [The Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [42 Philosophers Tester — dantonik](https://github.com/dantonik/42-philosophers-tester)
