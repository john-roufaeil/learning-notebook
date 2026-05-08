def multiplicationTable(target):
  res = []
  for num in range(1, target + 1):
    numMultiples = []
    for multiplier in range(1, num + 1):
      numMultiples.append(num*multiplier)
    res.append(numMultiples)
  return res

res = multiplicationTable(5)
print(res)