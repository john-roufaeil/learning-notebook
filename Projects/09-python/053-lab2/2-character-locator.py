def charLocator(inputStr, letter):
  result = []
  for idx in range(len(inputStr)):
    if inputStr[idx] == letter:
      result.append(idx)
  return result

res = charLocator("This is not JavaScript", 'i')
print(res)