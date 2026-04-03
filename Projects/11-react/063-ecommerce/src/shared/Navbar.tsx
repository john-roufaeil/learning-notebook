import { faStore } from "@fortawesome/free-solid-svg-icons";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { Link } from "react-router-dom";

function Navbar() {
  return (
    <nav className="flex items-center gap-4 p-4 bg-gray-800 text-white">
      <FontAwesomeIcon icon={faStore} className="text-xl" />
      <div className="flex gap-4 text-lg">
        <Link to="/products" className="hover:underline">Products</Link>
        <Link to="/cart" className="hover:underline">Cart</Link>
      </div>
    </nav>
  )
}

export default Navbar;