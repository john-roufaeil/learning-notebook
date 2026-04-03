import { Link } from "react-router-dom";

function ProductCard({ product }) {
  return (
    <div className="border-2 border-gray-300 p-4 shadow rounded-xl">
      <div className="w-full aspect-square bg-gray-200 overflow-hidden">
        <img
          src={product.images[0]}
          alt={product.title}
          className="w-full h-full object-cover"
        />
      </div>
      <div className="border-t-2 border-gray-300 pt-4">
        <h2 className="font-semibold">{product.title}</h2>
        <p className="text-gray-600">{product.description}</p>
        <div className="flex items-center justify-between mt-4">
          <p className="text-xl font-bold">${product.price.toFixed(2)}</p>
          <Link to={`/products/${product.id}`} className="bg-blue-500 text-white p-2 rounded-lg hover:bg-blue-600 hover:cursor-pointer">
            View Details
          </Link>
        </div>
      </div>
    </div>
  )
}

export default ProductCard;