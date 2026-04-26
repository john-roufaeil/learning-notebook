import express from "express";

const app = express();
app.use(express.json());

let students = [];
let nextId = 1;

app.get('/students', (req, res) => {
  const response = {
    data: students,
    links: {
      self: { href: "/students", method: "GET" },
      create: { href: "/students", method: "POST" }
    }
  };
  res.status(200).json(response);
});

app.get('/students/:id', (req, res) => {
  const id = parseInt(req.params.id);
  const student = students.find(s => s.id === id);

  if (!student) {
    return res.status(404).json({ error: "Student not found" });
  }

  const response = {
    data: student,
    links: {
      self: { href: `/students/${id}`, method: "GET" },
      update: { href: `/students/${id}`, method: "PUT" },
      delete: { href: `/students/${id}`, method: "DELETE" }
    }
  };
  res.status(200).json(response);
});

app.post('/students', (req, res) => {
  const { name, age } = req.body;

  if (!name || age === undefined) {
    return res.status(400).json({ error: "Name and age are required" });
  } else if (typeof age !== 'number' || age <= 0) {
    return res.status(400).json({ error: "Age must be a positive number" });
  }

  const id = nextId++;
  const newStudent = { id, name, age };
  students.push(newStudent);

  const response = {
    data: newStudent,
    links: {
      self: { href: "/students", method: "GET" },
      getThisStudent: { href: `/students/${id}`, method: "GET" },
      updateThisStudent: { href: `/students/${id}`, method: "PUT" },
      deleteThisStudent: { href: `/students/${id}`, method: "DELETE" }
    }
  }
  res.status(201).json(response);
});

app.delete('/students/:id', (req, res) => {
  const id = parseInt(req.params.id);
  const student = students.find(s => s.id === id);

  if (!student) {
    return res.status(404).json({ error: "Student not found" });
  }

  students = students.filter(s => s.id !== id)

  const response = {
    data: student,
    links: {
      self: { href: `/students/${id}`, method: "GET" },
      getAllStudents: { href: "/students", method: "GET" },
      createStudent: { href: "/students", method: "POST" }
    }
  };
  res.status(200).json(response);
});

app.put('/students/:id', (req, res) => {
  const id = parseInt(req.params.id);
  const { name, age } = req.body;
  const studentIdx = students.findIndex(s => s.id === id);

  if (studentIdx === -1) {
    return res.status(404).json({ error: "Student not found" });
  } else if (!name || age === undefined) {
    return res.status(400).json({ error: " Name and age are required" });
  } else if (typeof age !== "number" || age <= 0) {
    return res.status(400).json({ error: "Age must be a positive number" })
  }

  const updatedStudent = { id, name, age };
  students[studentIdx] = updatedStudent;
  const response = {
    data: updatedStudent,
    links: {
      self: { href: `/students/${id}`, method: "GET" },
      deleteThisStudent: { href: `/students/${id}`, method: "DELETE" },
    }
  }
  res.status(200).json(response);
})

app.listen(3000, () => {
  console.log("Server is running on port 3000");
});
