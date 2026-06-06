import { Injectable, NotFoundException } from '@nestjs/common';
import { Todo, TodoStatus } from './todo.entity';
import { CreateTodoDto } from './dto/create-todo.dto';
import { EditTodoDto } from './dto/edit-todo.dto';

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

  addTodo(todo: CreateTodoDto): Todo {
    const newTodo = {
      ...todo,
      id: this.todos.length + 1,
      status: todo.status ?? TodoStatus.TODO,
    };
    this.todos.push(newTodo);
    return newTodo;
  }

  updateTodo(id: number, newTodo: EditTodoDto): Todo {
    const todoIdx = this.todos.findIndex((todo) => todo.id === id);
    if (todoIdx === -1) throw new NotFoundException('Todo not found');
    return Object.assign(this.todos[todoIdx], newTodo);
  }

  deleteTodo(id: number): void {
    const todoIdx = this.todos.findIndex((todo) => todo.id === id);
    if (todoIdx === -1) throw new NotFoundException('Todo not found');
    this.todos.splice(todoIdx, 1);
  }
}
