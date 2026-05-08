def drawPyramid(num):
  for i in range(num):
    for j in range(num-i-1):
      print(" ", end="")
    
    for j in range(i+1):
      print("*", end="")
    print()

# 0 -> 3 spaces + 1 stars
# 1 -> 2 spaces + 2 stars
# 2 -> 1 spaces + 3 stars
# 3 -> 0 spaces + 4 stars
# num-i-1, i+1

drawPyramid(10)