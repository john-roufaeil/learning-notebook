import { faAngleLeft, faAngleRight, faAngleDoubleLeft, faAngleDoubleRight } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome"

function Pagination({ page, setPage, totalPages }) {
  return (
    <div className="flex items-center justify-center gap-4 my-6">
      <FontAwesomeIcon
        icon={faAngleDoubleLeft}
        onClick={() => setPage(1)}
        className="border-2 border-gray-300 p-2 rounded-lg hover:bg-gray-200 hover:cursor-pointer"
      />

      <FontAwesomeIcon
        icon={faAngleLeft}
        onClick={() => setPage(Math.max(1, page - 1))}
        className="border-2 border-gray-300 p-2 rounded-lg hover:bg-gray-200 hover:cursor-pointer"
      />

      <p className="text-gray-600">Page {page}</p>

      <FontAwesomeIcon
        icon={faAngleRight}
        onClick={() => setPage(Math.min(totalPages, page + 1))}
        className="border-2 border-gray-300 p-2 rounded-lg hover:bg-gray-200 hover:cursor-pointer"
      />

      <FontAwesomeIcon
        icon={faAngleDoubleRight}
        onClick={() => setPage(totalPages)}
        className="border-2 border-gray-300 p-2 rounded-lg hover:bg-gray-200 hover:cursor-pointer"
      />
    </div>
  )
}

export default Pagination;