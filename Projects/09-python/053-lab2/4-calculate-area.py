def calculateArea(shapeInitial, num1, num2=1):
  res = 0
  if (shapeInitial == "t"):
    res = 0.5 * num1 * num2
  elif (shapeInitial == "r"):
    res = num1 * num2
  elif (shapeInitial == "s"):
    res = num1 **2
  elif (shapeInitial == "c"):
    res = 22/7 * num1 ** 2
  return res

triangleArea = calculateArea('t', 10, 7)
rectangleArea = calculateArea('r', 10, 7)
squareArea = calculateArea('s', 10)
circleArea = calculateArea('c', 10)

print(triangleArea)
print(rectangleArea)
print(squareArea)
print(circleArea)