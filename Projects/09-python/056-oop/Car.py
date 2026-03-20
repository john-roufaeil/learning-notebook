class Car:
  def __init__(self, name, fuelRate, velocity):
    self._name = name
    self._fuelRate = fuelRate
    self._velocity = velocity

  @parameter
  def name(self):
    return self._name
  @name.setter
  def name(self, value):
    self._name = value
  
  @parameter
  def fuelRate(self):
    return self._fuelRate
  @fuelRate.setter
  def fuelRate(self, value):
    self._fuelRate = value
  
  @parameter
  def velocity(self):
    return self.velocity
  @velocity.setter
  def velocity(self, value):
    self.velocity = value
  
  def run(self):
    pass

  def stop(self):
    pass