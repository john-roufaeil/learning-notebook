from Person import Person

class Employee(Person):
  def __init__(self, id, car, email, salary, distanceToWork, name, money, mood, healthRate):
    super().__init__(name, money, mood, healthRate)
    self.__id = id
    self._car = car
    self.__email = email
    self.__salary = salary
    self._distanceToWork = distanceToWork

  @property
  def id(self):
    return self.__id
  @id.setter
  def id(self, value):
    self.__id = value
  
  @property
  def car(self):
    return self._car
  @car.setter
  def car(self, value):
    if not isinstance(value, Car):
      self._car = None
      raise ValueError("Not a valid car object")
    self._car = value

  @property
  def email(self):
    return self.__email
  @email.setter
  def email(self, value):
    if not isinstance(value, str):
      raise ValueError("Email must be a string")
    self.__email = value.lower()
  
  @property
  def salary(self):
    return self.__salary
  @salary.setter
  def salary(self, value):
    if value < 0:
      raise ValueError("Salary cannot be negative")
    self.__salary = value

  @property
  def distanceToWork(self):
    return self._distanceToWork
  @distanceToWork.setter
  def distanceToWork(self, value):
    if value < 0:
      raise ValueError("Distance to work cannot be negative")
    self._distanceToWork = value

  def work(self):
    pass

  def drive(self):
    pass
  
  def refuel(self):
    pass
  
  def send_mail(self):
    pass