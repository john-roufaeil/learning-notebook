def removeVowels(*args):
  newStrs = []
  for str in args:
    newStr=""
    for char in str:
      if char.lower() not in ['a', 'e', 'o', 'i', 'u']:
        newStr += char
    newStrs.append(newStr)
  return newStrs

str1 = "mobile"
str2 = "laptop"
str3 = "mouse"
str4 = "aeoui"

res = removeVowels(str1, str2, str3, str4)

for str in res:
  print(str)