# Intellect
A UCI compliant C++ Chess Engine
Not rated on CCRL as version 1 has not been published but this version would be around 2350 CCRL based on my testing. 

## Features
### Board Representation
* Plain Magic Bitboards
* Psuedolegal Move Generation
* Supports Castling, Promotion and En Passant
* 50 Move Rule and 3 Fold Repetition

### Search
* Alpha Beta Negamax Algorithm with PVS Search
* Iterative Deepening with Aspiration Windows
* Quiesce Search
**Search Efficiency**
* Transposition Tables
* Internal Iterative Deepening Reductions
* Reverse Futility Pruning/Static Null Move Pruning
* Null Move Pruning
* Razoring
* History Heuristics
* MVV LVA Move Ordering
* Late Move Pruning
* Futility Pruning
* Late Move Reductions
* Delta Pruning in Quiesce Search
* SEE Pruning in Quiesce Search

### Evaluation
* Tapered Evaluation
* Middlegame and Endgame Piece Square Tables
* Passed Pawn Bonus
* Mobility

## Future Improvements
There are still many improvements that I would like to make including: 
* NNUE Evaluation
* Search Parameter Tuning 
* Speeding Up Board Representation
