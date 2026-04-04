import { faCartShopping, faTrash } from "@fortawesome/free-solid-svg-icons";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { useSelector, useDispatch } from "react-redux";
import { Link } from "react-router-dom";

function CartPage() {
  const cartItems = useSelector(state => state.cart.items);
  const totalItems = useSelector(state => state.cart.totalItems);
  const totalPrice = useSelector(state => state.cart.totalPrice);
  const dispatch = useDispatch();

  return (
    <div className="min-h-screen bg-gray-50 flex justify-center py-10 px-4">
      <div className="w-full max-w-4xl">
        {totalItems === 0 ? (
          <div className="flex flex-col items-center justify-center text-center mt-20">
            <div className="bg-blue-100 p-6 rounded-full mb-4 aspect-square flex items-center justify-center">
              <FontAwesomeIcon icon={faCartShopping} className="text-blue-500 text-4xl" />
            </div>
            <h1 className="text-3xl font-bold mb-2">Your Cart is Empty</h1>
            <p className="text-gray-500">Looks like you haven’t added anything yet.</p>
            <Link to="/products" className="mt-6 bg-blue-600 text-white py-3 px-6 rounded-xl font-semibold hover:bg-blue-700 transition">
              Browse Products
            </Link>
          </div>
        ) : (
          <>
            <h1 className="text-3xl font-bold mb-6">Shopping Cart</h1>

            <div className="bg-white shadow-md rounded-2xl p-6 space-y-4">
              {Object.values(cartItems).map(item => (
                <div
                  key={item.id}
                  className="flex items-center gap-4 border-b pb-4 last:border-none"
                >
                  <img
                    src={item.images[0]}
                    alt={item.title}
                    className="w-24 h-24 object-cover rounded-xl"
                  />

                  <div className="flex-1">
                    <h2 className="font-semibold text-lg line-clamp-1">
                      {item.title}
                    </h2>
                    <p className="text-gray-500 text-sm">
                      ${item.price.toFixed(2)} each
                    </p>

                    <div className="flex items-center gap-3 mt-2">
                      <button
                        onClick={() =>
                          dispatch({
                            type: "cart/decreaseQuantity",
                            payload: item.id,
                          })
                        }
                        disabled={item.quantity === 1}
                        className={`w-8 h-8 flex items-center justify-center bg-gray-200 rounded hover:bg-gray-300 ${item.quantity === 1 ? "cursor-not-allowed opacity-50" : "cursor-pointer"}`}
                      >
                        -
                      </button>

                      <span className="font-medium">{item.quantity}</span>

                      <button
                        onClick={() =>
                          dispatch({
                            type: "cart/addToCart",
                            payload: item,
                          })
                        }
                        className="w-8 h-8 flex items-center justify-center bg-gray-200 rounded hover:bg-gray-300 cursor-pointer"
                      >
                        +
                      </button>
                    </div>
                  </div>

                  <div className="text-right flex flex-col items-end gap-2">
                    <p className="font-semibold">
                      ${(item.price * item.quantity).toFixed(2)}
                    </p>

                    <button
                      onClick={() =>
                        dispatch({
                          type: "cart/removeFromCart",
                          payload: item.id,
                        })
                      }
                      className="text-red-500 hover:text-red-600 cursor-pointer flex items-center gap-1 text-sm"
                    >
                      <FontAwesomeIcon icon={faTrash} />
                      Remove
                    </button>
                  </div>
                </div>
              ))}
            </div>

            <div className="bg-white shadow-md rounded-2xl p-6 mt-6 flex justify-between items-center">
              <span className="text-lg font-medium">
                Total ({totalItems} items)
              </span>
              <span className="text-2xl font-bold">
                ${totalPrice.toFixed(2)}
              </span>
            </div>
          </>
        )}
      </div>
    </div>
  );
}

export default CartPage;