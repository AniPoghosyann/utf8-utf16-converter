# Data Structures in C++

This repository contains implementations of fundamental **data structures in C++**.  
All structures are implemented **from scratch without using STL containers**, focusing on understanding how the data structures work internally.

## Implemented Data Structures

### Linked List
A **templated doubly linked list** implementation.

Operations:
- `push_back()` – insert element at the end
- `pop_back()` – remove last element
- `print()` – display the list
- `operator[]` – access element by index

Features:
- Copy constructor
- Assignment operator
- Operator overloading (`<<`, `>>`, `++`, `--`)
- Exception handling for invalid index access

File: `LinkedList.cpp`

---

### Stack
A **templated stack implemented using a doubly linked list**.

Operations:
- `push()` – add element to stack
- `pop()` – remove top element
- `print()` – display stack contents
- `operator[]` – access element by index

Features:
- Custom exceptions (`EmptyStack`, `OutOfBounds`)
- Copy constructor and assignment operator
- Operator overloading (`<<`, `>>`, `++`, `--`)

File: `Stack.cpp`

---

### Binary Search Tree (BST)

A **templated Binary Search Tree storing key-value pairs**.

Operations:
- `insert(key, value)` – insert node
- `remove(key)` – remove node
- `findKey()` – search for a key
- `print()` – in-order traversal

Features:
- Deep copy support
- Operator overloading (`<<`, `>>`)
- Destructor for memory cleanup

File: `BST.cpp`

---

### Priority Queue

A **max priority queue implemented using a linked list**.

Operations:
- `push()` – insert element with priority
- `pop()` – remove highest priority element
- `top()` – return highest priority element
- `print()` – display queue contents

The queue keeps elements **sorted by priority**, so the highest priority element is always at the front.

File: `PriorityQueue.cpp`

---

## Project Structure

```
data_structures/
│
├── LinkedList.cpp
├── Stack.cpp
├── BST.cpp
├── PriorityQueue.cpp
└── README.md
```

---

## Compilation

Example using `g++`:

```
g++ LinkedList.cpp -o linkedlist
./linkedlist
```

You can compile other files similarly:

```
g++ Stack.cpp -o stack
g++ BST.cpp -o bst
g++ PriorityQueue.cpp -o pq
```

---

## Purpose

This project was created to practice:

- Object-Oriented Programming in C++
- Templates
- Operator Overloading
- Dynamic Memory Management
- Exception Handling
- Implementation of classic data structures
