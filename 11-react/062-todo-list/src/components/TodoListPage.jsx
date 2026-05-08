import { useState } from "react";
import { TodoItem } from "./TodoItem";
import { AddTodo } from "./AddTodo";

export const TodoListPage = () => {
  let initialTodos = [
    {
      id: 1,
      title: 'Buy groceries',
      description: 'Milk, Bread, Eggs, Butter',
      completed: false,
    },
    {
      id: 2,
      title: 'Walk the dog',
      description: 'Take Fido for a walk in the park',
      completed: true,
    },
    {
      id: 3,
      title: 'Finish project',
      description: 'Complete the React project by Friday',
      completed: false,
    },
    {
      id: 4,
      title: 'Do laundry',
      description: 'Wash and fold clothes',
      completed: true,
    }
  ]
  const [todos, setTodos] = useState(initialTodos);

  const addTodo = (id, title, description) => {
    const newTodo = { id, title, description, completed: false }
    setTodos([...todos, newTodo]);
  }

  const removeTodo = (id) => {
    setTodos(todos.filter((todo) => todo.id !== id));
  }

  const toggleTodo = (id) => {
    setTodos(
      todos.map((todo) => {
        if (todo.id === id) {
          return { ...todo, completed: !todo.completed };
        }
        return todo;
      })
    );
  }

  return (
    <div className='p-8 max-w-5xl mx-auto shadow-2xl min-h-screen'>
      <h1 className="text-3xl text-center font-bold">To-do App!</h1>

      <AddTodo addTodo={addTodo} />

      <div className="flex flex-wrap gap-4 mt-8 ">
        {todos.length > 0 ? todos.map((todo) => (
          <TodoItem
            key={todo.id}
            todo={todo}
            removeTodo={removeTodo}
            toggleTodo={toggleTodo}
          />
        )) : (
          <p className="text-gray-500 text-center w-full">No todos left. Add one above!</p>
        )}
      </div>
    </div>
  )
}