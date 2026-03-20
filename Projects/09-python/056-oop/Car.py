class Car:
  def __init__(self, name, fuelRate, velocity):
    self._name = name
    self._fuelRate = fuelRate
    self._velocity = velocity

  @property
  def name(self):
    return self._name
  @name.setter
  def name(self, value):
    self._name = value
  
  @property
  def fuelRate(self):
    return self._fuelRate
  @fuelRate.setter
  def fuelRate(self, value):
    self._fuelRate = value
  
  @property
  def velocity(self):
    return self._velocity
  @velocity.setter
  def velocity(self, value):
    self._velocity = value
  
  def run(self):
    pass

  def stop(self):
    pass