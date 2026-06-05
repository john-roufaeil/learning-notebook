export enum TodoStatus {
  TODO = 'todo',
  IN_PROGRESS = 'in_progress',
  DONE = 'done',
}

export class Todo {
  id!: number;
  task!: string;
  status: TodoStatus = TodoStatus.TODO;
}
