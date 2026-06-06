import { IsString, IsEnum, IsOptional } from 'class-validator';
import { TodoStatus } from '../todo.entity';

export class EditTodoDto {
  @IsString()
  @IsOptional()
  task: string;

  @IsEnum(TodoStatus)
  @IsOptional()
  status: TodoStatus;
}
