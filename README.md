# Mars Lander
Solving Mars Lander puzzle from CodinGame website using Genetic Algorithm (RHEA - Rolling Horizon Genetic Algorithm to be exact). There is `merger` script in the `src` directory which merges all the files into one, letting us to submit the program on the CodinGame website. There are also seven testcases in `src/tests` directory, which allows to run the tests locally - every testcase is one map on which we have to land. There is also option to visualize the evolution run.

### Clarification of usage
Use `Makefile` provided in `src` directory to build `mars-lander` binary. To exectute the program, run `./mars-lander`. If you want to specialize a different map, you can do this with `-m` option passed to the program, e.g. `./mars-lander -m 7` to run the 7-th map. Use verbose option `-v` to enable visualization as well - program uses SFML library for visualization. You can explore more options with `./mars-lander --help` command.

### CodinGame Mars Lander original puzzles
* Mars Lander Episode 1 - https://www.codingame.com/training/easy/mars-lander-episode-1
* Mars Lander Episode 2 - https://www.codingame.com/training/medium/mars-lander-episode-2
* Mars Lander Episode 3 - https://www.codingame.com/training/expert/mars-lander-episode-3

## Screenshots
![](doc/gifs/giffed.gif)
