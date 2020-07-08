# cpp-random-extras
Some random engines and distributions for C++17 which play nice with `<random>` and `Boost.Random`.

## xoshiro.hpp
A C++ implementation of the [xoshiro/xoroshiro](http://prng.di.unimi.it/) family of PRNGS.
Very fast non-cryptographic PRNGs with small state.

```
xoshiro256pp rng(31337); // xoshiro256++
xoroshiro64ss rng2(42); // xoroshiro64**
```

I didn't include jump()/long_jump(), but they'd be trivial to add with the same kind of template
specialization as is used for next().

## dice_distribution.hpp
A random distribution class for very basic dice expressions (`XdY` or `XdY +/- Z` only).
Intended to be fast to parse (no regex) and super fast to roll.

```
xoshiro256pp rng(42);
dice_distribution<int, boost::random::uniform_smallint<int>> dist("3d6 + 1");
fmt::print("roll = {}\n", dist(rng));
```
