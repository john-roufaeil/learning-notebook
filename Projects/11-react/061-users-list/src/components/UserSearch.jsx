import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faSearch, faXmark } from '@fortawesome/free-solid-svg-icons';

export const UserSearch = ({ search, setSearch }) => {
  return (
    <div className="mb-6 flex items-center gap-3 justify-center max-w-xl mx-auto">

      <div className="relative w-full">
        <FontAwesomeIcon
          icon={faSearch}
          className="absolute left-3 top-1/2 -translate-y-1/2 text-gray-400"
        />
        <input
          type="text"
          placeholder="Search by email..."
          value={search}
          onChange={(e) => setSearch(e.target.value)}
          className="w-full pl-10 py-3 border border-gray-300 rounded-lg focus:outline-cyan-500"
        />
      </div>

      <button
        onClick={() => setSearch('')}
        className="px-4 py-3 bg-red-500 text-white rounded-lg hover:bg-red-600 hover:cursor-pointer"
      >
        <span className='flex gap-1 justify-center items-center'>
          <FontAwesomeIcon icon={faXmark} />
          Clear
        </span>
      </button>
    </div>
  );
};