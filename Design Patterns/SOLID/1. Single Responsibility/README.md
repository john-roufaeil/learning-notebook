# S — Single Responsibility Principle (SRP)
### *"A class should have only one reason to change."* — Robert C. Martin

---

## Table of Contents
1. [The World Before SRP — The Pain](#the-world-before-srp--the-pain)
2. [Why This Problem Matters](#why-this-problem-matters)
3. [What SRP Actually Says](#what-srp-actually-says)
4. [Deep Dive: What is a "Responsibility"?](#deep-dive-what-is-a-responsibility)
5. [Example 1 — The God Class (Bad) vs. Separated Classes (Good)](#example-1--the-god-class-bad-vs-separated-classes-good)
6. [Example 2 — User Management System](#example-2--user-management-system)
7. [Example 3 — Invoice Processing](#example-3--invoice-processing)
8. [Real World Scenario — E-Commerce Order System](#real-world-scenario--e-commerce-order-system)
9. [Common Misunderstandings](#common-misunderstandings)
10. [How to Spot SRP Violations](#how-to-spot-srp-violations)
11. [Summary](#summary)

---

## The World Before SRP — The Pain

Imagine you're a junior developer who just joined a company. Your first task: **"Add a discount field to the invoice email."**

Simple, right? You open the codebase and find a file called `Invoice.java`. It's 2,000 lines long. You scroll through it and slowly realize this single class:

- Connects to the database and fetches invoice data
- Calculates totals, taxes, and discounts
- Formats the invoice as HTML
- Sends the invoice via email
- Logs the transaction to a log file
- Validates user permissions

You make your small change. You add the discount field to the email template. You test it — emails look great. You ship it.

**Production breaks.**

The HTML formatter had a shared method with the PDF renderer that you accidentally broke. The PDF invoices are now corrupt. You never even touched the PDF code — but it was all tangled together in one massive class.

This is the pain of violating SRP. A change in one area breaks something completely unrelated. You have no idea what's safe to touch.

---

## Why This Problem Matters

When a class has multiple responsibilities, those responsibilities become **coupled** to each other. This causes a cascade of real-world problems:

### 1. Fragility
Changing one feature breaks another. The codebase becomes fragile — like a house of cards. Nobody wants to touch it.

### 2. Rigidity
Adding new features becomes terrifying. You have to understand the *entire* class before changing *any* of it. This slows development to a crawl.

### 3. Impossible to Test
How do you unit test a class that sends emails AND queries the database AND calculates taxes? You can't test tax logic without also setting up a fake database and a fake email server. Testing becomes a nightmare.

### 4. No Reusability
Want to use the email-sending logic in a different part of the app? Tough. It's buried inside the Invoice class, tangled with database code and PDF generation.

### 5. Team Conflicts
When two developers need to change the same massive class for completely different reasons — one fixing a tax bug, one changing the email template — they step on each other's work constantly (merge conflicts).

---

## What SRP Actually Says

> **A class should have only one reason to change.**

The key word is **"reason."** Not "one method" or "one function." One **reason to change.**

Think of it this way: **who** would ask you to change this code?

- The **accounting team** might ask you to change tax calculation logic.
- The **marketing team** might ask you to change the email template.
- The **DBA (database admin)** might ask you to change how data is queried.
- The **security team** might ask you to change permission checks.

If **different people/teams/stakeholders** can ask you to change the same class, that class has multiple responsibilities and violates SRP.

A class obeying SRP has only **one actor** — one group of people — who could ever have a reason to change it.

---

## Deep Dive: What is a "Responsibility"?

This is where beginners get confused. A "responsibility" is NOT just about the number of methods or lines of code. It's about **cohesion** — how closely related are the things this class does?

Think of it like a job title:

| Job Title | Responsibilities |
|---|---|
| Chef | Cooks food. That's it. |
| Chef + Accountant + Driver | Too many. Fire hazard. |

A class is like an employee. Give it one job. When it does that job well, it's a good class.

### The "Axis of Change" Test
Robert Martin calls it the "axis of change." If something in the outside world changes (a business rule, a technology, a format), which classes need to change?

- Tax laws change → **only** the tax calculator class should change.
- Email template changes → **only** the email formatter class should change.
- Database schema changes → **only** the data access class should change.

When a class needs to change for **multiple** external reasons, it's carrying multiple responsibilities.

---

## Example 1 — The God Class (Bad) vs. Separated Classes (Good)

### ❌ BAD — The "God Class" that does everything

```python
class Employee:
    def __init__(self, name, salary, department):
        self.name = name
        self.salary = salary
        self.department = department

    # Responsibility 1: Business Logic
    def calculate_pay(self):
        if self.department == "engineering":
            return self.salary * 1.2  # 20% bonus
        return self.salary

    # Responsibility 2: Database persistence
    def save_to_database(self):
        db = DatabaseConnection()
        db.execute(
            "INSERT INTO employees (name, salary, department) VALUES (?, ?, ?)",
            (self.name, self.salary, self.department)
        )

    # Responsibility 3: Report generation
    def generate_report(self):
        return f"""
        Employee Report
        ---------------
        Name: {self.name}
        Salary: ${self.calculate_pay()}
        Department: {self.department}
        """

    # Responsibility 4: Email notification
    def send_payslip_email(self):
        email_body = self.generate_report()
        EmailService.send(
            to=f"{self.name}@company.com",
            subject="Your Payslip",
            body=email_body
        )
```

**What's wrong here?**
- The `Employee` class has FOUR separate reasons to change:
  1. Business logic changes (pay calculation rules)
  2. Database schema or ORM changes
  3. Report format changes
  4. Email format or email provider changes

- You can't test `calculate_pay()` without dragging in the database and email logic.
- A marketing person's email format change could break a payroll calculation test.

---

### ✅ GOOD — Each class has one reason to change

```python
# Responsibility 1: Business Logic ONLY
class Employee:
    def __init__(self, name, salary, department):
        self.name = name
        self.salary = salary
        self.department = department

    def calculate_pay(self):
        if self.department == "engineering":
            return self.salary * 1.2
        return self.salary


# Responsibility 2: Database persistence ONLY
class EmployeeRepository:
    def save(self, employee: Employee):
        db = DatabaseConnection()
        db.execute(
            "INSERT INTO employees (name, salary, department) VALUES (?, ?, ?)",
            (employee.name, employee.salary, employee.department)
        )

    def find_by_name(self, name: str) -> Employee:
        db = DatabaseConnection()
        row = db.query("SELECT * FROM employees WHERE name = ?", (name,))
        return Employee(row["name"], row["salary"], row["department"])


# Responsibility 3: Report generation ONLY
class EmployeeReportGenerator:
    def generate(self, employee: Employee) -> str:
        return f"""
        Employee Report
        ---------------
        Name: {employee.name}
        Salary: ${employee.calculate_pay()}
        Department: {employee.department}
        """


# Responsibility 4: Email notification ONLY
class PayslipEmailService:
    def __init__(self, report_generator: EmployeeReportGenerator):
        self.report_generator = report_generator

    def send(self, employee: Employee):
        report = self.report_generator.generate(employee)
        EmailService.send(
            to=f"{employee.name}@company.com",
            subject="Your Payslip",
            body=report
        )
```

**What's great now?**
- Tax law changes? Edit **only** `Employee.calculate_pay()`.
- Switch from SQL to MongoDB? Edit **only** `EmployeeRepository`.
- Redesign the report? Edit **only** `EmployeeReportGenerator`.
- Switch from SendGrid to Mailchimp? Edit **only** `PayslipEmailService`.

Each class has **exactly one reason to change**.

---

## Example 2 — User Management System

Here's a real-world web application scenario. You're building a user registration system.

### ❌ BAD

```javascript
class UserService {
  registerUser(userData) {
    // Validation logic
    if (!userData.email.includes("@")) {
      throw new Error("Invalid email");
    }
    if (userData.password.length < 8) {
      throw new Error("Password too short");
    }

    // Password hashing
    const hashedPassword = bcrypt.hashSync(userData.password, 10);

    // Database insertion
    const user = db.query(
      "INSERT INTO users (email, password) VALUES (?, ?)",
      [userData.email, hashedPassword]
    );

    // Send welcome email
    nodemailer.sendMail({
      to: userData.email,
      subject: "Welcome!",
      text: `Hi ${userData.name}, welcome to our platform!`,
    });

    // Log the registration
    fs.appendFileSync(
      "app.log",
      `[${new Date()}] New user registered: ${userData.email}\n`
    );

    return user;
  }
}
```

This `registerUser` function has **5 responsibilities**: validation, hashing, database, email, logging. Changing any one of them risks breaking the others.

### ✅ GOOD

```javascript
// 1. Validates user input only
class UserValidator {
  validate(userData) {
    if (!userData.email.includes("@")) {
      throw new Error("Invalid email");
    }
    if (userData.password.length < 8) {
      throw new Error("Password too short");
    }
  }
}

// 2. Handles password security only
class PasswordService {
  hash(plainTextPassword) {
    return bcrypt.hashSync(plainTextPassword, 10);
  }

  verify(plainText, hashed) {
    return bcrypt.compareSync(plainText, hashed);
  }
}

// 3. Handles database operations only
class UserRepository {
  save(email, hashedPassword) {
    return db.query(
      "INSERT INTO users (email, password) VALUES (?, ?)",
      [email, hashedPassword]
    );
  }

  findByEmail(email) {
    return db.query("SELECT * FROM users WHERE email = ?", [email]);
  }
}

// 4. Handles email communication only
class WelcomeEmailService {
  send(user) {
    nodemailer.sendMail({
      to: user.email,
      subject: "Welcome!",
      text: `Hi ${user.name}, welcome to our platform!`,
    });
  }
}

// 5. Handles logging only
class UserActivityLogger {
  logRegistration(email) {
    fs.appendFileSync(
      "app.log",
      `[${new Date()}] New user registered: ${email}\n`
    );
  }
}

// 6. Orchestrates the workflow — its ONE job is coordination
class UserRegistrationService {
  constructor(validator, passwordService, userRepo, emailService, logger) {
    this.validator = validator;
    this.passwordService = passwordService;
    this.userRepo = userRepo;
    this.emailService = emailService;
    this.logger = logger;
  }

  register(userData) {
    this.validator.validate(userData);
    const hashedPassword = this.passwordService.hash(userData.password);
    const user = this.userRepo.save(userData.email, hashedPassword);
    this.emailService.send(user);
    this.logger.logRegistration(userData.email);
    return user;
  }
}
```

Now each class is independently testable, swappable, and understandable. Want to switch from bcrypt to argon2? Touch **only** `PasswordService`. Want to switch from file logging to a cloud logger? Touch **only** `UserActivityLogger`.

---

## Example 3 — Invoice Processing

Let's look at a financial system — very common in enterprise software.

### ❌ BAD

```csharp
public class Invoice
{
    public List<LineItem> Items { get; set; }
    public Customer Customer { get; set; }

    // Calculates total — business logic
    public decimal CalculateTotal()
    {
        return Items.Sum(item => item.Price * item.Quantity);
    }

    // Applies tax — business logic (but different tax rules per region!)
    public decimal CalculateTax()
    {
        return CalculateTotal() * 0.2m; // UK VAT hardcoded!
    }

    // Formats as HTML — presentation logic
    public string ToHtml()
    {
        return $"<h1>Invoice for {Customer.Name}</h1><p>Total: {CalculateTotal()}</p>";
    }

    // Saves to database — persistence logic
    public void Save()
    {
        Database.Save(this);
    }

    // Sends PDF via email — communication logic
    public void EmailToCustomer()
    {
        var pdf = PdfGenerator.Generate(this.ToHtml());
        EmailClient.Send(Customer.Email, "Your Invoice", pdf);
    }
}
```

The Invoice class knows about HTML, email, databases, AND tax rules. It's doing everything.

### ✅ GOOD

```csharp
// Pure data model — only holds data
public class Invoice
{
    public int Id { get; set; }
    public List<LineItem> Items { get; set; }
    public Customer Customer { get; set; }
}

// Business logic — calculates money amounts
public class InvoiceCalculator
{
    private readonly ITaxStrategy _taxStrategy;

    public InvoiceCalculator(ITaxStrategy taxStrategy)
    {
        _taxStrategy = taxStrategy;
    }

    public decimal CalculateSubtotal(Invoice invoice)
        => invoice.Items.Sum(i => i.Price * i.Quantity);

    public decimal CalculateTax(Invoice invoice)
        => _taxStrategy.Calculate(CalculateSubtotal(invoice));

    public decimal CalculateTotal(Invoice invoice)
        => CalculateSubtotal(invoice) + CalculateTax(invoice);
}

// Presentation logic — formats invoice for display
public class InvoiceHtmlFormatter
{
    public string Format(Invoice invoice, decimal total)
    {
        return $"<h1>Invoice for {invoice.Customer.Name}</h1><p>Total: {total:C}</p>";
    }
}

// Persistence logic — saves and retrieves from database
public class InvoiceRepository
{
    public void Save(Invoice invoice) => Database.Save(invoice);
    public Invoice GetById(int id) => Database.Find<Invoice>(id);
}

// Communication logic — sends invoice to customer
public class InvoiceEmailService
{
    private readonly InvoiceHtmlFormatter _formatter;
    private readonly InvoiceCalculator _calculator;

    public void Send(Invoice invoice)
    {
        var total = _calculator.CalculateTotal(invoice);
        var html = _formatter.Format(invoice, total);
        var pdf = PdfGenerator.Generate(html);
        EmailClient.Send(invoice.Customer.Email, "Your Invoice", pdf);
    }
}
```

Adding EU tax rules? Add a new `ITaxStrategy` implementation — **don't touch** the calculator. Redesigning the HTML layout? Touch **only** `InvoiceHtmlFormatter`.

---

## Real World Scenario — E-Commerce Order System

You're building an online store. An order comes in. Here's how SRP guides the entire architecture:

```
OrderController         → Handles HTTP request/response ONLY
    ↓
OrderValidator          → Validates order data ONLY (is stock available? is address valid?)
    ↓
PricingEngine           → Calculates prices, discounts, coupons ONLY
    ↓
PaymentProcessor        → Handles payment gateway ONLY
    ↓
InventoryService        → Updates stock levels ONLY
    ↓
OrderRepository         → Saves order to database ONLY
    ↓
OrderConfirmationEmail  → Sends confirmation email ONLY
    ↓
OrderEventPublisher     → Publishes events for analytics/other systems ONLY
```

Each service has **one job**. The team of 8 developers can each own one of these services and work in parallel without constantly conflicting. When the payment gateway changes from Stripe to PayPal, **only** the `PaymentProcessor` needs updating.

This is SRP at the architectural level. It scales from methods → classes → modules → services → microservices. The principle is the same at every level.

---

## Common Misunderstandings

### ❌ Myth 1: "SRP means every class should have only one method"
**Wrong.** A class can have many methods. What matters is whether all those methods serve the **same single purpose**. A `UserValidator` class can have `validateEmail()`, `validatePassword()`, `validateAge()` — all are part of the **single responsibility** of validation.

### ❌ Myth 2: "SRP means maximum decomposition — split everything into tiny classes"
**Wrong.** Splitting too aggressively creates a different problem: too many tiny classes that are hard to understand together. The test is: does this class have **multiple reasons to change**? If no, don't split it.

### ❌ Myth 3: "SRP only applies to classes"
**Wrong.** SRP applies to:
- **Functions/Methods**: A function should do one thing.
- **Classes**: A class should represent one concept.
- **Modules**: A module should serve one domain.
- **Microservices**: A service should own one capability.

### ❌ Myth 4: "My class is small, so it obeys SRP"
**Wrong.** A 20-line class can still violate SRP if it's doing two unrelated things. Size is irrelevant — cohesion is what matters.

---

## How to Spot SRP Violations

Ask yourself these questions when reviewing code:

1. **The "and" test**: Can you describe what this class does without using the word "and"?
   - ❌ "It calculates the total **and** sends an email."
   - ✅ "It calculates the total."

2. **The "who would ask me to change this?" test**: Are there two different stakeholders who could request changes to this class?
   - If accounting AND marketing both care about your class, it has two responsibilities.

3. **The import smell**: Is your class importing from wildly different domains?
   - A class that imports both `DatabaseConnection` and `EmailClient` is suspicious.

4. **The testing pain test**: When you write a unit test, do you need to mock 5 different things?
   - That's a sign the class is doing too much.

5. **The "reason to change" count**: List out every external change that would force you to modify this class. Each entry is a responsibility.

---

## Summary

| Concept | Description |
|---|---|
| **The Principle** | A class should have only one reason to change |
| **The Problem It Solves** | Fragile, rigid, untestable, tangled code |
| **The Core Idea** | Each class = one job, owned by one stakeholder |
| **The Benefit** | Easier testing, less coupling, parallel development |
| **The Anti-Pattern** | The "God Class" that knows everything and does everything |
| **The Risk of Going Too Far** | Over-decomposition into too many micro-classes |

SRP is the foundation of clean code. When your classes each do one thing, testing becomes a joy, bugs become isolated, and your codebase becomes something developers are **excited** to work in rather than afraid of.

> **Practical rule of thumb**: If your class needs more than 3–4 imports from unrelated domains, or if describing it requires the word "and," you probably have an SRP violation. Start there.