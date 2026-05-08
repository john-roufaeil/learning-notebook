import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faPlus } from '@fortawesome/free-solid-svg-icons';
import { useState } from "react";

export const AddTodo = ({ addTodo }) => {
  const [id, setId] = useState('');
  const [title, setTitle] = useState('');
  const [description, setDescription] = useState('');

  return (
    <div className="mb-6 flex items-center gap-3 justify-center max-w-3xl mx-auto border-2 rounded-2xl p-4 border-gray-300 mt-8">

      <div className="w-full flex flex-col gap-2">
        <div className="flex gap-3">
          <input
            type="text"
            placeholder="ID"
            value={id}
            onChange={(e) => setId(e.target.value)}
            className="flex-1 w-full p-2 border border-gray-300 rounded-lg focus:outline-cyan-500"
          />
          <input
            type="text"
            placeholder="Title"
            value={title}
            onChange={(e) => setTitle(e.target.value)}
            className="flex-3 w-full p-2 border border-gray-300 rounded-lg focus:outline-cyan-500"
          />
          <button
            onClick={() => { addTodo(id, title, description); setId(''); setTitle(''); setDescription(''); }}
            className="flex-2 px-4 py-3 bg-green-500 text-white rounded-lg hover:bg-green-600 hover:cursor-pointer"
          >
            <span className='flex gap-1 justify-center items-center'>
              <FontAwesomeIcon icon={faPlus} />
              Add Todo
            </span>
          </button>
        </div>

        <input
          type="text"
          placeholder="Description"
          value={description}
          onChange={(e) => setDescription(e.target.value)}
          className="flex-4 w-full p-2 border border-gray-300 rounded-lg focus:outline-cyan-500"
        />
      </div>

    </div>
  );
};