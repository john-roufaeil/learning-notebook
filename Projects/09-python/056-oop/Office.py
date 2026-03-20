from Employee import Employee

class Office:
  def __init__(self, name, employees):
    self._name = name
    self._employees = employees
  
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
  
  def fire(self, id):
    pass
  
  def calculate_lateness(self, id):
    pass

  def reward(self, id):
    pass
