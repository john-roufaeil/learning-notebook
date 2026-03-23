class Car:
  def __init__(self, name, fuelRate=0, velocity=0):
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
    if value < 0 or value > 100:
      raise ValueError("Fuel rate must be between 0 and 100")
    self._fuelRate = value
  
  @property
  def velocity(self):
    return self._velocity
  @velocity.setter
  def velocity(self, value):
    if value < 0 or value > 200:
      raise ValueError("Velocity must be between 0 and 200")
    self._velocity = value
  
  def run(self, velocity, distance):
    self.velocity = velocity
    consumed = min(self.fuelRate, distance)
    self.fuelRate -= consumed
    self.stop(distance - consumed)
    
  def stop(self, remainingDistance):
    self.velocity = 0
    if remainingDistance == 0:
      print("Car stopping; arrived at destination")
    else:
      print(f"Car stopping; distance remaining {remainingDistance}")
