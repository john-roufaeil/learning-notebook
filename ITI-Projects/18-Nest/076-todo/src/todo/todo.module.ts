import { TodoService } from './todo.service';
import { TodoController } from './todo.controller';
import { MiddlewareConsumer, Module, RequestMethod } from '@nestjs/common';
import { LoggingMiddleware } from 'middlewares/logger.middleware';

@Module({
  imports: [],
  controllers: [TodoController],
  providers: [TodoService],
})
export class TodoModule {
  configure(consumer: MiddlewareConsumer) {
    consumer
      .apply(LoggingMiddleware)
      .forRoutes({ path: 'todos', method: RequestMethod.POST });
  }
}
