import { faCartShopping } from "@fortawesome/free-solid-svg-icons";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

function CartPage() {
  return (
    <div className="min-h-screen text-center w-full flex justify-center items-center flex-col ">
      <FontAwesomeIcon icon={faCartShopping} className="text-blue-500 text-6xl mb-4" />
      <h1 className="text-4xl font-bold">Your Cart</h1>
      <p className="text-2xl font-semibold">Your cart is empty</p>
    </div>
  )
}

export default CartPage;