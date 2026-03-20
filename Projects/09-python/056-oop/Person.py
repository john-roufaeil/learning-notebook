class Person:
  def __init__(self, name, mood, money, healthRate):
    self._name = name
    self._mood = mood
    self.__money = money
    self.__healthRate = healthRate

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
    self._mood = value

  @property
  def money(self):
    return self.__money
  @money.setter
  def money(self, value):
    self.__money = value
    
  @property
  def healthRate(self):
    return self.__healthRate
  @healthRate.setter
  def healthRate(self, value):
    self.__healthRate = value

  def sleep(self):
    pass
  
  def eat(self):
    pass
  
  def buy(self):
    pass
