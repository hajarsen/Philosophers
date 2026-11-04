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

### Threads and Shared Resources

Each philosopher runs in a separate POSIX thread. They share access to the same set of forks, so their execution order depends on the operating system's scheduler.

A **mutex** gives one thread exclusive access to a resource:

```text
Philosopher A          Shared fork           Philosopher B
      |                     |                      |
      +---- lock() -------->|                      |
      |                 [in use]                  |
      |                     |<------ lock() -------+
      |                     |       waits          |
      +---- unlock() ------>|                      |
      |                  [free]                    |
      |                     +---- acquired ------->+
```

The forks are represented by `pthread_mutex_t` objects in [`philosophers.h`](philosophers.h). The program acquires and releases them in [`lifecycle.c`](lifecycle.c).

### Avoiding Deadlock

If all five philosophers take their left fork first, they can end up in a circular wait:

```text
P1 waits for P2's fork
P2 waits for P3's fork
P3 waits for P4's fork
P4 waits for P5's fork
P5 waits for P1's fork
        -> deadlock
```

This implementation varies the fork acquisition order by philosopher ID: **even-numbered philosophers take the left fork first; odd-numbered philosophers take the right fork first**. Odd-numbered philosophers also start with a short delay to reduce immediate contention.

Breaking the uniform acquisition order helps prevent the simple circular-wait deadlock. However, avoiding deadlock alone does **not** guarantee that every philosopher gets enough time to eat.

The single-philosopher case is handled separately: with only one fork, that philosopher cannot eat and eventually reaches the death deadline.

See [`acquire_forks()`](lifecycle.c) and [`philo_routine()`](lifecycle.c).

### Starvation and Monitoring

The program records each philosopher's **last meal time**. A dedicated monitor regularly checks whether a philosopher has exceeded `time_to_die` without eating.

Conceptually, the time since the last meal is:

```text
elapsed = current_time - last_meal_time
```

When the deadline is exceeded and the philosopher is not marked as eating, the monitor sets the shared termination flag and reports a death.

When the optional meal target is provided, the monitor also checks whether **every philosopher** has eaten the requested number of times. If so, the simulation ends.

The checks are implemented in [`monitor.c`](monitor.c).

### Synchronization and Race Conditions

Multiple threads can read or update shared values at the same time. Without synchronization, one thread might see a value while another is in the middle of changing it.

This project uses separate mutexes for different responsibilities:

| Mutex | What it protects |
|---|---|
| Fork mutexes | Ownership of each shared fork |
| Print mutex | Status messages so they do not overlap |
| Death mutex | The shared stop/death flag |
| Meal mutex | Last-meal timestamps and meal counts |
| Per-philosopher eating mutex | The philosopher's eating state |

This division keeps unrelated operations from all competing for a single global lock. The shared structures are defined in [`philosophers.h`](philosophers.h), with setup in [`setup.c`](setup.c).

### Time and Simulation States

The program uses `gettimeofday()` to measure elapsed time in milliseconds and a custom `ft_usleep()` loop for short waits. The routine cycles through fork acquisition, eating, sleeping and thinking until the shared termination flag is set.

A typical message looks like:

```text
201 3 is eating
```

This means philosopher 3 began eating 201 milliseconds after the simulation's reference start time. The number is illustrative; exact scheduling and output can vary between runs.

See [`time.c`](time.c), [`lifecycle.c`](lifecycle.c) and [`utils_runtime.c`](utils_runtime.c).

### References

- [POSIX threads — Linux manual](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [pthread_mutex_lock(3p) — POSIX manual](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [gettimeofday(2) — Linux manual](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)
- [The Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [42 Philosophers Tester — dantonik](https://github.com/dantonik/42-philosophers-tester)
