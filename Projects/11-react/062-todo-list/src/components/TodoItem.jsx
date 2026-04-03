import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faCheck } from '@fortawesome/free-solid-svg-icons';

export const TodoItem = ({ todo, removeTodo, toggleTodo }) => {
  return (
    <div className="border-2 border-gray-300 rounded-2xl p-4 w-full md:w-[48%] flex flex-col gap-3">

      <div className="flex items-center gap-3">
        <button
          onClick={() => toggleTodo(todo.id)}
          className={`w-5 h-5 rounded-full border-2 flex items-center justify-center hover:cursor-pointer hover:border-green-500 transition
          ${todo.completed ? 'bg-green-500 border-green-500' : 'border-gray-400'}`}
        >
          {todo.completed &&
            <span className="text-white flex jusify-center items-center">
              <FontAwesomeIcon icon={faCheck} className="text-[10px] text-white" />
            </span>
          }
        </button>

        <h3 className={`font-semibold text-lg ${todo.completed && 'line-through text-gray-400'}`}>
          {todo.title}
        </h3>
      </div>

      <p className={`${todo.completed && 'line-through text-gray-400'}`}>
        {todo.description}
      </p>

      <div className="flex justify-end gap-2">
        <button
          onClick={() => removeTodo(todo.id)}
          className="px-3 py-1 rounded-lg bg-red-500 text-white text-sm hover:cursor-pointer hover:bg-red-600 transition"
        >
          Remove
        </button>
      </div>

    </div>
  );
};