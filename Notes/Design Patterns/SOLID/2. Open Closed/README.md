# O — Open/Closed Principle (OCP)
### *"Software entities should be open for extension, but closed for modification."* — Bertrand Meyer / Robert C. Martin

---

## Table of Contents
1. [The Pain — What Happens Without OCP](#the-pain--what-happens-without-ocp)
2. [What OCP Actually Means](#what-ocp-actually-means)
3. [The Key Insight: Abstractions are the Door](#the-key-insight-abstractions-are-the-door)
4. [Example 1 — Shape Area Calculator](#example-1--shape-area-calculator)
5. [Example 2 — Payment Processing System](#example-2--payment-processing-system)
6. [Example 3 — Notification System](#example-3--notification-system)
7. [Example 4 — Discount Strategies](#example-4--discount-strategies)
8. [Real World Scenario — Report Generator](#real-world-scenario--report-generator)
9. [OCP and the Strategy Pattern](#ocp-and-the-strategy-pattern)
10. [Common Misunderstandings](#common-misunderstandings)
11. [When NOT to Apply OCP](#when-not-to-apply-ocp)
12. [Summary](#summary)

---

## The Pain — What Happens Without OCP

You've built a working payment system. It handles credit cards. Life is good.

Then your boss says: **"We need to support PayPal."**

You open the code:

```python
def process_payment(payment_type, amount):
    if payment_type == "credit_card":
        charge_credit_card(amount)
    elif payment_type == "paypal":  # You add this
        charge_paypal(amount)
```

Fine. Three months later: **"Add Apple Pay."**

```python
def process_payment(payment_type, amount):
    if payment_type == "credit_card":
        charge_credit_card(amount)
    elif payment_type == "paypal":
        charge_paypal(amount)
    elif payment_type == "apple_pay":  # You add this again
        charge_apple_pay(amount)
```

Six months later: **"Crypto payments. And Stripe. And Klarna."**

You now have a 200-line `if/elif` chain. Every time a new payment method is added, you:

1. **Open** this existing, working function
2. **Modify** it (risking breaking credit cards and PayPal)
3. **Re-test** everything all over again
4. **Pray** you didn't introduce a bug in the code you didn't touch

This is the core violation of OCP: you keep **modifying** existing, proven code instead of **extending** it. And every modification is a chance to break something that was already working perfectly.

Imagine you're on a surgical team. The operation was a success — the patient is fine. Now you need to operate on a *different* patient. Does that mean you should cut the first patient open again? Of course not. That's insane. But that's exactly what OCP violations force you to do with code.

---

## What OCP Actually Means

> **Open for extension**: You can add new behavior.  
> **Closed for modification**: You don't need to change existing, tested code to do so.

These two ideas sound contradictory. How can you add new behavior WITHOUT changing existing code?

The answer: **abstractions**.

You write code that depends on **abstract interfaces** rather than **concrete implementations**. New behavior is added by creating **new implementations** of the interface — not by changing the interface users.

Think of an electrical outlet. Your house wiring is "closed for modification" — you don't rewire your house every time you buy a new device. But it's "open for extension" — you can plug in any device that has the right plug shape. The outlet is the **abstraction**. The devices are the **extensions**.

---

## The Key Insight: Abstractions are the Door

The mechanism that makes OCP work is **abstraction** — usually in the form of:
- **Interfaces** (in Java, C#, TypeScript)
- **Abstract Base Classes** (in Python, C++)
- **Duck typing** (in Python, JavaScript)
- **Higher-order functions** (passing functions as arguments)

When your high-level code depends on an **abstraction** rather than a **concrete class**, you can swap in any new implementation without ever touching the high-level code.

---

## Example 1 — Shape Area Calculator

This is the classic OCP example, and it's classic for a reason — it's crystal clear.

### ❌ BAD — Adding a new shape requires modifying existing code

```python
class AreaCalculator:
    def calculate_area(self, shape):
        if shape["type"] == "circle":
            return 3.14 * shape["radius"] ** 2

        elif shape["type"] == "rectangle":
            return shape["width"] * shape["height"]

        # Every new shape requires opening this class and adding another branch
        elif shape["type"] == "triangle":
            return 0.5 * shape["base"] * shape["height"]

        # What if someone passes "Triangle" with a capital T?
        # What if they forget to include a required field?
        # This function becomes a minefield over time.
```

**Problems:**
- Every new shape (pentagon, hexagon, ellipse) means re-opening and modifying `calculate_area`.
- If you have 5 places in the codebase that use shape calculations, you have to update all 5.
- The `if/elif` chain grows forever. It never shrinks.
- This code can never be "done" — it's permanently incomplete and permanently risky to modify.

---

### ✅ GOOD — New shapes extend the system without touching existing code

```python
from abc import ABC, abstractmethod
import math

# The ABSTRACTION — defines the "contract" every shape must fulfill
class Shape(ABC):
    @abstractmethod
    def area(self) -> float:
        pass

# Concrete implementation 1 — CLOSED for modification after release
class Circle(Shape):
    def __init__(self, radius: float):
        self.radius = radius

    def area(self) -> float:
        return math.pi * self.radius ** 2

# Concrete implementation 2 — CLOSED for modification after release
class Rectangle(Shape):
    def __init__(self, width: float, height: float):
        self.width = width
        self.height = height

    def area(self) -> float:
        return self.width * self.height

# The calculator depends ONLY on the abstraction, not on any specific shape
class AreaCalculator:
    def total_area(self, shapes: list[Shape]) -> float:
        return sum(shape.area() for shape in shapes)


# ✅ Adding a triangle NEVER requires touching AreaCalculator
class Triangle(Shape):
    def __init__(self, base: float, height: float):
        self.base = base
        self.height = height

    def area(self) -> float:
        return 0.5 * self.base * self.height

# ✅ Adding an ellipse NEVER requires touching AreaCalculator
class Ellipse(Shape):
    def __init__(self, semi_major: float, semi_minor: float):
        self.semi_major = semi_major
        self.semi_minor = semi_minor

    def area(self) -> float:
        return math.pi * self.semi_major * self.semi_minor


# Usage — AreaCalculator never changed, but it handles all shapes
calculator = AreaCalculator()
shapes = [Circle(5), Rectangle(4, 6), Triangle(3, 8), Ellipse(7, 3)]
print(calculator.total_area(shapes))  # Works perfectly
```

`AreaCalculator` was written once, tested once, and never touched again. You can add 100 more shapes. It just works. That's the power of OCP.

---

## Example 2 — Payment Processing System

Back to our payment system from the intro — let's fix it properly.

### ❌ BAD — The if/elif payment nightmare

```javascript
class PaymentProcessor {
  process(paymentType, amount) {
    if (paymentType === "credit_card") {
      console.log(`Charging credit card: $${amount}`);
      // credit card specific API calls...
    } else if (paymentType === "paypal") {
      console.log(`Charging PayPal: $${amount}`);
      // PayPal specific API calls...
    } else if (paymentType === "apple_pay") {
      console.log(`Charging Apple Pay: $${amount}`);
      // Apple Pay specific API calls...
    }
    // Adding crypto means opening this file... again.
  }
}
```

### ✅ GOOD — New payment methods are extensions, not modifications

```javascript
// The abstraction — the "contract" all payment methods must implement
class PaymentMethod {
  pay(amount) {
    throw new Error("pay() must be implemented by subclass");
  }
}

// Each payment method is a self-contained, closed unit
class CreditCardPayment extends PaymentMethod {
  constructor(cardNumber, cvv) {
    super();
    this.cardNumber = cardNumber;
    this.cvv = cvv;
  }

  pay(amount) {
    console.log(`Charging $${amount} to credit card ending in ${this.cardNumber.slice(-4)}`);
    // Stripe API integration here
  }
}

class PayPalPayment extends PaymentMethod {
  constructor(email) {
    super();
    this.email = email;
  }

  pay(amount) {
    console.log(`Charging $${amount} via PayPal account: ${this.email}`);
    // PayPal SDK here
  }
}

class ApplePayPayment extends PaymentMethod {
  pay(amount) {
    console.log(`Charging $${amount} via Apple Pay`);
    // Apple Pay API here
  }
}

// The processor is CLOSED — it never changes when new payment methods are added
class PaymentProcessor {
  process(paymentMethod, amount) {
    paymentMethod.pay(amount);
  }
}

// ✅ Adding crypto: zero changes to existing code
class CryptoPayment extends PaymentMethod {
  constructor(walletAddress) {
    super();
    this.walletAddress = walletAddress;
  }

  pay(amount) {
    console.log(`Transferring ${amount} USDC to wallet: ${this.walletAddress}`);
    // Blockchain API here
  }
}

// Usage — processor never changed
const processor = new PaymentProcessor();
processor.process(new CreditCardPayment("4111111111111111", "123"), 99.99);
processor.process(new PayPalPayment("user@example.com"), 49.99);
processor.process(new CryptoPayment("0xAbC123..."), 200.00);
```

Now when your PM says "Add WeChat Pay by Friday," you:
1. Create a new file: `WeChatPayment.js`
2. Extend `PaymentMethod`
3. Implement `pay()`
4. Done. Zero other files change.

The existing, tested payment processor is **never touched**. You can't accidentally break credit card processing while adding WeChat Pay.

---

## Example 3 — Notification System

Real scenario: You need to notify users. First just by email. Then by SMS. Then push notifications. Then Slack.

### ❌ BAD

```csharp
public class NotificationService
{
    public void Notify(string channel, string userId, string message)
    {
        if (channel == "email")
        {
            var email = GetUserEmail(userId);
            EmailClient.Send(email, message);
        }
        else if (channel == "sms")
        {
            var phone = GetUserPhone(userId);
            SmsClient.Send(phone, message);
        }
        else if (channel == "push")
        {
            var deviceToken = GetDeviceToken(userId);
            PushClient.Send(deviceToken, message);
        }
        // Every new channel: open this file, add another branch, re-test everything
    }
}
```

### ✅ GOOD

```csharp
// The abstraction
public interface INotificationChannel
{
    void Send(string userId, string message);
}

// Channel implementations — each closed after written
public class EmailNotification : INotificationChannel
{
    public void Send(string userId, string message)
    {
        var email = UserService.GetEmail(userId);
        EmailClient.Send(email, message);
    }
}

public class SmsNotification : INotificationChannel
{
    public void Send(string userId, string message)
    {
        var phone = UserService.GetPhone(userId);
        TwilioClient.SendSms(phone, message);
    }
}

public class PushNotification : INotificationChannel
{
    public void Send(string userId, string message)
    {
        var token = DeviceService.GetToken(userId);
        FirebaseClient.Send(token, message);
    }
}

// The service is CLOSED — never changes regardless of how many channels are added
public class NotificationService
{
    private readonly IEnumerable<INotificationChannel> _channels;

    public NotificationService(IEnumerable<INotificationChannel> channels)
    {
        _channels = channels;
    }

    public void NotifyAll(string userId, string message)
    {
        foreach (var channel in _channels)
        {
            channel.Send(userId, message);
        }
    }
}

// ✅ Adding Slack: zero changes to existing code
public class SlackNotification : INotificationChannel
{
    public void Send(string userId, string message)
    {
        var slackId = SlackDirectory.Lookup(userId);
        SlackClient.PostMessage(slackId, message);
    }
}

// Usage — dependency injection adds slack without changing NotificationService
var service = new NotificationService(new INotificationChannel[]
{
    new EmailNotification(),
    new SmsNotification(),
    new SlackNotification(),  // Just add it here!
});
```

---

## Example 4 — Discount Strategies

This is a very common business scenario: different discount rules for different user types.

### ❌ BAD

```python
def calculate_discount(user_type: str, price: float) -> float:
    if user_type == "regular":
        return price
    elif user_type == "premium":
        return price * 0.9  # 10% off
    elif user_type == "vip":
        return price * 0.8  # 20% off
    elif user_type == "employee":
        return price * 0.5  # 50% off
    # Marketing emails you: "Add a 'student' discount at 15% please"
    # You open THIS file. Again.
```

### ✅ GOOD

```python
from abc import ABC, abstractmethod

class DiscountStrategy(ABC):
    @abstractmethod
    def apply(self, price: float) -> float:
        pass

class NoDiscount(DiscountStrategy):
    def apply(self, price: float) -> float:
        return price

class PremiumDiscount(DiscountStrategy):
    def apply(self, price: float) -> float:
        return price * 0.9

class VipDiscount(DiscountStrategy):
    def apply(self, price: float) -> float:
        return price * 0.8

class EmployeeDiscount(DiscountStrategy):
    def apply(self, price: float) -> float:
        return price * 0.5

# ✅ New: StudentDiscount — zero changes to existing code
class StudentDiscount(DiscountStrategy):
    def apply(self, price: float) -> float:
        return price * 0.85

# The checkout is CLOSED — it never changes
class ShoppingCart:
    def __init__(self, discount_strategy: DiscountStrategy):
        self.discount_strategy = discount_strategy
        self.items = []

    def add_item(self, price: float):
        self.items.append(price)

    def total(self) -> float:
        subtotal = sum(self.items)
        return self.discount_strategy.apply(subtotal)

# Usage
vip_cart = ShoppingCart(VipDiscount())
vip_cart.add_item(100)
print(vip_cart.total())  # 80.0

student_cart = ShoppingCart(StudentDiscount())
student_cart.add_item(100)
print(student_cart.total())  # 85.0
```

This pattern is called the **Strategy Pattern** — and it's fundamentally an expression of OCP.

---

## Real World Scenario — Report Generator

You work at a data company. They need reports. Currently: PDF only.

Then: Excel. Then: CSV. Then: JSON API. Then: Interactive HTML dashboard.

```python
from abc import ABC, abstractmethod
from typing import List, Dict

class ReportData:
    def __init__(self, title: str, rows: List[Dict]):
        self.title = title
        self.rows = rows

# The abstraction
class ReportExporter(ABC):
    @abstractmethod
    def export(self, data: ReportData) -> bytes:
        pass

    @abstractmethod
    def content_type(self) -> str:
        pass

# Existing implementations — CLOSED after tested
class PdfReportExporter(ReportExporter):
    def export(self, data: ReportData) -> bytes:
        # Use reportlab or weasyprint
        return generate_pdf(data.title, data.rows)

    def content_type(self) -> str:
        return "application/pdf"

class ExcelReportExporter(ReportExporter):
    def export(self, data: ReportData) -> bytes:
        # Use openpyxl
        return generate_excel(data.rows)

    def content_type(self) -> str:
        return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"

class CsvReportExporter(ReportExporter):
    def export(self, data: ReportData) -> bytes:
        import csv, io
        output = io.StringIO()
        writer = csv.DictWriter(output, fieldnames=data.rows[0].keys())
        writer.writeheader()
        writer.writerows(data.rows)
        return output.getvalue().encode()

    def content_type(self) -> str:
        return "text/csv"

# The report service — COMPLETELY CLOSED
class ReportService:
    def generate(self, data: ReportData, exporter: ReportExporter) -> dict:
        file_content = exporter.export(data)
        return {
            "content": file_content,
            "content_type": exporter.content_type(),
        }

# ✅ New requirement: JSON export
# Zero lines of existing code change
class JsonReportExporter(ReportExporter):
    def export(self, data: ReportData) -> bytes:
        import json
        return json.dumps({"title": data.title, "data": data.rows}).encode()

    def content_type(self) -> str:
        return "application/json"
```

Your `ReportService` was written on day 1. It's still the same on day 500. You've added 8 export formats and never once risked breaking PDF generation.

---

## OCP and the Strategy Pattern

The Strategy Pattern is the most natural implementation of OCP. The pattern has 3 parts:

```
[Context] ——uses——> [Strategy Interface] <——implements—— [ConcreteStrategyA]
                                                    <——implements—— [ConcreteStrategyB]
                                                    <——implements—— [ConcreteStrategyC]
```

- **Context**: Your high-level code (AreaCalculator, PaymentProcessor, ShoppingCart). CLOSED.
- **Strategy Interface**: The abstraction that defines what a behavior must look like. CLOSED.
- **Concrete Strategies**: The specific implementations. Each is NEW — they EXTEND the system.

Other patterns that implement OCP:
- **Decorator Pattern**: Add behavior by wrapping objects.
- **Template Method**: Define the skeleton, let subclasses fill in specifics.
- **Observer Pattern**: Add new observers without changing the subject.
- **Plugin Architecture**: Entire systems built on OCP (VS Code extensions, browser plugins).

---

## Common Misunderstandings

### ❌ Myth 1: "Closed means you can NEVER change the class"
**Wrong.** "Closed for modification" means closed to changes that add new behavior. You can still fix bugs. You can still refactor internals. OCP is about **feature additions**, not absolute immutability.

### ❌ Myth 2: "Every class must immediately be OCP-compliant from day 1"
**Wrong.** This leads to over-engineering. The pragmatic approach: build it simple first. When the **second** similar requirement appears, that's your signal to refactor toward OCP. This is the **"Rule of Three"** — after the third variation, you build the abstraction.

### ❌ Myth 3: "OCP means never using if/else"
**Wrong.** Simple if/else for simple, stable conditions is fine. OCP matters when you have behavior that will grow and vary over time — like payment types, export formats, or discount strategies.

### ❌ Myth 4: "Abstractions always make code better"
**Wrong.** Abstractions add complexity. A system with 50 single-use interfaces nobody extends is worse than a clean if/else. Apply OCP where change is **likely and repetitive**, not everywhere by default.

---

## When NOT to Apply OCP

OCP is not free. Abstractions add:
- More files and classes
- Indirection (harder to follow the code flow)
- Upfront design effort

Don't apply OCP when:
- The behavior is **unlikely to change** (fixed business rules, constants)
- There are only **2 variations** and they're simple
- You're in **early prototype** mode — get it working first
- The abstraction would be used **only once** — that's not an abstraction, it's just indirection for no gain

The engineering judgment is: **how likely is this to change or grow?** If high — apply OCP. If low — keep it simple.

---

## Summary

| Concept | Description |
|---|---|
| **The Principle** | Open for extension, closed for modification |
| **The Problem It Solves** | Constantly re-opening and risking working code when adding features |
| **The Mechanism** | Abstractions (interfaces/abstract classes) + polymorphism |
| **The Pattern** | Strategy, Decorator, Observer, Template Method |
| **The Benefit** | New features never touch tested code |
| **The Anti-Pattern** | The ever-growing if/elif/else chain |
| **The Risk** | Over-engineering with abstractions that are never reused |

> **Practical rule of thumb**: When you find yourself writing "else if" for the third time in the same function for similar-but-different behaviors, stop. That's your OCP signal. Build the abstraction now, or suffer the if/else chain forever.