# Python

Open source, general purpose programming language

compare in specific details compiled vs interpreted languages

Python is loosely typed language

Falsey values: None, False, 0, empty collections: "", (), [], {}

## Data Structures

### Lists

Python lists are mutable. This means you can change, add, or remove elements in a list after it has been created, without creating a new object in memory. 

- remove() takes the value
- pop() takes the index

### Tuple

Python tuples are immutable. This means that once a tuple is created, you cannot change its sequence of elements, add new items, or remove existing items.

### Dictionary

Python dictionary is mutable. This means you can modify the dictionary after it has been created by:
Adding new key-value pairs.
Updating the values for existing keys.
Removing key-value pairs

`name` in infoDict # True
infoDict.items() --> [(key, value), ...] list of tuples
infoDict.keys()
infoDict.update({'key':'value', 'key2':value2})

infoDict.get(index, defaultValue) 
infoDict[index]
infoDict['key2'] = infoDict.pop('key1')


## Functions

In Python, *args allows a function to accept an arbitrary number of positional arguments, which are collected into a tuple. It is used when the number of inputs is unknown or flexible. The asterisk (*) is the essential unpacking operator, while "args" is the conventional name

Normal arguments are passed as individual values, but when a function is defined to accept arbitrary positional arguments using *args, those arguments are collected into a tuple inside the function.
*args (arbitrary positional arguments) collects any number of extra positional arguments into a tuple.
**kwargs (arbitrary keyword arguments) collects any number of extra keyword arguments into a dictionary

for key, value in kwargs.items(): print(f'{key}: {value}')


##

list += flattens
list.append() keeps structure

```python
def multiplicationTable(target):
  res = []
  for num in range(1, target + 1):
    numMultiples = []
    for j in range(1, num + 1):
      numMultiples.append(j*num)
    res.append(numMultiples)
  return res

res = multiplicationTable(3)
print(res)
```

---

## Scopes

### Lexical Scope

global name

### Global Scope

### Nonlocal Scope

---

## Standard Library
The Python standard library is a comprehensive collection of pre-written modules and built-in functions that are included with every Python installation, allowing developers to perform a wide range of tasks without installing external packages. 
Real Python
Real Python
 +1
Often described as a "batteries included" language, Python provides a rich set of tools for common programming problems, including: 
Real Python
Real Python
 +1
System interaction: Modules like os, sys, and subprocess for interacting with the operating system, managing files and directories, and running external programs.
Data handling: Built-in support for various data formats, including json for JSON data, csv for comma-separated values, and sqlite3 for an embedded database.
Networking and internet: Modules such as urllib, http, and socket for fetching data from the web, creating web servers, and low-level networking.
Mathematics and data structures: Functionality provided by math, statistics, and specialized container types in the collections module.
Date and time: Modules like datetime and calendar for handling dates and times.
Text processing: Tools for working with text, including the re module for regular expressions and the string module for common string operations.
Testing and debugging: Utilities like unittest and doctest to help write and run tests for your code.
## Tips & Tricks

l = list(1,2,3)
any(l), all(l)

## Pipenv

python3 -m venv env
source env/bin/activate
deactivate
pip list
pip install <pckg>
pip install -r requirements.txt
pip freeze > requirements.txt

---

## OOP

```python
class Human:
  def __init__(self):
    print("Hi there")
man = Human()
```

|Aspect|Class Method|Static Method
|---|---|---|
|Decorator|@classmethod|@staticmethod|
|First Argument|Receives the class (cls) automatically|Receives no special first argument|
|Access to Data|Can access and modify class-level data|Cannot access class or instance data directly|
|Primary Use|Alternative constructors, methods needing class state modifications|Utility/helper functions related to the class namespace|

Use a class method when you need to perform an action that involves the class itself, but not a specific instance. A classic example is an alternative constructor, where you might pass data in a different format and use the class method to create a new instance of the class from that data.

Use a static method when you have a function that has a logical connection to the class, but doesn't need to access any class-specific or instance-specific data. It could essentially be a standalone function at the module level but is placed in the class for organizational purposes (namespacing).

### Inheritance

Python supports multiple inheritance, a feature of object-oriented programming that allows a class to inherit attributes and methods from more than one parent (base) class. This promotes code reuse and helps in combining functionalities from different sources. 

```python
class Mammal:
    def mammal_info(self):
        print("Mammals can give direct birth.")

class WingedAnimal:
    def winged_animal_info(self):
        print("Winged animals can flap.")

class Bat(Mammal, WingedAnimal):
    pass

# Create an object of the Bat class
b1 = Bat()
b1.mammal_info()
b1.winged_animal_info()

```

@property = getter?
@prop.setter


## Iterators

## Lambda

## Generators & Yield

## map & filter fns