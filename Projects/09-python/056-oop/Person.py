class Person:
  moods = ("Happy", "Tired", "Lazy")

  def __init__(self, name, mood, money, healthRate):
    self.name = name
    self.mood = mood
    self.money = money
    self.healthRate = healthRate

  @property
  def name(self):
    return self._name
  @name.setter
  def name(self, value):
    if not isinstance(value, str):
      raise ValueError("Name must be a string.")
    self._name = value

  @property
  def mood(self):
    return self._mood
  @mood.setter
  def mood(self, value):
    if value not in Person.moods:
      raise ValueError(f"Mood must be one of {Person.moods}")
    self._mood = value

  @property
  def money(self):
    return self.__money
  @money.setter
  def money(self, value):
    if value < 0:
      raise ValueError("Money cannot be negative")
    self.__money = value
    
  @property
  def healthRate(self):
    return self.__healthRate
  @healthRate.setter
  def healthRate(self, value):
    if not isinstance(value, int):
      raise ValueError("Health rate must be an integer")
    if value > 100 or value < 0:
      raise ValueError("Health rate must be between 0 and 100")
    self.__healthRate = value

  def sleep(self, hours):
    if hours > 7:
      self.mood = "Lazy"
    elif hours < 7:
      self.mood = "Tired"
    else:
      self.mood = "Happy"
  
  def eat(self, meals):
    if meals >= 3:
      self.healthRate = 100
    elif meals == 2:
      self.healthRate = 75
    elif meals == 1:
      self.healthRate = 50
    else:
      raise ValueError("Meals must be positive")
  
  def buy(self, items):
    cost = items * 10
    if self.money < cost:
      raise ValueError("Not enough money to buy items")
    self.money -= items * 10
