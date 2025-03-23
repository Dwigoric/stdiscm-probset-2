## STDISCM Problem Set 2

This repository contains my attempt to solve Problem Set 2
concerning process synchronization using C++.

This is a simulation of a Looking For Group dungeon
queueing of an MMORPG.

To build this CMake project, you can use the following commands:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

This will create a `build` directory and compile the project.
You can run the simulation with the following command:

```bash
./ProblemSet2
```

If you have JetBrains CLion, you can simply open the project and run it from there.

### Configuration

After building the project, put the `config.txt` file in the `build` directory.
This file contains the configuration for the simulation. The format of the file is as follows:

```
n=<number of concurrent instances>
t=<number of tanks>
h=<number of healers>
d=<number of dps>
t1=<minimum instance completion time>
t2=<maximum instance completion time>
```