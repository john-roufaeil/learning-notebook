from Employee import Employee
from Car import Car
from Office import Office

c0 = Car("Fiat128")
c1 = Car("Toyota")
e0 = Employee("1233", c0, "samy@email.com", 10000, 20, "Samy", 10, "Happy", 80)
e1 = Employee("1234", c1, "john@email.com", 70000, 10, "John", 1800000, "Happy", 80)
e2 = Employee("1235", None, "ahmed@email.com", 60000, 15, "Ahmed", 1500000, "Happy", 70)
e3 = Employee("1236", None, "mary@email.com", 80000, 5, "Mary", 2000000, "Happy", 90)
o1 = Office("ITI Main Office", [e0, e1, e2])


e0.sleep(10)
e1.sleep(7)
e2.sleep(5)
e0.eat(1)
e1.eat(2)
e2.eat(3)
e0.buy(1)
e1.buy(20)
e2.buy(30)

e0.work(7)
e1.work(8)
e2.work(9)

e0.refuel(50)
e1.refuel(200)

e0.drive(60, 55)
e1.drive(80, 100)

o1.hire(e3)
o1.fire("1235")
o1.deduct("1233", 500)
o1.reward("1234", 500)

e1.send_mail("john731.g@gmail.com", "This is email body", "John", "Test Email")

o1.export_data("ITI.txt")