## Hitori

I was spending Christmas alone (like always, and I don't really celebrate Christmas), so I decided to play [Hitori](https://en.wikipedia.org/wiki/Hitori). Now I don't consider myself an expert, but I've played this game before and I think I do well. I went on [hitoriconquest.com](hitoriconquest.com) and played a few games; apparently, most of the time, I'm only better than around 50% (~1:15 to 1:30s) of the players. Unacceptable! Rarely, I get 75% but that's not good enough.

You only need a handful of basic patterns. After that, it's simple scanning.
Maybe I need to scan better? Or perhaps, learn more complex patterns?

I couldn't dismiss the possibility of there being many bots. So I wrote a solver
to see how fast I would be if I already knew the solution.

The solver returns instantaneously and it takes around 35-40s to fill in the colors. And still, 10% of the players were better. So, just around 50 bots.

![Screenshot](https://github.com/recurze/Hitori/blob/master/ss.png?raw=true)

How fast are human solvers anyway? I can't find any videos of championships on YouTube.

Welp, I guess I need to do better.

## Stats

```
           puzzleID        time  percentile
count    200.000000  200.000000  200.000000
mean    8392.365000   63.900000   76.358500
std     4919.384974   23.855511   14.729225
min     2099.000000   29.000000   32.800000
25%     3333.250000   47.000000   69.425000
50%     7716.500000   59.000000   79.400000
75%    10118.750000   77.000000   87.850000
max    17613.000000  173.000000   98.400000
```

![Screenshot](https://github.com/recurze/Hitori/blob/master/time_hist.png?raw=true)
![Screenshot](https://github.com/recurze/Hitori/blob/master/percentile_hist.png?raw=true)
![Screenshot](https://github.com/recurze/Hitori/blob/master/scatter.png?raw=true)


Sometimes I submit too early and there were mistakes. I'm marked them in the file. It happened 33/200 times.

## Tips

* The goal is to color black. Specifically, the goal is not to color white. So don't bother.
* In addition to the common patterns, look at the corners and ensure no isolation.
* Don't check if there are duplicates, instead check if there empty spaces that can take more black squares.
* Usually, there is a unique solution.

## Conclusion

I was wrong to think that just because it takes x seconds to solve using a computer and input the solution, the faster times were from bots. My best score is (26s, 99.1%, id: 5898). So it's definitely possible that humans are just fast. (Doesn't mean there are bots too.)

![Screenshot](https://github.com/recurze/Hitori/blob/master/best.png?raw=true)

## TODO

* Update solver to solve backtrack.txt and backtrack_2.txt.
