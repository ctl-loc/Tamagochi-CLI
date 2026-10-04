# Terminal Tamagotchi: C Project Roadmap

## Goal 1: The Future-Proof Foundation
*Objective: Define the data structure and handle binary saving/loading with future compatibility.*
* **Subgoal 1.1:** Create a `Pet` struct containing `version`, `hunger`, `happiness`, and a `char reserved[64]` array.
* **Subgoal 1.2:** Write a function to save the struct to `pet.dat` in binary mode.
* **Subgoal 1.3:** Write a function to load from `pet.dat` (or initialize a new pet if the file doesn't exist).
> **Research Hint:** Look up `fread()` and `fwrite()` for binary I/O. The `reserved` array ensures the file size on disk stays constant when you add variables later.

## Goal 2: The passage of time (Offline Progression)
*Objective: Make the pet's stats change based on real-world time elapsed between closing and opening the app.*
* **Subgoal 2.1:** Add a `time_t last_saved;` variable to your struct (subtract from `reserved` size!).
* **Subgoal 2.2:** On load, get the current time and calculate how many seconds have passed since `last_saved`.
* **Subgoal 2.3:** Decrease hunger and happiness proportionally to the time elapsed (e.g., -1 point per 3600 seconds).
> **Research Hint:** Research `time(NULL)` to get current epoch time, and `difftime()` in `<time.h>` to calculate the difference between two timestamps.

## Goal 3: The Interactive Game Loop
*Objective: Turn the program into an actual game that stays open and accepts commands.*
* **Subgoal 3.1:** Create a `while` loop that clears the terminal and prints the pet's current stats.
* **Subgoal 3.2:** Implement a menu (e.g., "1: Feed, 2: Play, 3: Quit") and read user input.
* **Subgoal 3.3:** Update the stats based on the action, clamp values between 0-100, and save before quitting.
> **Research Hint:** Look up "ANSI escape sequence clear screen" (like `\033[2J`) to refresh the UI instead of printing new lines forever. 

## Goal 4: Version Migration (The Retrocompatibility Test)
*Objective: Update the game to v2.0 without breaking your existing save file.*
* **Subgoal 4.1:** Add an `int age;` to your struct. Shrink the `reserved` array again to compensate.
* **Subgoal 4.2:** Change your struct's default version to `2`.
* **Subgoal 4.3:** In your load function, check if the loaded version is `1`. If so, manually set `age = 0` and update the version to `2`.
> **Research Hint:** Because of the padding, `fread` will successfully read the v1 file into your v2 struct. You just need to initialize the new variables that were previously zeroed out bytes.

## Goal 5: Visuals and Polish
*Objective: Bring the pet to life using standard output formatting.*
* **Subgoal 5.1:** Write a function that prints different ASCII art based on the pet's state (e.g., sleeping, happy, starving).
* **Subgoal 5.2:** Add colors to the UI (e.g., Red for low stats, Green for high stats).
* **Subgoal 5.3:** Implement a realtime loop check so stats slowly decay even while the game is left open.
> **Research Hint:** Research "ANSI color codes in C" (e.g., `printf("\033[0;32m");` for green). For non-blocking input in standard C, look into `kbhit()` implementations for your OS.