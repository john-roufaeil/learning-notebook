# Problems

## 1. [Linked List Cycle](https://leetcode.com/problems/linked-list-cycle/description/?envType=problem-list-v2&envId=linked-list) (Easy)

> “If structure may loop, use relative speed to expose it.”

### Recognition Pattern

* Linked List + **cycle / revisit / loop**
  → think: **two pointers (slow & fast)**

### Core Idea

* Use **different speeds** to detect repetition
* You are detecting **behavior over time**, not nodes

### Technique

* slow → 1 step
* fast → 2 steps
* if they meet → **cycle exists**

### Loop Conditions (CRITICAL)

Every loop must define:

* **Success condition** → pointers meet
* **Failure condition** → fast reaches `None`

```python
while fast and fast.next:
```

### Common Mistakes

* Forgetting failure condition → infinite loop
* Using only one pointer
* Thinking in steps instead of **system behavior**

- LL + cycles/loops -> think of 2 pointers - relative motion reveals hidden structure (detecting behavior not nodes)
- Every loop should answer how to succeed or fail - if not the loop is incomplete

---

### Transferable Insight

Use this pattern when:

* Detecting cycles
* Finding middle of LL
* Finding intersection
* Any problem with **unknown repetition**

### Variations of same pattern

1. Detect cycle

slow = 1, fast = 2
meet → cycle

2. Find cycle start

reset one pointer to head
move both 1 step
meet → start

3. Find middle

fast hits end
slow is middle
