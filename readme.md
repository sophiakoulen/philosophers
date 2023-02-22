# philosophers

## tests
- ./philo 5 800 200 200: no philo should die
- ./philo 2 800 200 200: no philo should die
- ./philo 4 410 200 200: no philo should die

- ./philo 5 800 200 200 2: stop after each philo has had 2 meals
- ./philo 4 310 200 200: a philo should die, between 310 et 312ms
- ./philo 4 310 200 100: a philo dies
- ./philo 4 200 210 200: a philo dies, before 210ms


- ./philo 4 0 200 200: invalid argument
- ./philo 4 -500 200 200: invalid argument
- ./philo 4 310 200 200 -1: invalid argument
- ./philo 4 214748364732 200 200: invalid argument

is this okay?
- ./philo 3 1000 500 500: 1 philo dies after 1000ms
- ./philo 7 1000 300 300: 1 philo dies after 1900ms