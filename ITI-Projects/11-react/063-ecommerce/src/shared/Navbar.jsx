import { faStore, faCartShopping, faUser, faSignIn } from "@fortawesome/free-solid-svg-icons";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { Link } from "react-router-dom";
import { useContext } from "react";
import { LanguageContext } from "../contexts/LanguageContext";
import { useSelector } from "react-redux";
import { AuthContext } from "../contexts/AuthContext";

function Navbar() {
  const { lang, setLang } = useContext(LanguageContext);
  const { user } = useContext(AuthContext);
  const cartTotalItems = useSelector(state => state.cart.totalItems);

  return (
    <nav className="p-4 bg-gray-800 text-white flex justify-between items-center">
      <div className="flex items-center gap-2 bg-gray-700 px-3 py-1 rounded cursor-default">
        <FontAwesomeIcon icon={faStore} className="text-2xl" />
        <span className="text-xl">
          Store
        </span>
      </div>

      <div className="flex gap-8">
        <div className="flex gap-6 text-lg">
          <Link to="/products" className="hover:underline">Products</Link>
          <Link to="/contact" className="hover:underline">Contact</Link>
          <Link to="/cart" className="hover:underline relative">
            <FontAwesomeIcon icon={faCartShopping} />
            <span className="absolute -top-2 -right-2 bg-blue-500 text-white text-xs rounded-full h-5 w-5 flex items-center justify-center">
              {cartTotalItems}
            </span>
          </Link>
        </div>
        <div className="rounded-full hover:bg-gray-700 cursor-pointer p-1 aspect-square">
          {user ? (
            <Link to="/profile">
              <FontAwesomeIcon icon={faUser} />
            </Link>
          ) : (
            <Link to="/login">
              <FontAwesomeIcon icon={faSignIn} />
            </Link>
          )}
        </div>
        <select className="bg-gray-700 text-white border-none" value={lang} onChange={(e) => setLang(e.target.value)}>
          <option value="en">English</option>
          <option value="ar">Arabic</option>
        </select>
      </div>
    </nav>
  )
}

export default Navbar;