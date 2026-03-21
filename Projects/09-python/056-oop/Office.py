import json
from Employee import Employee

class Office:
  employeesNum = 0
  def __init__(self, name, employees):
    self._name = name
    self._employees = employees
    Office.employeesNum += len(employees)
  
  @property
  def name(self):
    return self._name
  @name.setter
  def name(self, value):
    if not isinstance(value, str):
      raise ValueError("Name must be a string")
    self._name = value.capitalize() if value else ""
  
  @property
  def employees(self):
    return self._employees
  @employees.setter
  def employees(self, value):
    self._employees = value
  
  def get_all_employees(self):
    return self._employees
  
  def get_employee(self, id):
    for e in self._employees:
      if e.id == id:
        return e
    return None

  def hire(self, employee):
    if not isinstance(employee, Employee):
      raise ValueError("Must hire an Employee object")
    self._employees.append(employee)
    Office.employeesNum += 1
  
  def fire(self, id):
    newEmployees = []
    for e in self.employees:
      if e.id != id:
        newEmployees.append(e)
    if len(self.employees) != len(newEmployees):
      Office.employeesNum -= 1
    self.employees = newEmployees
  
  def deduct(self, id, deduction):
    emp = self.get_employee(id)
    if not emp:
      return
    emp.salary -= deduction

  def reward(self, id, reward):
    emp = eslf.get_employee(id):
    if not emp:
      return
    emp.salary += reward

  def check_lateness(self, id, moveHour):
    emp = self.get_employee(id)
    if not emp:
        return

    isLate = Office.calculate_lateness(9, moveHour, emp.distanceToWork, emp.car.velocity)
    if isLate:
        self.deduct(id, 10)
    else:
        self.reward(id, 10)

  @staticmethod
  def calculate_lateness(targetHour , moveHour, distance, velocity):
    arrivalTime = moveHour + (distance / velocity)
    return targetHour < arrivalTime

  @classmethod
  def change_emps_num(cls, num):
    cls.employeesNum = num

  def export_data(self, filename):
    data = {
      "officeName": self.name,
      "employees": [emp.to_dict() for emp in self.employees]
    }
  
  try:
    file = open(filename, "w")
    file.write(data)
  except:
    print("Error in accessing file")
  finally:
    file.close()
