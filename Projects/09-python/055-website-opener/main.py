import random
import webbrowser

URLs = [
  "https://www.udemy.com/",
  "https://www.youtube.com/",
  "https://github.com/"
  ]

def openRandomURL():
  randomURL = random.randint(0,len(URLs) - 1)
  webbrowser.open(URLs[randomURL])

openRandomURL()
