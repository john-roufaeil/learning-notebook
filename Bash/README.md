# Bash — Bourne Again Shell

A practical guide to Bash scripting for building a miniDBMS — a CSV-backed database shell with full input/output automation.

## Table of Contents

1. [How Bash Works](#1-how-bash-works)
2. [Variables and Environment](#2-variables-and-environment)
3. [Basic Commands](#3-basic-commands)
4. [Input and Output Redirection](#4-input-and-output-redirection)
5. [Text Processing — grep, sed, awk](#5-text-processing--grep-sed-awk)
6. [Test Operators](#6-test-operators)
7. [Branching — if/elif/else and case](#7-branching--ifelifelse-and-case)
8. [Loops](#8-loops)
9. [Arrays and Associative Arrays](#9-arrays-and-associative-arrays)
10. [Functions](#10-functions)
11. [Globbing and Wildcards](#11-globbing-and-wildcards)
12. [Shell Options and Extras](#12-shell-options-and-extras)
13. [miniDBMS Project](#13-minidbms-project)

## 1. How Bash Works

Bash is both an interactive command interpreter and a scripting language. When you run a script, Bash executes each line as if you typed it yourself. The `#!/bin/bash` shebang on line 1 tells the OS which interpreter to use — without it, your script may run under plain `sh`, which doesn't support Bash features like `[[`, arrays, or `(( ))`.

Every command produces two things: **output** (text written to the terminal) and an **exit status** — a number from `0` (success) to `255` (failure). `$?` holds the exit status of the last command. This is why `if some_command` works: `if` literally checks whether the command exited with `0`.

```bash
#!/bin/bash

# single-line comment

: '
multi-line comment
using the no-op colon command
'
```

## 2. Variables and Environment

```bash
name="Alice"       # no spaces around =
count=42           # still a string unless declared -i
declare -i count   # integer type
declare -r PI=3.14 # readonly
declare -l lower   # auto-lowercase on assign
declare -u upper   # auto-uppercase on assign
```

**No spaces around `=`** — `name = "Alice"` means the command `name` with arguments `= Alice`, which fails.

Always quote variables: `"$name"`. Without quotes, Bash splits the value on spaces and expands globs, which causes bugs when values contain spaces.

### Key Built-in Variables

| Variable | Meaning |
|---|---|
| `$0` | Script name |
| `$1` … `$9` | Script arguments |
| `$#` | Number of arguments |
| `$@` | All arguments as separate words |
| `$*` | All arguments as one string |
| `$?` | Exit status of last command |
| `$SHELL` | Current shell path |
| `$IFS` | Word separator (default: space/tab/newline) |
| `$REPLY` | Default variable for `read` with no name |

### `$IFS` — How Bash Splits Words

`$IFS` controls how unquoted strings are split into words. Change it for CSV parsing:

```bash
IFS=","
read -ra fields <<< "Alice,30,admin"
echo "${fields[0]}"  # Alice
```

Always restore `$IFS` after changing it, or scope the change to a single command.

### Parameter Expansion

Transform variables without external commands:

```bash
${var:-default}      # use default if var is empty/unset
${var:?message}      # exit with message if var is empty/unset
${#var}              # length of var
${var:2:4}           # substring: 4 chars starting at index 2
${var/old/new}       # replace first match
${var//old/new}      # replace all matches
${var^^}             # uppercase all
${var,,}             # lowercase all
${var#prefix}        # strip prefix
${var%suffix}        # strip suffix
```

```bash
# Enforce required argument with a helpful message
db_file="${1:?Usage: $0 <database_file>}"

# Lowercase the first CLI argument for case-insensitive matching
cmd="${1,,}"
```

## 3. Basic Commands

### `echo`

```bash
echo "Hello"         # print text + newline
echo -n "Hello"      # no trailing newline
echo -e "a\tb\n"     # interpret escape sequences (\t = tab, \n = newline)
```

### `printf` — Formatted Output (Preferred Over echo)

`printf` is more predictable than `echo` and follows C-style formatting:

```bash
printf "%-10s %5d\n" "Alice" 30   # left-align string, right-align number
printf "%s,%s\n" "id" "name"      # CSV row
```

### `cat`

```bash
cat file.txt            # print file contents
cat file1 file2         # concatenate files
cat -n file.txt         # with line numbers
```

### `read`

Reads one line from stdin and assigns it to a variable:

```bash
read name                    # read into $name
read -p "Enter name: " name  # -p: show a prompt
read -r line                 # -r: raw mode — don't interpret backslashes (almost always use -r)
read -a arr                  # -a: split into an array
read -s password             # -s: silent input (for passwords)
read -t 5 input              # -t: timeout in seconds
```

**Why `-r`?** Without it, a backslash in the input is treated as an escape character. With `-r`, what you type is what you get.

### `find`

Searches for files by name, type, size, or date:

```bash
find . -name "*.csv"              # find all CSV files
find ./data -type f               # regular files only
find . -name "*.log" -delete      # find and delete
find . -newer reference.txt       # files newer than a reference
```

## 4. Input and Output Redirection

Every process has three standard streams: **stdin (0)**, **stdout (1)**, **stderr (2)**. Redirection rewires these before a command runs.

### File Redirection

```bash
cmd > file       # write stdout to file (overwrite)
cmd >> file      # append stdout to file
cmd < file       # read stdin from file
cmd 2> err.log   # write stderr to file
cmd 2>&1         # redirect stderr to wherever stdout is going
cmd &> file      # stdout and stderr both to file
cmd > /dev/null  # discard stdout
```

**Order matters:** `cmd > file 2>&1` — first stdout goes to file, then stderr follows stdout (to file). Reversed: `cmd 2>&1 > file` — stderr goes to the terminal, stdout goes to file.

### Pipelines

A pipeline `|` connects the stdout of one command to the stdin of the next. Commands run concurrently.

```bash
cat users.csv | grep "admin" | cut -d, -f1
```

**Subshell warning:** Each stage of a pipeline runs in its own subshell. Variables assigned inside a pipeline stage are lost afterward:

```bash
count=0
cat file | while read line; do
    (( count++ ))   # modifies count in a subshell — lost after pipe ends
done
echo "$count"       # still 0!

# Fix: redirect a file into the loop directly
while IFS= read -r line; do
    (( count++ ))
done < file
```

### Here Documents (`<<`)

Feed multiple lines of text into a command's stdin without a temp file:

```bash
cat << EOF
Name: $name
Date: $(date)
EOF
# Variables ARE expanded. Use <<'EOF' to suppress expansion.
```

### Here Strings (`<<<`)

Feed a single variable or string into a command:

```bash
grep "Alice" <<< "$db_content"
tr 'a-z' 'A-Z' <<< "$username"
read -r first rest <<< "Alice 30 admin"
```

### Command Substitution `$()`

Captures the stdout of a command into a variable:

```bash
today=$(date +%Y-%m-%d)
count=$(wc -l < data.csv)
```

Use `$()`, not backticks — it's cleaner and can be nested.

### Input Redirection — Four Patterns

```bash
# 1. From a file
tr 'a-z' 'A-Z' < input.txt

# 2. From a here-doc (multiple lines)
while read line; do echo "$line"; done << EOF
line one
line two
EOF

# 3. From a here-string (single variable/string)
read -r val <<< "$my_var"

# 4. From a pipeline
echo "hello world" | tr 'a-z' 'A-Z'
```

## 5. Text Processing — grep, sed, awk

These three tools handle most text and data processing. Knowing when to use each saves time.

### `grep` — Filter Lines by Pattern

Use `grep` when you want to **find or filter lines**:

```bash
grep "Alice" file             # lines containing "Alice"
grep -i "alice" file          # case-insensitive
grep -v "Alice" file          # lines NOT containing "Alice"
grep -n "Alice" file          # with line numbers
grep -c "Alice" file          # count matching lines
grep -E "Alice|Bob" file      # extended regex: Alice OR Bob
grep -o "pattern" file        # print only the matched part
grep -q "Alice" file          # quiet: just set exit status (use in if)
```

```bash
# Check if a user exists in the database
if grep -q "^Alice," users.csv; then
    echo "User found"
fi
```

### `sed` — Transform Lines

Use `sed` when you want to **substitute, extract, or delete lines**:

```bash
sed 's/old/new/'   file    # replace first match per line
sed 's/old/new/g'  file    # replace all matches
sed 's/old/new/gi' file    # global + case-insensitive
sed '3d'           file    # delete line 3
sed '/pattern/d'   file    # delete lines matching pattern
sed -n '5,10p'     file    # print only lines 5–10
sed -i 's/old/new/g' file  # edit file in-place
sed -i.bak 's/old/new/g' file  # in-place with backup
sed -E 's/^([^,]*),([^,]*)/\2,\1/'  # swap first two CSV fields (ERE)
```

### `awk` — Process Fields

Use `awk` when you need to **work with columns, do arithmetic, or aggregate data**. `awk` splits each line into fields (`$1`, `$2`, ...) by a delimiter you choose. `NR` is the current line number.

```bash
awk '{ print $1 }'              file   # print first field
awk -F, '{ print $2 }'         file   # comma-delimited: print field 2
awk 'NR > 1'                   file   # skip header line
awk -F, '$3 == "admin"'        file   # filter by field value
awk 'END { print NR }'         file   # print total line count
awk -F, '{ sum += $2 } END { print sum }' file  # sum a column
```

Multi-line awk programs:

```bash
awk -F, '
    NR == 1 { next }           # skip header
    $3 == "admin" {
        count++
        print $1, $2
    }
    END { print count " admins" }
' users.csv
```

**Key awk variables:**

| Variable | Meaning |
|---|---|
| `$0` | Whole line |
| `$1`, `$2`, ... | Fields |
| `$NF` | Last field |
| `NR` | Line number (total) |
| `NF` | Number of fields in current line |
| `FS` | Field separator (set with `-F`) |

### Choosing the Right Tool

| Task | Tool |
|---|---|
| Find lines matching a pattern | `grep` |
| Count or filter by pattern | `grep` |
| Substitute text | `sed` |
| Delete or extract lines | `sed` |
| Work with specific columns | `awk` |
| Arithmetic or aggregation | `awk` |
| Simple field extraction | `cut` |

## 6. Test Operators

Bash has multiple ways to evaluate conditions. They behave differently and are not interchangeable.

### `[ ]` — POSIX Test

`[` is a command. Quoting is mandatory. Use for portable scripts.

```bash
[ "$a" = "$b" ]    # string equality (= not ==)
[ -z "$a" ]        # true if string is empty
[ -n "$a" ]        # true if string is non-empty
[ "$a" -eq "$b" ]  # numeric equality
[ "$a" -lt "$b" ]  # numeric less than
[ -f "$file" ]     # file exists and is a regular file
[ -d "$dir" ]      # directory exists
```

String: use `=` and `!=`. Numbers: use `-eq`, `-ne`, `-lt`, `-gt`, `-le`, `-ge`. The `=` operator does NOT do numeric comparison.

### `[[ ]]` — Bash Extended Test (Preferred)

`[[` is Bash syntax, not a command. Word splitting and glob expansion don't happen inside it, so quoting is optional (but still good practice). It supports `&&`, `||`, `=~` (regex), and glob patterns on the right side of `==`.

```bash
[[ "$a" == "$b" ]]        # string equality
[[ "$a" == p* ]]          # glob pattern match
[[ "$a" =~ ^[0-9]+$ ]]   # regex match (ERE)
[[ -f "$file" && -r "$file" ]]  # compound conditions
```

**Why wildcards work in `[[` but not `[`:** `[[` is handled by the shell itself, which knows the right side of `==` is a pattern. `[` is a command — the shell expands globs before passing them as arguments, so they match filenames, not strings.

### `(( ))` — Arithmetic

Evaluates integer arithmetic. Exit status is `0` (true) if result is non-zero.

```bash
(( a == b ))       # equality
(( a > b ))        # comparison
(( count++ ))      # increment
(( i % 2 == 0 ))  # modulo
```

Variables inside `(( ))` don't need `$` (though it works). `let "a = b + 2"` is an older equivalent — prefer `(( ))`.

### Summary

| Construct | Use for | Notes |
|---|---|---|
| `[ ]` | POSIX string/file/numeric tests | Requires quoting; `-eq` for numbers, `=` for strings |
| `[[ ]]` | Bash tests, patterns, regex | Preferred; supports `&&`, `\|\|`, `=~`, glob in `==` |
| `(( ))` | Integer arithmetic | No `$` needed for variables |

## 7. Branching — if/elif/else and case

### `if`

`if` checks the exit status of any command — not just `[ ]` tests.

```bash
if [[ -f "$db_file" ]]; then
    echo "Database exists"
elif [[ -d "$(dirname "$db_file")" ]]; then
    echo "Directory exists, creating database"
else
    echo "Invalid path"; exit 1
fi
```

### `case`

`case` matches a value against glob patterns. Cleaner than a chain of `if/elif` for command dispatching. `;;` ends each branch.

```bash
case "$command" in
    insert|INSERT)
        do_insert "$@" ;;
    select|SELECT)
        do_select "$@" ;;
    quit|exit|q)
        echo "Goodbye"; exit 0 ;;
    *)
        echo "Unknown command: $command" ;;
esac
```

`;;` is required to end each branch (unlike C's `switch`). `;&` falls through; `;;&` continues testing patterns.

## 8. Loops

### `for` — Iterate Over a List

```bash
for fruit in apple banana cherry; do
    echo "$fruit"
done

# Over a range
for i in {1..10}; do echo "$i"; done
for i in {1..50..2}; do echo "$i"; done   # step by 2

# Using seq (needed when using variables in the range)
for i in $(seq 1 "$step" "$end"); do echo "$i"; done
```

**Why `$(seq)` when you have variables?** Brace expansion like `{1..10}` is evaluated at parse time and only works with literal numbers. `$(seq 1 $n)` runs at runtime, so variables work.

### C-style `for`

```bash
for (( i=0; i<10; i++ )); do
    echo "$i"
done
```

### `while` and `until`

```bash
while [[ condition ]]; do ... done   # loop while true
until [[ condition ]]; do ... done   # loop until true (i.e., while false)

# Read a file line by line (correct idiom)
while IFS= read -r line; do
    echo "$line"
done < "$file"
```

`IFS=` before `read` preserves leading/trailing whitespace in each line.

### `break` and `continue`

```bash
for i in {1..10}; do
    [[ $i -eq 5 ]] && continue   # skip 5
    [[ $i -eq 8 ]] && break      # stop at 8
    echo "$i"
done
```

## 9. Arrays and Associative Arrays

### Indexed Arrays

```bash
fruits=("apple" "banana" "cherry")
fruits[3]="date"        # add at index 3
fruits+=("elderberry")  # append

echo "${fruits[0]}"         # first element
echo "${fruits[-1]}"        # last element
echo "${#fruits[@]}"        # length
echo "${fruits[@]}"         # all elements (as separate words)
echo "${fruits[*]}"         # all elements joined by IFS
echo "${!fruits[@]}"        # all indices
unset fruits[2]             # delete element (leaves a gap)
```

**`${arr[@]}` vs `${arr[*]}`:** Inside double quotes, `"${arr[@]}"` keeps each element as a separate word (safe when elements contain spaces). `"${arr[*]}"` joins all into one string. Use `"${arr[@]}"` for iteration; use `"${arr[*]}"` only when you need a joined string.

**`${arr[$@]}` and `${arr[$*]}`** use the script's positional parameters as indices — rarely useful, best avoided.

### Associative Arrays

Must be declared with `declare -A`. Keys are strings.

```bash
declare -A user
user["name"]="Alice"
user["age"]=30
user["role"]="admin"

echo "${user["name"]}"       # Alice
echo "${!user[@]}"           # all keys
echo "${#user[@]}"           # number of keys

for key in "${!user[@]}"; do
    echo "$key = ${user[$key]}"
done

[[ -v user["name"] ]] && echo "key exists"
unset user["age"]
```

### Conditions on Arrays

```bash
echo "${#names[@]}"                          # total elements
for name in "${names[@]}"; do ...            # iterate all elements
for key in "${!names[@]}"; do ...            # iterate all keys (associative)
```

## 10. Functions

Functions run in the current shell — they can read and modify variables. Use `local` to prevent leaking variables into the caller's scope.

```bash
greet() {
    local name="$1"
    echo "Hello, $name"
}

greet "Alice"
```

### Return Values

Functions return an exit status (0–255) via `return`, not a value. To return a string, print to stdout and capture it.

```bash
# Return via exit status (0 = found, 1 = not found)
user_exists() {
    grep -q "^${1}," "$DB_FILE"
}
if user_exists "Alice"; then echo "Found"; fi

# Return a value via stdout
get_role() {
    awk -F, -v name="$1" '$1==name { print $3 }' "$DB_FILE"
}
role=$(get_role "Alice")
```

## 11. Globbing and Wildcards

Globbing is filename expansion — the shell replaces a pattern with matching filenames before the command runs.

| Pattern | Matches |
|---|---|
| `*` | Zero or more characters |
| `?` | Exactly one character |
| `[abc]` | One of: a, b, or c |
| `[a-z]` | A character in the range a–z |
| `[!abc]` | Any character NOT in the set |

```bash
ls *.csv            # all CSV files
ls data?.csv        # data1.csv, dataA.csv — not data10.csv
ls [0-9][0-9].csv   # 00.csv through 99.csv
```

### Extended Globs (`shopt -s extglob`)

```bash
shopt -s extglob
ls !(*.bak)         # all files EXCEPT .bak files
```

| Pattern | Matches |
|---|---|
| `?(pat)` | 0 or 1 occurrence |
| `*(pat)` | 0 or more |
| `+(pat)` | 1 or more |
| `@(pat)` | exactly 1 |
| `!(pat)` | NOT matching |

**Globs vs regex:** Globs are for matching filenames and strings in `case`/`[[`. Regex (used in `=~` and `grep -E`) are more powerful but different syntax — `*` in a glob means "any characters", but `*` in a regex means "zero or more of the previous character".

### `LC_COLLATE=C`

In UTF-8 locales, `[a-z]` may include uppercase letters due to locale collation. Set `LC_COLLATE=C` to force ASCII byte-order ranges:

```bash
LC_COLLATE=C ls [a-z]*.csv   # only files starting with a lowercase ASCII letter
```

## 12. Shell Options and Extras

### `shopt` Options

```bash
shopt -s extglob    # enable extended glob patterns
shopt -s nullglob   # unmatched globs expand to empty string instead of literal
shopt -s dotglob    # * includes hidden files (dot-files)
shopt -s globstar   # ** matches any depth of subdirectories
```

### `set` Options

```bash
set -e              # exit on any error
set -u              # treat unset variables as errors
set -o pipefail     # pipeline fails if any stage fails
set -x              # debug: print each command before running it
```

Put `set -euo pipefail` at the top of serious scripts.

### Arithmetic with `let`

```bash
let "a = b + 2"    # older style — prefer (( a = b + 2 ))
```

### Quoting Prevents Special Char Expansion

```bash
echo "$var"         # variable value printed literally, no glob expansion
echo $var           # word-split and glob-expanded — risky
```
