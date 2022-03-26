# Wordle solver
- inspired by [3blue1brown](https://youtu.be/v68zYyaEmEA)
- dataset from [jonhoo](https://github.com/jonhoo/roget)

## Build and run
Requirements
- `g++` with `c++20` support
- `openmp`

```
make
```

## Example output
- `O`: correct
- `X`: wrong
- `#`: misplaced

```
--------------------
Round 1: 12947 left.
#0: tares 17.0071
#1: lares 15.352
#2: rales 14.0519
#3: rates 13.3881
#4: teras 12.668
#5: nares 12.3016
#6: soare 12.0577
#7: tales 11.8548
#8: reais 11.6665
#9: tears 11.0116
Choose: #0        (<- this is where you choose the candidate)
Result: O#X##     (<- this is where you type in the result)
--------------------
Round 2: 4 left.
#0: tesla 0.0904208
#1: testa 0.0904208
#2: tsade 0.0847158
#3: tease 0.0847158
Choose: #0
Result: OOOOO
--------------------
Wordle 2/6
```
