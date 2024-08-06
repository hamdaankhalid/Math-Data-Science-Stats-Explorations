"""
Monty hall problem simulation

Perform a series of events and observe law of large numbers converge as the number of experiments increases.

Monty Hall Problem Setup
3 doors, one has a car, the other 2 have goats.

Each round of experiment is the following:
1. Guesser chooses a door at random.
2. Host then opens a door that is not car door for sure.
3. Now the guesser is given choice to switch from initial choice vs keeping initial choice.

The monty hall problem states that the P(car from switching choice) =  2/3
""" 
from random import randint

def simulation():
    # 0 is goat, 1 is car
    doors = [0, 0, 0]
    # insert a car at a random door
    car_idx = randint(0, 2)
    doors[car_idx] = 1
    # the guesser chooses a random door
    guessers_choice = randint(0, 2)
    # guest chooses one of the doors to open that is not guessers choice and is not car door
    # for simulation is this even a thing we care about?
    for i in range(0, 3):
        if (i != guessers_choice and i != car_idx):
            open_door = i

    # guesser does the switch between current choice and remaining choice
    for j in range(0, 3):
        if j == open_door or j == guessers_choice:
            continue
        guessers_choice = j
 
    return guessers_choice == car_idx

def main():
    # 1 Billion
    total_exp = 1_000_000_000
    # log per 100M exp
    log_cadence = 10_000_000
    total_wins = 0
    for i in range(total_exp):
        total_wins += simulation()
        if (i % log_cadence == 0):
            print(f"({i/log_cadence}) P of winning with switching {total_wins/(i+1)}, should approach 0.67777")

main()