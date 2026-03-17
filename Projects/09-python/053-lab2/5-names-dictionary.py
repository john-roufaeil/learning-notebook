def dictify(*names):
  result = {}
  names = sorted(names)
  for name in names:
    key = name[0].lower()
    if key not in result.keys():
      result[key] = []
    result[key].append(name)
  return result

res = dictify("John", "Ahmed", "Ali", "Belal")
print(res)