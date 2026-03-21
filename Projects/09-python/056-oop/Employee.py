import re
from Person import Person
from Car import Car

class Employee(Person):
  def __init__(self, id, car, email, salary, distanceToWork, name, money, mood, healthRate):
    super().__init__(name, mood, money, healthRate)
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
    if not re.match(r'^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$', value):
      raise ValueError("Invalid email")
    self.__email = value.lower()
  
  @property
  def salary(self):
    return self.__salary
  @salary.setter
  def salary(self, value):
    if value < 1000:
      raise ValueError("Salary must be 1000 or more")
    self.__salary = value

  @property
  def distanceToWork(self):
    return self._distanceToWork
  @distanceToWork.setter
  def distanceToWork(self, value):
    if value < 0:
      raise ValueError("Distance to work cannot be negative")
    self._distanceToWork = value

  def work(self, hours):
    if hours > 8:
      self.mood = "Tired"
    elif hours < 8:
      self.mood = "Lazy"
    else:
      self.mood = "Happy"

  def drive(self, velocity, distance):
    if not self.car:
      raise ValueError("Employee doesn't have a car")
    self.car.run(velocity, distance)
  
  def refuel(self, gasAmount):
    if not self.car:
      raise ValueError("Employee doesn't have a car")
    self.car.fuelRate = min(self.car.fuelRate + gasAmount, 100)
  
  def send_mail(self):
    pass

  def to_dict(self):
    return {
        "id": self.id,
        "name": self.name,
        "email": self.email,
        "salary": self.salary,
        "distanceToWork": self.distanceToWork,
        "money": self.money,
        "mood": self.mood,
        "healthRate": self.healthRate,
        "car": {
            "name": self.car.name,
            "fuelRate": self.car.fuelRate,
            "velocity": self.car.velocity
        } if self.car else None
    }