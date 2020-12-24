# Mars Lander
Solving Mars Lander puzzle from CodinGame website using Genetic Algorithm (RHEA - Rolling Horizon Genetic Algorithm to be exact). There is `merger` script in the `src` directory which merges all the files into one, letting us to submit the program on the CodinGame website. There are also seven testcases in `src/tests` directory, which allows to run the tests locally - every testcase is one map on which we have to land. There is also option to visualize the evolution run.

### Rules
Goal of this program is to control movement of the Mars lander - in each step we can rotate it by a degree from range $[-15, 15]$ (with limitation that rotation of the ship can be from <img src="https://render.githubusercontent.com/render/math?math=[-90, 90]">
 range, where 0 degree map to vertical position) and control the thrust of the engine - we can change thrust by $\{-1, 0, 1\}$ in every step (with limitation that thrust can be equal to any value from $\{0, 1, 2, 3, 4\}$). Moreover we have gravity on Mars which is equal to $3.711\frac{m}{s^2}$. Our objective is to succesfully land with our lander. There are some constraints in order for landing to be successfull:
* land on the flat ground
* land in a vertical position (tilt angle = $0^{\circ}$)
* vertical speed must be limited ($\le 40\frac{m}{s}$ in absolute value)
* horizontal speed must be limited ($\le 20\frac{m}{s}$ in absolute value)
* do not run out of the fuel which capacity is limited

Our main objective is of course (as previously mentioned) successfull landing, but we also want to use as little of fuel as possible.

### Clarification of usage
Use `Makefile` provided in `src` directory to build `mars-lander` binary. To exectute the program, run `./mars-lander`. If you want to specialize a different map, you can do this with `-m` option passed to the program, e.g. `./mars-lander -m 7` to run the 7-th map. Use verbose option `-v` to enable visualization as well - program uses SFML library for visualization. You can explore more options with `./mars-lander --help` command.

### CodinGame Mars Lander original puzzles
* Mars Lander Episode 1 - https://www.codingame.com/training/easy/mars-lander-episode-1
* Mars Lander Episode 2 - https://www.codingame.com/training/medium/mars-lander-episode-2
* Mars Lander Episode 3 - https://www.codingame.com/training/expert/mars-lander-episode-3

## Visualization
#### Map 7
![](doc/gifs/map7/giffed.gif)
#### Map 4
![](doc/gifs/map4/giffed.gif)
#### Map 5
![](doc/gifs/map5/giffed.gif)
