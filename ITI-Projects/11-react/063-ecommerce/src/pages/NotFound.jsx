import { faCircleXmark } from "@fortawesome/free-solid-svg-icons";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

function NotFound() {
  return (
    <div className="min-h-screen bg-red-50 text-center w-full flex justify-center items-center flex-col ">
      <FontAwesomeIcon icon={faCircleXmark} className="text-red-500 text-6xl mb-4" />
      <h1 className="text-4xl font-bold">Error 404</h1>
      <p className="text-2xl font-semibold">Page Not Found</p>
    </div>
  )
}

export default NotFound;