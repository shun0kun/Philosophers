*This project has been created as part of the 42 curriculum by sshimots.*

# Philosophers

## Description

This project is an implementation of the **Dining Philosophers Problem**, a classic problem in concurrent algorithm design used to illustrate synchronization issues such as deadlock and starvation.

In this problem, philosophers sit around a circular table with a bowl of spaghetti in the center. There is one fork between each pair of adjacent philosophers. To eat, a philosopher must pick up both forks (one on each side). After eating, the philosopher puts down the forks and sleeps for a certain amount of time. After sleeping, the philosopher starts thinking and attempts to eat again.

The simulation continues until:
- A philosopher dies of starvation, or
- All philosophers have eaten the required number of times (if specified).

The goal of this project is to manage threads and mutexes efficiently to prevent race conditions and minimize starvation.

## Instructions

### Compilation

- `make` or `make all`
  Compiles the program and creates the executable `philo`.

- `make clean`
  Removes object files.

- `make fclean`
  Removes object files and the executable.

- `make re`
  Recompiles the program from scratch.

### Execution

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers`
  The number of philosophers (and forks).

- `time_to_die`
  A philosopher dies if they do not start eating within `time_to_die` milliseconds since the start of their last meal or the start of the simulation.

- `time_to_eat`
  The time (in milliseconds) it takes for a philosopher to eat. During that time, they must hold two forks.

- `time_to_sleep`
  The time a philosopher spends sleeping.

- `number_of_times_each_philosopher_must_eat` (optional argument)
  If all philosophers have eaten at least `number_of_times_each_philosopher_must_eat` times, the simulation stops.

The simulation stops when:
- A philosopher dies of starvation, or
- All philosophers have eaten the required number of times (if the optional argument is provided).

## Resources

### References

- [Dining philosophers problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

### Use of AI

- Understanding the Dining Philosophers Problem.
- Clarifying concepts related to threads, mutexes, and semaphores.
- Learning how to use the pthread library and semaphore library.
- Reviewing the program's logic and design.
- Exploring possible improvements and alternative implementation approaches.
- Improving the English sentences in this README.
