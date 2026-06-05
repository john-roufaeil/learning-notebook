import { Injectable, NotFoundException } from '@nestjs/common';
import { Todo, TodoStatus } from './todo.type';

@Injectable()
export class TodoService {
  todos: Todo[] = [];

  getTodos(): Todo[] {
    return this.todos;
  }

  getTodo(id: number): Todo {
    const todo = this.todos.find((todo) => todo.id === id);
    if (!todo) throw new NotFoundException('Todo not found');
    return todo;
  }

  addTodo(todo: Todo): Todo {
    const newTodo = {
      ...todo,
      id: this.todos.length + 1,
      status: todo.status ?? TodoStatus.TODO,
    };
    this.todos.push(newTodo);
    return newTodo;
  }

  updateTodo(id: number, newTodo: Todo): Todo {
    const todoIdx = this.todos.findIndex((todo) => todo.id === id);
    if (todoIdx === -1) throw new NotFoundException('Todo not found');
    if (newTodo.task) this.todos[todoIdx].task = newTodo.task;
    if (newTodo.status) this.todos[todoIdx].status = newTodo.status;
    //Object.assign()
    return this.todos[todoIdx];
  }

  deleteTodo(id: number): void {
    const index = this.todos.findIndex((todo) => todo.id === id);
    if (index !== -1) {
      this.todos.splice(index, 1);
    }
  }
}
