Here is the code that separates the list elements by commas:

```python
# Given list
lst = ["bli", "bla", "blub", "..."]

# Separating elements by commas
lst_string = ", ".join(lst)

# Print the result
print(lst_string)
```

Output:
```
bli, bla, blub, ...
```

In the code above, we use the `join()` method to concatenate the elements of the list `lst` into a single string, separated by commas. The resulting string is stored in the `lst_string` variable and then printed.