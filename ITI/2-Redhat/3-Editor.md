# Vi Editor

- Default editor in all UNIX operating systems


## Commands

- h: left
- l: right
- j: down
- k: up
- i: insert before cursor
- I: insert from beginning of line
- a: insert after cursor
- A: insert at end of line
- x: delete cursor char
- X: backspace
- dd: delete line
- o: opens a new blank line below cursor
- O: opens a new blank line above cursor
- u: undo
- ctrl+r: redo
- w: forward one word
- b: back one word
- 0: go to beginning of line
- e: go to end of current word
- G: go to last line of file
- nG: go to line n
- ctrl+F: pages forward one screen
- ctrl+B: pages back one screen
- ctrl+L: refresh screen
- s: substitue a string for a character at the cursor
- dw: delete a word or part of word to right of cursor
- D: delete line from the cursor to right of cursor
- yy: copy
- dd: cut
- p: paste


- :w save file
- :wq save and exit
- :n goes to line n
- :set nu, :set nonu: show and hide line numbers
- :set ic, :setc noic: ignore or be case sensitive
- :set showmode, ;set noshowmode: display or turn off mode


```:%s/Linux/Unix/g```

```:1,3m7```

umask, alias & env vars are temporary (reset in new terminals)

man bash returns all shell variables

```env```, ```printenv``` show all env vars with values
```PS1, PS2, ...```

Environment in file -> user    environment
Environment in cli  -> session environment

alias c=clear
unalis c
\ls -> ignores alias "ls"

/home/user/.profile, /home/user/.bash_login, ...
/etc/.profile
/home/user.bashrc