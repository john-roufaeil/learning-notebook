import { IsString, IsEnum, IsOptional } from 'class-validator';
import { TodoStatus } from '../todo.entity';

export class CreateTodoDto {
  @IsString()
  task: string;

  @IsEnum(TodoStatus)
  @IsOptional()
  status: TodoStatus;
}
