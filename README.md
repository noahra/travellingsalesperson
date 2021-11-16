![alt text](salesman.png "Title")

<div>Icons made by <a href="https://www.flaticon.com/authors/flat-icons" title="Flat Icons">Flat Icons</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a></div>

This program solves the TSP-problem.
---

In order to run this program simply install your c++ compiler of your own choice and
execute the following commands in your terminal.
```console
foo@bar:~$ make main
foo@bar:~$./main
```

The code takes in an input as follows:
---

**INPUT**: An integer denoting amount of points, and the coordinates for each point.
Sample input can be seen in the box below.
```console
10
95.0129 61.5432
23.1139 79.1937
60.6843 92.1813
48.5982 73.8207
89.1299 17.6266
76.2097 40.5706
45.6468 93.5470
1.8504 91.6904
82.1407 41.0270
44.4703 89.3650
```
**OUTPUT**: 
The order for the smallest distance path between points from input. Sample output can be seen below.
```console
0
8
5
4
3
9
6
2
1
7
```