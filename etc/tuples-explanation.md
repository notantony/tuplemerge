## How do we process tuples for rules & tables?

### Case 1: We add a new rule $r$ and there's no compatible table for it:
We need to create a new table with a tuple slighlty wider than the default one for the rule.
1. Check, if the rule is more strongly aligned with source or destination addresses.
	- If the source and destination addresses' lenghts are close together (within 4 bits), use both of them.
	- othewise, drop the smaller **and its port**.
2. Widen the chosen IP part depending on the prefix length.

|Prefix length|0..8|9..16|17..24|25..31|32|
|-------------|----|-----|------|------|--|
|Bits dropped |0   |1    |2     |3     |4 |

3. Drop empty **IP** fields. Drop **Port** fields with length $\leqslant$ 16. Drop **Proto** fields with length $\leqslant$ 24. 

Examples (IPs length only):
```
	(srcIP, trgIP):
    (6, 6) -> *use both* -> cut: (6, 6)
    (2, 6) -> *use both* -> cut: (2, 6)
    (2, 7) -> *use trg* -> cut: (0, 7)
    (9, 3) -> *use src* -> cut: (8, 0)
    (7, 29) -> *use both* -> (0, 26)
    (30, 30) -> *use both* -> (27, 27)
```

### Case 2: We add a rule and now there are $> c$ collisions in the table
This means that the current tuple is not specific enough. Now we want to create a separate table for all the rules collided by the key (there are exactly $c + 1$ of them in the table).
1. Find the max common tuple $m_\ell$ for the collided rules (i.e. take minimum over each dimension). Ususally it should be good enough, but we slightly modify it.
2. Find the dimension-wise max tuple $m_{max}$. Find the dimension $d$ with the greatest difference with the first tuple: $i: m_{max} [i] - m_\ell[i] \rightarrow max$.
3. Set $m_\ell[i] = (m_{max} [i] + m_\ell[i]) / 2$. Use this a final tuple.
4. Check if there's a table with such a tuple. If there's one, insert all the conflicted rules into it. Otherwise, create a new one, applying same dropping rules as in **3.** in the previous case.

Example:
```
Collided rules' tuples (SrcIP, TrgIP, SrcPort, TrgPort, Misc):
[
	(3, 5, 16, 16, 32),
	(6, 2, 16, 16, 32),
	(2, 4, 16, 16, 32),
	(2, 7, 16, 16, 32),
]
m_l = (2, 2, 16, 16, 32)
m_max = (6, 7, 16, 16, 32)
d = argmax(6 - 2, 7 - 2, 16 - 16, 16 - 16, 32 - 32) = 1
m_l[d] = (7 + 2) / 2 = 4 
final tuple: (2, 4, 16, 16, 32)
```
