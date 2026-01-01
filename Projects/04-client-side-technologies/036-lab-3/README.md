# em vs rem

## em

- em is relative to its parent.
- it compounds; fontsize 10px, parent 2em, child 2em -> child = 40px
- can be used for padding/margin/buttons scaling with text but not for text itself (component internals)

## rem

- relative to root html element
- no compounding
- used in global scaling
- used for text and layout scaling