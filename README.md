# Stavanger
A Chess engine written in C++, Mainly a project for fun.

# Features

## Move Generation
- Hybrid board representation
  - Bitboards for piece/color combo + bitboards for total occupancy and occupancy by color.
  - 8x8 array for piece lookup by square.
- Magic bitboard lookup tables for sliding pieces + normal look up tables for king and knight.
- Pseudo legal move generation with check detection after attempting to make a move.
- Move make/unmake approach.

## Perft
- Perft(6) of starting position
  - 1.8s / 66.1 Mnodes/s (Intel Alder lake @ 5.1 GHz , 1 thread, no hashing, no bulk counting)
  - 0.6s / 200 Mnodes/s (with bulk counting, but gives incorrect results due to lack of check detection on leaf nodes)


