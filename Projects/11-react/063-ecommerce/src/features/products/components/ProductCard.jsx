import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faStar } from "@fortawesome/free-solid-svg-icons";
import { useNavigate } from "react-router-dom";
import { useDispatch } from "react-redux";

function ProductCard({ product }) {
  const navigate = useNavigate();
  const dispatch = useDispatch();

  const handleCardClick = () => {
    navigate(`/products/${product.id}`);
  };

  const handleAddToCart = (e) => {
    e.stopPropagation();
    console.log("Adding to cart:", product);
    dispatch({ type: "cart/addToCart", payload: product });
  };

  return (
    <div onClick={handleCardClick} className="group border border-gray-200 rounded-2xl overflow-hidden shadow-sm hover:shadow-lg transition cursor-pointer bg-white" >
      <div className="w-full aspect-square bg-gray-100">
        <img
          src={product.images[0]}
          alt={product.title}
          className="w-full h-full object-cover group-hover:scale-105 transition"
        />
      </div>

      <div className="p-4 flex flex-col gap-2">
        <h2 className="font-semibold text-lg line-clamp-1">
          {product.title}
        </h2>

        <p className="text-gray-500 text-sm line-clamp-2">
          {product.description}
        </p>

        <div className="flex justify-between gap-4 items-center">
          <span className="w-fit rounded-xl text-sm">
            {product.rating} ({product.reviews.length}) <FontAwesomeIcon icon={faStar} className="text-yellow-500" />
          </span>
          <span className="bg-gray-200 w-fit px-3 py-1 rounded-xl text-sm">
            {product.category}
          </span>
        </div>

        <div className="flex justify-between gap-4 items-center">
          <div className="mt-2 text-xl font-bold text-gray-800">
            ${product.price.toFixed(2)}
          </div>

          <div onClick={(e) => e.stopPropagation()} className="flex items-center gap-2 mt-3 flex-1" >
            <button onClick={handleAddToCart} className="flex-1 bg-gray-800 text-white py-2 rounded-lg hover:bg-gray-600 hover:cursor-pointer transition">
              Add to Cart
            </button>
          </div>
        </div>

      </div>
    </div>
  );
}

export default ProductCard;