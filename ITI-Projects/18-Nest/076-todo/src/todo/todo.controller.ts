import {
  Body,
  Controller,
  Get,
  Post,
  Patch,
  Param,
  Delete,
} from '@nestjs/common';
import { TodoService } from './todo.service';
import { Todo } from './todo.type';

@Controller('todos')
export class TodoController {
  constructor(private readonly todoService: TodoService) {}

  @Get()
  getTodos() {
    return this.todoService.getTodos();
  }

  @Get(':id')
  getTodo(@Param('id') id: number) {
    return this.todoService.getTodo(+id);
  }

  @Post()
  addTodo(@Body() body: Todo): Todo {
    return this.todoService.addTodo(body);
  }

  @Patch(':id')
  updateTodo(@Param('id') id: number, @Body() body: Todo): Todo {
    return this.todoService.updateTodo(+id, body);
  }

  @Delete(':id')
  delTodo(@Param('id') id: number) {
    return this.todoService.deleteTodo(+id);
  }
}
